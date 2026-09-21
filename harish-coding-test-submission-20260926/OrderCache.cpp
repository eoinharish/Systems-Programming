#include "OrderCache.h"

#include <algorithm>
#include <utility>
#include <limits>

bool OrderCache::isValidOrder(const Order& order)
{
  return !order.orderId().empty() && !order.securityId().empty() &&
         !order.user().empty() && !order.company().empty() &&
         (order.side() == "Buy" || order.side() == "Sell") &&
         order.qty() > 0;
}

void OrderCache::addOrder(Order order)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (!isValidOrder(order))
  {
    return;
  }

  const std::string orderId = order.orderId();
  const std::string user = order.user();
  const std::string securityId = order.securityId();
  const std::string company = order.company();
  const unsigned int qty = order.qty();

  const auto [it, inserted] = m_orders.emplace(orderId, std::move(order));
  if (!inserted)
  {
    return;
  }

  m_ordersByUser[user].insert(orderId);
  m_ordersBySecurity[securityId][qty].insert(orderId);

  auto& securityData = m_matchingData[securityId];
  auto& companyData = securityData.companies[company];

  if (it->second.side() == "Buy")
  {
    securityData.totalBuy += qty;
    companyData.buyQty += qty;
  }
  else
  {
    securityData.totalSell += qty;
    companyData.sellQty += qty;
  }
}

bool OrderCache::removeOrder(const std::string& orderId)
{
  auto orderIt = m_orders.find(orderId);

  if (orderIt == m_orders.end())
  {
    return false;
  }

  const Order& order = orderIt->second;

  const std::string user = order.user();
  const std::string securityId = order.securityId();
  const std::string company = order.company();
  const unsigned int qty = order.qty();

  const bool isBuy = order.side() == "Buy";

  m_orders.erase(orderIt);

  // Remove from user index
  auto userIt = m_ordersByUser.find(user);
  if (userIt != m_ordersByUser.end())
  {
    userIt->second.erase(orderId);
    if (userIt->second.empty())
    {
      m_ordersByUser.erase(userIt);
    }
  }

  // Remove from security index
  auto securityIt = m_ordersBySecurity.find(securityId);
  if (securityIt != m_ordersBySecurity.end())
  {
    auto quantityIt = securityIt->second.find(qty);
    if (quantityIt != securityIt->second.end())
    {
      quantityIt->second.erase(orderId);
      if (quantityIt->second.empty())
      {
        securityIt->second.erase(quantityIt);
      }
    }

    if (securityIt->second.empty())
    {
      m_ordersBySecurity.erase(securityIt);
    }
  }

  auto matchingIt = m_matchingData.find(securityId);
  if (matchingIt != m_matchingData.end())
  {
    auto& securityData = matchingIt->second;
    auto companyIt = securityData.companies.find(company);

    if (companyIt != securityData.companies.end())
    {
      if (isBuy)
      {
        securityData.totalBuy -= qty;
        companyIt->second.buyQty -= qty;
      }
      else
      {
        securityData.totalSell -= qty;
        companyIt->second.sellQty -= qty;
      }

      if (companyIt->second.buyQty == 0 && companyIt->second.sellQty == 0)
      {
        securityData.companies.erase(companyIt);
      }
    }

    if (securityData.totalBuy == 0 && securityData.totalSell == 0)
    {
      m_matchingData.erase(matchingIt);
    }
  }

  return true;
}

void OrderCache::cancelOrder(const std::string& orderId)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (!orderId.empty())
  {
    removeOrder(orderId);
  }
}

void OrderCache::cancelOrdersForUser(const std::string& user)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (user.empty())
  {
    return;
  }

  auto userIt = m_ordersByUser.find(user);
  if (userIt == m_ordersByUser.end())
  {
      return;
  }

  std::vector<std::string> orderIds;
  orderIds.reserve(userIt->second.size());

  for (const auto& orderId : userIt->second)
  {
    orderIds.push_back(orderId);
  }

  for (const auto& orderId : orderIds)
  {
    removeOrder(orderId);
  }
}

void OrderCache::cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (securityId.empty())
  {
    return;
  }

  auto securityIt = m_ordersBySecurity.find(securityId);
  if (securityIt == m_ordersBySecurity.end())
  {
    return;
  }

  auto quantityIt = securityIt->second.lower_bound(minQty);
  if (quantityIt == securityIt->second.end())
  {
    return;
  }

  std::vector<std::string> orderIds;

  for (auto it = quantityIt; it != securityIt->second.end(); it++)
  {
    orderIds.insert(orderIds.end(), it->second.begin(), it->second.end());
  }

  for (const auto& orderId : orderIds)
  {
    removeOrder(orderId);
  }
}

unsigned int OrderCache::getMatchingSizeForSecurity(const std::string& securityId)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  if (securityId.empty())
  {
    return 0;
  }

  auto it = m_matchingData.find(securityId);
  if (it == m_matchingData.end())
  {
    return 0;
  }

  const auto& data = it->second;

  if (data.totalBuy == 0 || data.totalSell == 0)
  {
    return 0;
  }

  uint64_t maxCompanyTotal = 0;

  for (const auto& entry : data.companies)
  {
    const auto& company = entry.second;
    maxCompanyTotal = std::max(maxCompanyTotal, company.buyQty + company.sellQty);
  }

  const uint64_t result = std::min({data.totalBuy, data.totalSell, data.totalBuy + data.totalSell - maxCompanyTotal});

  if (result > std::numeric_limits<unsigned int>::max())
  {
    return std::numeric_limits<unsigned int>::max();
  }

  return static_cast<unsigned int>(result); 
}

std::vector<Order> OrderCache::getAllOrders() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  
  std::vector<Order> result;
  result.reserve(m_orders.size());

  for (const auto& entry : m_orders)
  {
    result.push_back(entry.second);
  }

  return result;
}
