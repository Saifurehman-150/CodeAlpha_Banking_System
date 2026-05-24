The Banking Management System is a console-based C++ application designed to simulate real-world banking operations. It allows users to manage customers, accounts, deposits, withdrawals, transfers, and transaction history in a structured and secure way.

This project is built using Object-Oriented Programming (OOP) concepts without any external frameworks or databases, making it ideal for academic and internship-level submissions.

 Features
 Customer Management
Create new customers
View customer details
Store multiple accounts per customer
 Account Management
Create bank accounts linked to customers
Unique account number generation
Balance tracking
 Transactions
Deposit money
Withdraw money
Transfer funds between accounts
 Account Services
View account details
View last 5 transactions
List all accounts
List all customers
 Concepts Used
Object-Oriented Programming (OOP)
Classes & Objects
Encapsulation
Vectors (STL)
String manipulation
Input validation
Random ID generation
Time handling
Console-based UI design
 System Architecture
Customer  → holds personal info + list of accounts
Account   → manages balance + transactions
Transaction → stores history of operations
BankSystem → controls all operations
 How to Run
 Requirements
C++ Compiler (G++ / MinGW / Visual Studio)
Any IDE (CodeBlocks, DevC++, VS Code)
 Compile & Run
g++ main.cpp -o bank
./bank
 Menu System
1. Create New Customer
2. Create New Account
3. Deposit Money
4. Withdraw Money
5. Transfer Funds
6. View Account Details
7. View Customer Details
8. List All Customers
9. List All Accounts
0. Exit System
 Input Validation

The system ensures:

No empty input allowed
Amount must be positive
Account must exist before transactions
Transfer cannot occur to same account
Proper error messages for invalid actions
 Limitations
Data is stored in memory only (not persistent after exit)
No database integration
Console-based interface only
 Future Improvements
File-based data storage
GUI interface (Qt / SFML)
Login & Admin system
PIN-based authentication
Real-time banking simulation
 Developer Notes

This project is developed for:

DLD / Computer Organization Lab
C++ OOP practice
Internship submission (CodeAlpha / similar programs)

It demonstrates:

Clean architecture
Modular programming
Real-world banking logic simulation
📜 License

This project is for educational purposes only.
