# Order Cache

A C++ implementation of an in-memory order cache supporting order insertion, cancellation, indexing, and calculation of the maximum matchable quantity for a security.

The implementation is designed to handle a large number of active orders efficiently while keeping the data structures simple and avoiding pointer-based ownership.

## Features

* Add and cancel individual orders
* Cancel all orders belonging to a user
* Cancel orders for a security above a minimum quantity
* Calculate the maximum matchable quantity for a security
* Retrieve all active orders
* Efficient indexing for user and security-based operations
* Input validation and duplicate order handling
* Optional thread safety using a single mutex
* Google Test based unit tests

## Design

The cache maintains the orders in a primary hash table and maintains additional indexes for operations that need to locate groups of orders efficiently.

```text
OrderCache
│
├── m_orders
│   └── orderId → Order
│
├── m_ordersByUser
│   └── user → order IDs
│
├── m_ordersBySecurity
│   └── securityId → quantity → order IDs
│
└── m_matchingData
    └── securityId → company → buy/sell quantities
```

### Primary Order Storage

```cpp
std::unordered_map<std::string, Order> m_orders;
```

The order ID is unique, so an `unordered_map` provides average O(1) lookup, insertion, and removal.

This map is the source of truth for active orders.

### User Index

```cpp
std::unordered_map<
    std::string,
    std::unordered_set<std::string>
> m_ordersByUser;
```

This allows all orders belonging to a user to be located without scanning every active order.

### Security and Quantity Index

```cpp
std::unordered_map<
    std::string,
    std::map<
        unsigned int,
        std::unordered_set<std::string>
    >
> m_ordersBySecurity;
```

The `std::map` is used for quantities because `cancelOrdersForSecIdWithMinimumQty()` requires finding all quantities greater than or equal to a threshold.

This allows the implementation to use:

```cpp
lower_bound(minQty)
```

instead of scanning every order.

### Matching Data

For each security, the cache maintains total buy/sell quantities and the quantities belonging to each company:

```cpp
struct CompanyData
{
    uint64_t buyQty = 0;
    uint64_t sellQty = 0;
};

struct SecurityData
{
    uint64_t totalBuy = 0;
    uint64_t totalSell = 0;

    std::unordered_map<std::string, CompanyData> companies;
};
```

This allows `getMatchingSizeForSecurity()` to calculate the result without iterating over every individual order.

## Matching Logic

Orders can match when:

* They belong to the same security.
* One order is a Buy and the other is a Sell.
* The orders belong to different companies.
* The same quantity cannot be matched more than once.

For a security:

* `B` = total buy quantity
* `S` = total sell quantity
* `M` = maximum combined buy + sell quantity belonging to a single company

The maximum matchable quantity is:

```text
min(B, S, B + S - M)
```

The first two terms limit the result by the total available buy and sell quantities.

The third term accounts for the fact that orders from the same company cannot match each other. The largest company can therefore restrict the amount of quantity that can participate in valid cross-company matches.

This avoids performing pairwise matching between individual orders.

## Complexity

Let:

* `N` = total number of active orders
* `K` = number of orders affected by a cancellation
* `Q` = number of distinct quantities for a security
* `C` = number of companies for a security

| Operation                              |   Complexity |
| -------------------------------------- | -----------: |
| `addOrder()`                           | O(1) average |
| `cancelOrder()`                        | O(1) average |
| `cancelOrdersForUser()`                |         O(K) |
| `cancelOrdersForSecIdWithMinimumQty()` | O(log Q + K) |
| `getMatchingSizeForSecurity()`         |         O(C) |
| `getAllOrders()`                       |         O(N) |

The implementation uses `uint64_t` internally for aggregate quantities to avoid overflow when summing quantities across a large number of orders.

## Thread Safety

The cache can be used safely from multiple threads by protecting the shared state with a single `std::mutex`.

The mutex protects the complete update of all indexes so that the cache cannot be observed in a partially updated state.

The implementation uses `std::lock_guard<std::mutex>` for RAII-based locking.

The matching and cancellation operations are protected as complete logical operations rather than using individual atomic variables, since the cache maintains consistency across multiple data structures.

## Validation and Edge Cases

The implementation handles:

* Empty order IDs
* Empty security IDs
* Empty users or companies
* Invalid order sides
* Zero quantities
* Duplicate order IDs
* Cancelling non-existent orders
* Cancelling orders for users with no active orders
* Securities with no matching orders

Invalid orders are ignored and duplicate order IDs do not replace an existing order.

## Testing

The project uses Google Test.

The test suite covers:

* Order insertion
* Duplicate orders
* Individual cancellation
* User-based cancellation
* Security/quantity-based cancellation
* Matching calculations
* Multiple companies
* Same-company orders
* Empty and invalid inputs
* Large numbers of orders
* Edge cases

The matching logic was also validated against independently calculated matching results on randomized test cases.

## Build

### Requirements

* C++17 compatible compiler
* CMake
* Google Test

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
```

On macOS:

```bash
cmake --build . -j$(sysctl -n hw.ncpu)
```

### Run Tests

```bash
ctest --output-on-failure
```

Or run the test executable directly:

```bash
./OrderCacheTest
```

## Project Structure

```text
.
├── CMakeLists.txt
├── OrderCache.h
├── OrderCache.cpp
├── OrderCacheTest.cpp
├── TESTING.md
├── README.md
└── .gitignore
```

Build artifacts are kept under `build/` and excluded from version control.

## Notes

The implementation uses only the C++ standard library. No Boost or other third-party libraries are required by the implementation.

The primary design goal is to keep the implementation efficient and straightforward while maintaining consistency between the primary order storage and its secondary indexes.
