// UnitTests using Google Test framework.

#include <string>
#include <vector>
#include <map>
#include "OrderCache.h"
#include "gtest/gtest.h"

class OrderCacheTest : public ::testing::Test {
protected:
    OrderCache cache;

    void SetUp() override 
    {
    }

    static std::map<std::string, unsigned int> getOrders(const std::vector<Order>& orders)
    {
        std::map<std::string, unsigned int> result;
        for (const auto& order : orders)
        {
            result[order.orderId()] = order.qty();
        }
        return result;
    }
};

TEST_F(OrderCacheTest, ExampleAddOrder)
{
    cache.addOrder(Order{"OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"});
    ASSERT_EQ(cache.getAllOrders().size(), 1);
}

TEST_F(OrderCacheTest, AddAndGetOrders)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec2", "Sell", 200, "User2", "CompanyB"});

    auto orders = cache.getAllOrders();

    ASSERT_EQ(orders.size(), 2);
    EXPECT_EQ(getOrders(orders).at("Ord1"), 100);
    EXPECT_EQ(getOrders(orders).at("Ord2"), 200);
}

TEST_F(OrderCacheTest, DuplicateOrderIdIsIgnored)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord1", "Sec2", "Sell", 200, "User2", "CompanyB"});

    auto orders = cache.getAllOrders();

    ASSERT_EQ(orders.size(), 1);
    EXPECT_EQ(orders[0].securityId(), "Sec1");
    EXPECT_EQ(orders[0].qty(), 100);
}

TEST_F(OrderCacheTest, InvalidOrdersAreIgnored)
{
    cache.addOrder(Order{"", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord3", "Sec1", "Hold", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord4", "Sec1", "Buy", 0, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord5", "Sec1", "Buy", 100, "", "CompanyA"});
    cache.addOrder(Order{"Ord6", "Sec1", "Buy", 100, "User1", ""});

    EXPECT_TRUE(cache.getAllOrders().empty());
}

TEST_F(OrderCacheTest, CancelOrder)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec1", "Sell", 100, "User2", "CompanyB"});

    cache.cancelOrder("Ord1");

    auto orders = cache.getAllOrders();
    ASSERT_EQ(orders.size(), 1);
    EXPECT_EQ(orders[0].orderId(), "Ord2");
}

TEST_F(OrderCacheTest, CancelMissingOrderDoesNothing)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});

    cache.cancelOrder("Missing");

    EXPECT_EQ(cache.getAllOrders().size(), 1);
}

TEST_F(OrderCacheTest, CancelOrdersForUser)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec2", "Sell", 200, "User1", "CompanyB"});
    cache.addOrder(Order{"Ord3", "Sec1", "Sell", 300, "User2", "CompanyC"});

    cache.cancelOrdersForUser("User1");

    auto orders = cache.getAllOrders();
    ASSERT_EQ(orders.size(), 1);
    EXPECT_EQ(orders[0].orderId(), "Ord3");
}

TEST_F(OrderCacheTest, CancelOrdersForUnknownUserDoesNothing)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});

    cache.cancelOrdersForUser("Missing");

    EXPECT_EQ(cache.getAllOrders().size(), 1);
}

TEST_F(OrderCacheTest, CancelOrdersForSecurityAndMinimumQty)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec1", "Buy", 200, "User2", "CompanyB"});
    cache.addOrder(Order{"Ord3", "Sec1", "Sell", 300, "User3", "CompanyC"});
    cache.addOrder(Order{"Ord4", "Sec2", "Buy", 500, "User4", "CompanyD"});

    cache.cancelOrdersForSecIdWithMinimumQty("Sec1", 200);

    auto orders = getOrders(cache.getAllOrders());

    ASSERT_EQ(orders.size(), 2);
    EXPECT_EQ(orders.count("Ord1"), 1);
    EXPECT_EQ(orders.count("Ord4"), 1);
}

TEST_F(OrderCacheTest, MinimumQtyIsInclusive)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec1", "Buy", 200, "User2", "CompanyB"});

    cache.cancelOrdersForSecIdWithMinimumQty("Sec1", 200);

    auto orders = getOrders(cache.getAllOrders());

    ASSERT_EQ(orders.size(), 1);
    EXPECT_EQ(orders.count("Ord1"), 1);
}

TEST_F(OrderCacheTest, MatchingExample1)
{
    cache.addOrder(Order{"OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"});
    cache.addOrder(Order{"OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB"});
    cache.addOrder(Order{"OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"});
    cache.addOrder(Order{"OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"});
    cache.addOrder(Order{"OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"});
    cache.addOrder(Order{"OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"});
    cache.addOrder(Order{"OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE"});
    cache.addOrder(Order{"OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE"});

    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 0);
    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 2700);
    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 0);
}

