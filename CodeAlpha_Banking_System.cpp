
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cctype>
#include <cstdlib>
using namespace std;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

// Clear screen function
void clearScreen() {
    system("cls");
}

string generateId() {
    time_t now = time(0);
    string id = to_string(now);
    for (char& c : id) {
        if (isdigit(c)) c = '0' + (c - '0' + rand()) % 10;
    }
    return id.substr(id.length() - 6);
}

bool isValidAmount(double amount) {
    return amount > 0;
}

string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

void pressEnter() {
    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}

// ==========================================
// CLASS: TRANSACTION
// ==========================================

class Transaction {
private:
    string transactionId;
    string timestamp;
    string type;
    double amount;
    double balanceAfter;
    string description;
    string relatedAccount;

public:
    Transaction(string type, double amount, double balanceAfter, string description = "") {
        this->transactionId = generateId();

        time_t now = time(0);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);
        this->timestamp = buffer;
        this->timestamp.erase(remove(this->timestamp.begin(), this->timestamp.end(), '\n'), this->timestamp.end());

        this->type = type;
        this->amount = amount;
        this->balanceAfter = balanceAfter;
        this->description = description;
    }

    void setRelatedAccount(string acc) { this->relatedAccount = acc; }

    string getId() { return transactionId; }
    string getTimestamp() { return timestamp; }
    string getType() { return type; }
    double getAmount() { return amount; }
    double getBalanceAfter() { return balanceAfter; }
    string getDescription() { return description; }
    string getRelatedAccount() { return relatedAccount; }

    void display() {
        cout << "  [" << timestamp << "] "
            << setw(12) << left << type
            << " | Amount: $" << fixed << setprecision(2) << amount
            << " | Balance: $" << balanceAfter;
        if (!description.empty()) {
            cout << " | " << description;
        }
        cout << endl;
    }
};

// ==========================================
// CLASS: ACCOUNT
// ==========================================

class Account {
private:
    string accountNumber;
    string customerId;
    double balance;
    vector<Transaction> transactions;

public:
    Account(string accountNumber, string customerId) {
        this->accountNumber = accountNumber;
        this->customerId = customerId;
        this->balance = 0.0;
    }

    string getAccountNumber() { return accountNumber; }
    string getCustomerId() { return customerId; }
    double getBalance() { return balance; }
    vector<Transaction>& getTransactions() { return transactions; }

    bool deposit(double amount) {
        if (!isValidAmount(amount)) {
            cout << "\n Error: Amount must be positive!" << endl;
            return false;
        }

        balance += amount;
        Transaction t("Deposit", amount, balance, "Counter/ATM");
        transactions.push_back(t);
        return true;
    }

    bool withdraw(double amount) {
        if (!isValidAmount(amount)) {
            cout << "\n Error: Amount must be positive!" << endl;
            return false;
        }

        if (balance < amount) {
            cout << "\n Error: Insufficient funds!" << endl;
            return false;
        }

        balance -= amount;
        Transaction t("Withdrawal", amount, balance, "Counter/ATM");
        transactions.push_back(t);
        return true;
    }

    void transferIn(double amount, string fromAccount) {
        balance += amount;
        string desc = "From: " + fromAccount;
        Transaction t("Transfer In", amount, balance, desc);
        t.setRelatedAccount(fromAccount);
        transactions.push_back(t);
    }

    bool transferOut(double amount, string toAccount) {
        if (!isValidAmount(amount)) {
            cout << "\n Error: Amount must be positive!" << endl;
            return false;
        }

        if (balance < amount) {
            cout << "\n Error: Insufficient funds!" << endl;
            return false;
        }

        balance -= amount;
        string desc = "To: " + toAccount;
        Transaction t("Transfer Out", amount, balance, desc);
        t.setRelatedAccount(toAccount);
        transactions.push_back(t);
        return true;
    }

    void showHistory(int limit = 10) {
        if (transactions.empty()) {
            cout << "  No transactions found." << endl;
            return;
        }

        cout << "\n--- Recent Transactions ---" << endl;
        int count = 0;
        for (int i = transactions.size() - 1; i >= 0 && count < limit; i--) {
            transactions[i].display();
            count++;
        }
    }

    void displayInfo() {
        cout << "\n=======================================\n";
        cout << "        ACCOUNT INFORMATION\n";
        cout << "=======================================\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Customer ID:    " << customerId << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "=======================================\n";
    }
};

// ==========================================
// CLASS: CUSTOMER
// ==========================================

class Customer {
private:
    string customerId;
    string name;
    string phone;
    string email;
    vector<string> accounts;

public:
    Customer(string customerId, string name, string phone = "", string email = "") {
        this->customerId = customerId;
        this->name = name;
        this->phone = phone;
        this->email = email;
    }

