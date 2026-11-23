#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
using namespace std;

// B1: Abstract Bill class and runtime polymorphism
class Bill {
public:
    virtual double calculateTotal() const = 0;
    virtual string getType() const = 0;
    virtual ~Bill() { cout << "Bill object destroyed\n"; }
};

class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(double c, double t) : consultation(c), tests(t) {}
    double calculateTotal() const override { return consultation + tests; }
    string getType() const override { return "Outpatient"; }
};

class InPatientBill : public Bill {
    double room, treatment, medicine;
public:
    InPatientBill(double r, double t, double m) : room(r), treatment(t), medicine(m) {}
    double calculateTotal() const override { return room + treatment + medicine; }
    string getType() const override { return "Inpatient"; }
};

class DayCareBill : public Bill {
    double procedure, medicine;
public:
    DayCareBill(double p, double m) : procedure(p), medicine(m) {}
    double calculateTotal() const override { return procedure + medicine; }
    string getType() const override { return "Day care"; }
};

class EmergencyBill : public Bill {
    double emergencyFee, treatment;
public:
    EmergencyBill(double e, double t) : emergencyFee(e), treatment(t) {}
    double calculateTotal() const override { return emergencyFee + treatment; }
    string getType() const override { return "Emergency"; }
};

// Abstract payer hierarchy
class Payer {
public:
    virtual double amountToPay(double billAmount) const = 0;
    virtual string getName() const = 0;
    virtual ~Payer() = default;
};

class SelfPay : public Payer {
public:
    double amountToPay(double amount) const override { return amount; }
    string getName() const override { return "Self Pay"; }
};

class Insurance : public Payer {
    double coveragePercent;
public:
    Insurance(double p) : coveragePercent(p) {}
    double amountToPay(double amount) const override {
        return amount * (1.0 - coveragePercent / 100.0);
    }
    string getName() const override { return "Insurance"; }
};

class GovernmentScheme : public Payer {
    double coveragePercent;
public:
    GovernmentScheme(double p) : coveragePercent(p) {}
    double amountToPay(double amount) const override {
        return amount * (1.0 - coveragePercent / 100.0);
    }
    string getName() const override { return "Government Scheme"; }
};

int main() {
    vector<unique_ptr<Bill>> bills;
    bills.push_back(make_unique<OutPatientBill>(500, 800));
    bills.push_back(make_unique<InPatientBill>(2500, 6000, 1500));
    bills.push_back(make_unique<DayCareBill>(3000, 700));
    bills.push_back(make_unique<EmergencyBill>(2000, 4500));

    vector<unique_ptr<Payer>> payers;
    payers.push_back(make_unique<SelfPay>());
    payers.push_back(make_unique<Insurance>(70));
    payers.push_back(make_unique<GovernmentScheme>(90));

    cout << fixed << setprecision(2);
    cout << "HOSPITAL BILLING SYSTEM\n";
    for (const auto& bill : bills) {
        double total = bill->calculateTotal();
        cout << "\nBill Type: " << bill->getType()
             << "\nTotal Bill: Rs. " << total << "\n";
        for (const auto& payer : payers) {
            cout << payer->getName() << " pays: Rs. "
                 << payer->amountToPay(total) << "\n";
        }
    }
    cout << "\nRuntime polymorphism demonstrated using base-class pointers.\n";
    return 0;
}