TEST_F(OrderCacheTest, MatchingExample2)
{
    cache.addOrder(Order{"OrdId1", "SecId1", "Sell", 100, "User10", "Company2"});
    cache.addOrder(Order{"OrdId2", "SecId3", "Sell", 200, "User8", "Company2"});
    cache.addOrder(Order{"OrdId3", "SecId1", "Buy", 300, "User13", "Company2"});
    cache.addOrder(Order{"OrdId4", "SecId2", "Sell", 400, "User12", "Company2"});
    cache.addOrder(Order{"OrdId5", "SecId3", "Sell", 500, "User7", "Company2"});
    cache.addOrder(Order{"OrdId6", "SecId3", "Buy", 600, "User3", "Company1"});
    cache.addOrder(Order{"OrdId7", "SecId1", "Sell", 700, "User10", "Company2"});
    cache.addOrder(Order{"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
    cache.addOrder(Order{"OrdId9", "SecId2", "Buy", 900, "User6", "Company2"});
    cache.addOrder(Order{"OrdId10", "SecId2", "Sell", 1000, "User5", "Company1"});
    cache.addOrder(Order{"OrdId11", "SecId1", "Sell", 1100, "User13", "Company2"});
    cache.addOrder(Order{"OrdId12", "SecId2", "Buy", 1200, "User9", "Company2"});
    cache.addOrder(Order{"OrdId13", "SecId1", "Sell", 1300, "User1", "Company1"});

    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 300);
    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 1000);
    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 600);
}

TEST_F(OrderCacheTest, MatchingExample3)
{
    cache.addOrder(Order{"OrdId1", "SecId3", "Sell", 100, "User1", "Company1"});
    cache.addOrder(Order{"OrdId2", "SecId3", "Sell", 200, "User3", "Company2"});
    cache.addOrder(Order{"OrdId3", "SecId1", "Buy", 300, "User2", "Company1"});
    cache.addOrder(Order{"OrdId4", "SecId3", "Sell", 400, "User5", "Company2"});
    cache.addOrder(Order{"OrdId5", "SecId2", "Sell", 500, "User2", "Company1"});
    cache.addOrder(Order{"OrdId6", "SecId2", "Buy", 600, "User3", "Company2"});
    cache.addOrder(Order{"OrdId7", "SecId2", "Sell", 700, "User1", "Company1"});
    cache.addOrder(Order{"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
    cache.addOrder(Order{"OrdId9", "SecId1", "Buy", 900, "User5", "Company2"});
    cache.addOrder(Order{"OrdId10", "SecId1", "Sell", 1000, "User1", "Company1"});
    cache.addOrder(Order{"OrdId11", "SecId2", "Sell", 1100, "User6", "Company2"});

    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId1"), 900);
    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId2"), 600);
    EXPECT_EQ(cache.getMatchingSizeForSecurity("SecId3"), 0);
}

TEST_F(OrderCacheTest, SameCompanyOrdersCannotMatch)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 1000, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec1", "Sell", 1000, "User2", "CompanyA"});

    EXPECT_EQ(cache.getMatchingSizeForSecurity("Sec1"), 0);
}

TEST_F(OrderCacheTest, MultipleOrdersFromSameCompanyAreAggregated)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec1", "Buy", 200, "User2", "CompanyA"});
    cache.addOrder(Order{"Ord3", "Sec1", "Sell", 250, "User3", "CompanyB"});

    EXPECT_EQ(cache.getMatchingSizeForSecurity("Sec1"), 250);
}

TEST_F(OrderCacheTest, MatchingChangesAfterCancellation)
{
    cache.addOrder(Order{"Ord1", "Sec1", "Buy", 100, "User1", "CompanyA"});
    cache.addOrder(Order{"Ord2", "Sec1", "Buy", 200, "User2", "CompanyB"});
    cache.addOrder(Order{"Ord3", "Sec1", "Sell", 150, "User3", "CompanyC"});
    cache.addOrder(Order{"Ord4", "Sec1", "Sell", 150, "User4", "CompanyA"});

    EXPECT_EQ(cache.getMatchingSizeForSecurity("Sec1"), 300);

    cache.cancelOrder("Ord3");
    EXPECT_EQ(cache.getMatchingSizeForSecurity("Sec1"), 150);

    cache.cancelOrdersForUser("User1");
    EXPECT_EQ(cache.getMatchingSizeForSecurity("Sec1"), 150);
}

TEST_F(OrderCacheTest, UnknownSecurityHasNoMatches)
{
    EXPECT_EQ(cache.getMatchingSizeForSecurity("Missing"), 0);
}

TEST_F(OrderCacheTest, EmptyInputsDoNothing)
{
    cache.cancelOrder("");
    cache.cancelOrdersForUser("");
    cache.cancelOrdersForSecIdWithMinimumQty("", 100);

    EXPECT_TRUE(cache.getAllOrders().empty());
    EXPECT_EQ(cache.getMatchingSizeForSecurity(""), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