    string getCustomerId() { return customerId; }
    string getName() { return name; }
    string getPhone() { return phone; }
    string getEmail() { return email; }
    vector<string>& getAccounts() { return accounts; }

    void addAccount(string accountNumber) {
        accounts.push_back(accountNumber);
    }

    void displayInfo() {
        cout << "\n=======================================\n";
        cout << "        CUSTOMER INFORMATION\n";
        cout << "=======================================\n";
        cout << "Customer ID: " << customerId << endl;
        cout << "Name:        " << name << endl;
        cout << "Phone:       " << phone << endl;
        cout << "Email:       " << email << endl;
        cout << "Total Accounts: " << accounts.size() << endl;
        cout << "=======================================\n";
    }
};

// ==========================================
// CLASS: BANK SYSTEM
// ==========================================

class BankSystem {
private:
    vector<Customer> customers;
    vector<Account> accounts;

public:
    Customer* findCustomer(string customerId) {
        for (auto& cust : customers) {
            if (cust.getCustomerId() == customerId) {
                return &cust;
            }
        }
        return nullptr;
    }

    Account* findAccount(string accountNumber) {
        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accountNumber) {
                return &acc;
            }
        }
        return nullptr;
    }

    bool createCustomer() {
        clearScreen();
        cout << "\n--- CREATE NEW CUSTOMER ---\n";

        string name, phone, email;
        cout << "Enter Name: ";
        getline(cin, name);
        if (name.empty() || name.find_first_not_of(' ') == string::npos) {
            cout << "\n Error: Name cannot be empty!" << endl;
            return false;
        }

        cout << "Enter Phone: ";
        getline(cin, phone);

        cout << "Enter Email: ";
        getline(cin, email);

        if (!email.empty() && email.find('@') == string::npos) {
            cout << " Warning: Email format seems invalid (ignored)." << endl;
            email = "";
        }

        string id = "C" + generateId();
        Customer newCustomer(id, name, phone, email);
        customers.push_back(newCustomer);

        cout << "\n CUSTOMER CREATED SUCCESSFULLY!" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Customer ID: " << id << endl;
        cout << "Name: " << name << endl;

        return true;
    }

    bool createAccount() {
        clearScreen();
        cout << "\n--- CREATE NEW ACCOUNT ---\n";

        string customerId;
        cout << "Enter Customer ID: ";
        getline(cin, customerId);
        customerId = toUpper(customerId);

        Customer* cust = findCustomer(customerId);
        if (cust == nullptr) {
            cout << "\n Error: Customer not found! Please create customer first." << endl;
            return false;
        }

        string accNum = "A" + generateId();
        Account newAccount(accNum, customerId);
        accounts.push_back(newAccount);
        cust->addAccount(accNum);

        cout << "\n ACCOUNT CREATED SUCCESSFULLY!" << endl;
        cout << "-----------------------------------" << endl;
        cout << "Account Number: " << accNum << endl;
        cout << "Customer: " << cust->getName() << endl;

        return true;
    }

    bool deposit() {
        clearScreen();
        cout << "\n--- DEPOSIT MONEY ---\n";

        string accNum;
        cout << "Enter Account Number: ";
        getline(cin, accNum);
        accNum = toUpper(accNum);

        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "\n Error: Account not found!" << endl;
            return false;
        }

        double amount;
        cout << "Enter Amount to Deposit: $";
        cin >> amount;
        cin.ignore();

        if (acc->deposit(amount)) {
            cout << "\n DEPOSIT SUCCESSFUL!" << endl;
            cout << "Current Balance: $" << fixed << setprecision(2) << acc->getBalance() << endl;
            return true;
        }
        return false;
    }

    bool withdraw() {
        clearScreen();
        cout << "\n--- WITHDRAW MONEY ---\n";

        string accNum;
        cout << "Enter Account Number: ";
        getline(cin, accNum);
        accNum = toUpper(accNum);

        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "\n Error: Account not found!" << endl;
            return false;
        }

        double amount;
        cout << "Enter Amount to Withdraw: $";
        cin >> amount;
        cin.ignore();

        if (acc->withdraw(amount)) {
            cout << "\n WITHDRAWAL SUCCESSFUL!" << endl;
            cout << "Current Balance: $" << fixed << setprecision(2) << acc->getBalance() << endl;
            return true;
        }
        return false;
    }

    bool transfer() {
        clearScreen();
        cout << "\n--- TRANSFER FUNDS ---\n";

        string fromAcc, toAcc;
        cout << "Enter From Account Number: ";
        getline(cin, fromAcc);
        fromAcc = toUpper(fromAcc);

        cout << "Enter To Account Number: ";
        getline(cin, toAcc);
        toAcc = toUpper(toAcc);

        if (fromAcc == toAcc) {
            cout << "\n❌ Error: Cannot transfer to the same account!" << endl;
            return false;
        }

        Account* sender = findAccount(fromAcc);
        Account* receiver = findAccount(toAcc);

        if (sender == nullptr) {
            cout << "\n Error: Sender account not found!" << endl;
            return false;
        }
        if (receiver == nullptr) {
            cout << "\n Error: Receiver account not found!" << endl;
            return false;
        }

        double amount;
        cout << "Enter Amount to Transfer: $";
        cin >> amount;
        cin.ignore();

        if (sender->transferOut(amount, toAcc)) {
            receiver->transferIn(amount, fromAcc);
            cout << "\n TRANSFER SUCCESSFUL!" << endl;
            cout << "Amount transferred: $" << fixed << setprecision(2) << amount << endl;
            cout << "Sender New Balance: $" << sender->getBalance() << endl;
            return true;
        }
        return false;
    }

    bool viewAccountDetails() {
        clearScreen();
        cout << "\n--- VIEW ACCOUNT DETAILS ---\n";

        string accNum;
        cout << "Enter Account Number: ";
        getline(cin, accNum);
        accNum = toUpper(accNum);

        Account* acc = findAccount(accNum);
        if (acc == nullptr) {
            cout << "\n Error: Account not found!" << endl;
            return false;
        }

        acc->displayInfo();
        cout << "\n--- TRANSACTION HISTORY (Last 5) ---\n";
        acc->showHistory(5);

        return true;
    }

    bool viewCustomerDetails() {
        clearScreen();
        cout << "\n--- VIEW CUSTOMER DETAILS ---\n";

        string custId;
        cout << "Enter Customer ID: ";
        getline(cin, custId);
        custId = toUpper(custId);

        Customer* cust = findCustomer(custId);
        if (cust == nullptr) {
            cout << "\n Error: Customer not found!" << endl;
            return false;
        }

        cust->displayInfo();

        cout << "Accounts:" << endl;
        for (string accNum : cust->getAccounts()) {
            Account* acc = findAccount(accNum);
            if (acc) {
                cout << "  - " << accNum << " (Balance: $" << acc->getBalance() << ")" << endl;
            }
        }

        return true;
    }

    void listAllCustomers() {
        clearScreen();
        cout << "\n--- ALL CUSTOMERS ---\n";
        if (customers.empty()) {
            cout << "No customers found." << endl;
            return;
        }

        for (auto& cust : customers) {
            cout << "ID: " << cust.getCustomerId()
                << " | Name: " << cust.getName()
                << " | Accounts: " << cust.getAccounts().size() << endl;
        }
    }

    void listAllAccounts() {
        clearScreen();
        cout << "\n--- ALL ACCOUNTS ---\n";
        if (accounts.empty()) {
            cout << "No accounts found." << endl;
            return;
        }

        for (auto& acc : accounts) {
            cout << "Acc#: " << acc.getAccountNumber()
                << " | Customer ID: " << acc.getCustomerId()
                << " | Balance: $" << acc.getBalance() << endl;
        }
    }
};

