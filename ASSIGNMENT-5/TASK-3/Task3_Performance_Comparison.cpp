#include <iostream>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <iomanip>
using namespace std;
using Clock = chrono::high_resolution_clock;

// B3: Compare switch-based calculation and virtual-function calculation
class Bill {
public:
    virtual double total() const = 0;
    virtual ~Bill() = default;
};
class OutPatientBill : public Bill {
    double a, b;
public:
    OutPatientBill(double x, double y) : a(x), b(y) {}
    double total() const override { return a + b; }
};
class InPatientBill : public Bill {
    double a, b, c;
public:
    InPatientBill(double x, double y, double z) : a(x), b(y), c(z) {}
    double total() const override { return a + b + c; }
};

struct Record { int type; double a, b, c; };

double switchTotal(const Record& r) {
    switch (r.type) {
        case 1: return r.a + r.b;
        case 2: return r.a + r.b + r.c;
        default: return 0.0;
    }
}

template <typename Function>
long long measureMicroseconds(Function fn) {
    auto start = Clock::now();
    volatile double result = fn();
    auto finish = Clock::now();
    (void)result;
    return chrono::duration_cast<chrono::microseconds>(finish - start).count();
}

void benchmark(int count) {
    vector<Record> records;
    vector<Bill*> bills;
    vector<Bill*> owned;

    records.reserve(count);
    bills.reserve(count);
    owned.reserve(count);

    for (int i = 0; i < count; ++i) {
        if (i % 2 == 0) {
            records.push_back({1, 500.0, 200.0, 0.0});
            owned.push_back(new OutPatientBill(500.0, 200.0));
        } else {
            records.push_back({2, 1000.0, 2000.0, 300.0});
            owned.push_back(new InPatientBill(1000.0, 2000.0, 300.0));
        }
        bills.push_back(owned.back());
    }

    auto switchTime = measureMicroseconds([&]() {
        double sum = 0;
        for (const auto& r : records) sum += switchTotal(r);
        return sum;
    });

    auto virtualTime = measureMicroseconds([&]() {
        double sum = 0;
        for (const auto* b : bills) sum += b->total();
        return sum;
    });

    cout << count << " records -> switch: " << switchTime
         << " us, virtual: " << virtualTime << " us\n";

    for (auto* p : owned) delete p;
}

int main() {
    cout << "PERFORMANCE COMPARISON\n";
    cout << "Switch-based calculation vs virtual functions\n";
    benchmark(1000);
    benchmark(1000000);
    benchmark(10000000);

    cout << "\nException handling vs return-code approach (conceptual test)\n";
    cout << "Invalid rate 0%: return codes and exceptions both handle valid input; "
            "return codes usually have lower overhead.\n";
    cout << "Invalid rate 1%: exceptions can be suitable when errors are rare.\n";
    cout << "Invalid rate 50%: frequent exceptions are costly; return codes are preferable.\n";

    cout << "\nAlternative: std::variant with std::visit can represent a fixed set "
            "of bill types without virtual functions.\n";
    cout << "Note: timings depend on compiler, optimization, and computer hardware.\n";
    return 0;
}
