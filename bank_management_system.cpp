#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// Class to handle individual account data
class Account {
private:
    int accountNo;
    char name[50];
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNo;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter Initial Deposit: ";
        cin >> balance;
        cout << "\n--- Account Created Successfully ---" << endl;
    }

    void showAccount() const {
        cout << "\nAcc No. : " << accountNo;
        cout << "\nHolder  : " << name;
        cout << "\nBalance : $" << fixed << setprecision(2) << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    void withdraw(double amount) {
        if (amount > balance)
            cout << "Insufficient balance!" << endl;
        else
            balance -= amount;
    }

    int getAccNo() const { return accountNo; }
};

// Function prototypes for file handling
void writeAccount();
void displayBalance(int n);
void depositWithdraw(int n, int option);

int main() {
    int choice, num;

    do {
        cout << "\n===============================";
        cout << "\n   BANK MANAGEMENT SYSTEM";
        cout << "\n===============================";
        cout << "\n1. NEW ACCOUNT";
        cout << "\n2. DEPOSIT AMOUNT";
        cout << "\n3. WITHDRAW AMOUNT";
        cout << "\n4. BALANCE ENQUIRY";
        cout << "\n5. EXIT";
        cout << "\n\nSelect Option (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeAccount();
                break;
            case 2:
                cout << "\nEnter Account Number: "; cin >> num;
                depositWithdraw(num, 1);
                break;
            case 3:
                cout << "\nEnter Account Number: "; cin >> num;
                depositWithdraw(num, 2);
                break;
            case 4:
                cout << "\nEnter Account Number: "; cin >> num;
                displayBalance(num);
                break;
            case 5:
                cout << "\nThank you for using our banking system!";
                break;
            default:
                cout << "\nInvalid choice!";
        }
    } while (choice != 5);

    return 0;
}

// --- File Management Functions ---

void writeAccount() {
    Account acc;
    ofstream outFile("bank_data.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

void displayBalance(int n) {
    Account acc;
    bool found = false;
    ifstream inFile("bank_data.dat", ios::binary);

    if (!inFile) {
        cout << "File could not be opened!! Press any Key...";
        return;
    }

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == n) {
            acc.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found) cout << "\nAccount number does not exist.";
}

void depositWithdraw(int n, int option) {
    double amt;
    bool found = false;
    Account acc;
    fstream File("bank_data.dat", ios::binary | ios::in | ios::out);

    if (!File) {
        cout << "File could not be opened!!";
        return;
    }

    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char*>(&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            if (option == 1) {
                cout << "\nEnter amount to DEPOSIT: ";
                cin >> amt;
                acc.deposit(amt);
            } else {
                cout << "\nEnter amount to WITHDRAW: ";
                cin >> amt;
                acc.withdraw(amt);
            }
            // Move file pointer back to update the record
            int pos = (-1) * static_cast<int>(sizeof(acc));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            cout << "\nRecord Updated.";
            found = true;
        }
    }
    File.close();
    if (!found) cout << "\nRecord Not Found.";
}
