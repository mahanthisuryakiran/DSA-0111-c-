#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
using namespace std;

// B4: Existing abstract interfaces
class Bill {
public:
    virtual double calculateTotal() const = 0;
    virtual string name() const = 0;
    virtual ~Bill() = default;
};

class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(double c, double t) : consultation(c), tests(t) {}
    double calculateTotal() const override { return consultation + tests; }
    string name() const override { return "Outpatient"; }
};

class InPatientBill : public Bill {
    double room, treatment;
public:
    InPatientBill(double r, double t) : room(r), treatment(t) {}
    double calculateTotal() const override { return room + treatment; }
    string name() const override { return "Inpatient"; }
};

// New bill type added without changing the existing classes
class TelemedicineBill : public Bill {
    double onlineConsultation, reportFee;
public:
    TelemedicineBill(double c, double r) : onlineConsultation(c), reportFee(r) {}
    double calculateTotal() const override { return onlineConsultation + reportFee; }
    string name() const override { return "Telemedicine"; }
};

class Payer {
public:
    virtual double patientShare(double amount) const = 0;
    virtual string name() const = 0;
    virtual ~Payer() = default;
};

class SelfPay : public Payer {
public:
    double patientShare(double amount) const override { return amount; }
    string name() const override { return "Self Pay"; }
};

// New payer type added without changing existing payer classes
class EmployeeHealthPlan : public Payer {
    double coverage;
public:
    EmployeeHealthPlan(double percent) : coverage(percent) {
        if (percent < 0 || percent > 100)
            throw invalid_argument("Coverage must be between 0 and 100.");
    }
    double patientShare(double amount) const override {
        return amount * (1.0 - coverage / 100.0);
    }
    string name() const override { return "Employee Health Plan"; }
};

int main() {
    try {
        vector<unique_ptr<Bill>> bills;
        bills.push_back(make_unique<OutPatientBill>(500, 300));
        bills.push_back(make_unique<InPatientBill>(2000, 3500));
        bills.push_back(make_unique<TelemedicineBill>(700, 100));

        vector<unique_ptr<Payer>> payers;
        payers.push_back(make_unique<SelfPay>());
        payers.push_back(make_unique<EmployeeHealthPlan>(80));

        cout << fixed << setprecision(2);
        cout << "EXTENDED HOSPITAL BILLING SYSTEM\n";
        for (const auto& bill : bills) {
            cout << "\n" << bill->name() << " Bill: Rs. "
                 << bill->calculateTotal() << "\n";
            for (const auto& payer : payers) {
                cout << payer->name() << " payment: Rs. "
                     << payer->patientShare(bill->calculateTotal()) << "\n";
            }
        }

        cout << "\nNew bill and payer added successfully.\n";
        cout << "Existing classes were not modified.\n";
        cout << "Smart pointers manage memory automatically (RAII).\n";
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

/*
For memory-safety testing with GCC:
g++ -std=c++17 -fsanitize=address -g Task4_Extending_Billing_System.cpp -o task4
./task4

If the online compiler does not support AddressSanitizer, run the normal
program and mention this limitation in the report. A plug-in architecture
could load new bill or payer implementations as separate modules.
*/