// ==========================================
// MAIN MENU
// ==========================================

void displayMainMenu() {
    cout << "\n";
    cout << "========================================\n";
    cout << "          BANKING SYSTEM MENU           \n";
    cout << "========================================\n";
    cout << "  1. Create New Customer                \n";
    cout << "  2. Create New Account                 \n";
    cout << "  3. Deposit Money                      \n";
    cout << "  4. Withdraw Money                     \n";
    cout << "  5. Transfer Funds                     \n";
    cout << "  6. View Account Details               \n";
    cout << "  7. View Customer Details              \n";
    cout << "  8. List All Customers                 \n";
    cout << "  9. List All Accounts                  \n";
    cout << "  0. Exit System                        \n";
    cout << "Select Option: ";
}

int main() {
    BankSystem bank;
    int choice;

    srand(time(0));
    clearScreen();

    while (true) {
        displayMainMenu();
        cin >> choice;
        cin.ignore();

        bool success = true;

        switch (choice) {
        case 1:
            bank.createCustomer();
            pressEnter();
            break;
        case 2:
            bank.createAccount();
            pressEnter();
            break;
        case 3:
            success = bank.deposit();
            if (!success) pressEnter();
            else pressEnter();
            break;
        case 4:
            success = bank.withdraw();
            if (!success) pressEnter();
            else pressEnter();
            break;
        case 5:
            success = bank.transfer();
            if (!success) pressEnter();
            else pressEnter();
            break;
        case 6:
            bank.viewAccountDetails();
            pressEnter();
            break;
        case 7:
            bank.viewCustomerDetails();
            pressEnter();
            break;
        case 8:
            bank.listAllCustomers();
            pressEnter();
            break;
        case 9:
            bank.listAllAccounts();
            pressEnter();
            break;
        case 0:
            clearScreen();
            cout << "\n\n";
          
            cout << "   Thank you for using our system   \n";
            cout << "            GOODBYE!                \n";
           
            return 0;
        }
    }