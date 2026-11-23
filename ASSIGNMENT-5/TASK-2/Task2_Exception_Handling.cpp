#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
using namespace std;

// B2: Custom exception hierarchy
class BillingError : public runtime_error {
public:
    explicit BillingError(const string& message) : runtime_error(message) {}
};

class InvalidChargeError : public BillingError {
public:
    InvalidChargeError() : BillingError("Charge cannot be negative.") {}
};

class CoverageError : public BillingError {
public:
    CoverageError() : BillingError("Claim amount is greater than the insurance cover.") {}
};

class UnknownPackageError : public BillingError {
public:
    UnknownPackageError() : BillingError("Unknown hospital package selected.") {}
};

double calculateClaim(double charge, double cover, int packageChoice) {
    try {
        if (charge < 0) throw InvalidChargeError();
        if (packageChoice < 1 || packageChoice > 3) throw UnknownPackageError();
        if (charge > cover) throw CoverageError();

        double patientShare = charge * 0.20; // patient pays 20%
        return patientShare;
    } catch (const CoverageError&) {
        cout << "Claim validation failed inside calculation function.\n";
        throw; // rethrow to caller
    }
}

int main() {
    double charge, cover;
    int packageChoice;

    cout << "HOSPITAL INSURANCE CLAIM\n";
    cout << "Enter bill amount: ";
    if (!(cin >> charge)) {
        cout << "Error: Invalid input. Please enter a numeric amount.\n";
        return 1;
    }

    cout << "Enter insurance cover amount: ";
    if (!(cin >> cover)) {
        cout << "Error: Invalid insurance cover input.\n";
        return 1;
    }

    cout << "Select package (1-General, 2-Surgery, 3-Emergency): ";
    if (!(cin >> packageChoice)) {
        cout << "Error: Invalid package input.\n";
        return 1;
    }

    try {
        double patientAmount = calculateClaim(charge, cover, packageChoice);
        cout << "Claim accepted.\n";
        cout << "Patient amount to pay: Rs. " << patientAmount << "\n";
    } catch (const InvalidChargeError& e) {
        cout << "Invalid charge: " << e.what() << "\n";
    } catch (const CoverageError& e) {
        cout << "Coverage error: " << e.what() << "\n";
    } catch (const UnknownPackageError& e) {
        cout << "Package error: " << e.what() << "\n";
    } catch (const bad_alloc&) {
        cout << "Memory allocation failed.\n";
    } catch (const exception& e) {
        cout << "General error: " << e.what() << "\n";
    }

    return 0;
}

/*
Testing examples:
1) Bill 1000, cover 5000, package 1 -> accepted; patient pays 200.
2) Bill -100 -> InvalidChargeError.
3) Bill 6000, cover 5000 -> CoverageError (rethrow demonstrated).
4) Package 9 -> UnknownPackageError.
5) Enter letters instead of a number -> input validation error.

bad_alloc is caught because it can occur during dynamic memory allocation;
this small program does not deliberately exhaust system memory.
*/
