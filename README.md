# Bank Management Application

## 1. Overview

The Bank Management System is a fully functional console-based application developed in C++ to simulate real-world banking operations in a structured and efficient manner. The system enables users to manage customer accounts, perform financial transactions, and maintain detailed records with high accuracy and reliability.

It supports essential banking functionalities such as account creation, deposits, withdrawals, fund transfers, balance inquiries, and transaction history tracking. Each operation is designed with strict validation rules to ensure data consistency and prevent invalid inputs.

The system uses file handling mechanisms to store account data persistently, allowing information to be retained across multiple executions. Additionally, hash-based indexing is implemented using an unordered map to achieve fast and efficient lookup of account records, even when handling a large number of accounts.

Overall, the project demonstrates a scalable and modular approach to building a banking application using core programming concepts in C++.

---

## 2. Objectives

* To design a structured banking system using C++
* To implement secure and validated financial operations
* To manage multiple user accounts efficiently
* To maintain transaction history for auditing purposes
* To ensure persistent data storage using file handling
* To optimize performance using efficient data structures
* To apply object-oriented programming concepts in a real-world scenario

---

## 3. Key Features

### Account Management

* Creation of new accounts with unique account numbers
* Storage of customer details including name, email, and phone
* Support for multiple account types (Savings and Current)

### Financial Transactions

* Deposit (credit) operations with validation
* Withdrawal (debit) operations with constraints
* Fund transfer between accounts
* Automatic balance updates after each transaction

### Transaction Tracking

* Maintains a history of all transactions
* Records transaction type, amount, timestamp, and balance
* Supports viewing transaction history in reverse chronological order

### Data Management

* Persistent storage using file handling
* Automatic loading of data on startup
* Automatic saving of data on program exit

### Analytics and Insights

* Total number of accounts
* Total bank balance
* Highest account balance
* Total transaction count
* Account type distribution

---

## 4. System Architecture

### Data Structures Used

#### Vector

Used to dynamically store all account records.

#### Unordered Map (Hash Index)

Maps account numbers to their positions in the vector, enabling constant-time lookup.

#### Structures

**Account Structure**

* Account Number
* Customer Name
* Email
* Phone Number
* Balance
* Account Type
* Created Date
* Last Transaction Date
* Transaction Count
* Transaction History (Vector of transactions)

**Transaction Structure**

* Timestamp
* Transaction Type (CREDIT, DEBIT, TRANSFER)
* Amount
* Balance After Transaction
* Description

---

## 5. Functional Modules

### 5.1 Account Creation

* Generates unique account numbers
* Validates user input fields
* Initializes account with default balance
* Stores account in memory and file

### 5.2 Credit Operation

* Accepts deposit amount
* Updates account balance
* Logs transaction details

### 5.3 Debit Operation

* Checks minimum balance requirement
* Enforces withdrawal limits
* Updates balance and logs transaction

### 5.4 Fund Transfer

* Validates sender and receiver accounts
* Performs debit and credit operations
* Logs transfer transactions for both accounts

### 5.5 Balance Inquiry

* Displays account information
* Shows current balance and last transaction

### 5.6 Transaction History

* Displays all transactions in structured format
* Shows timestamp, type, amount, and description

### 5.7 Account Overview

* Displays all accounts in tabular format
* Provides a summary of account details

### 5.8 Bank Statistics

* Calculates total accounts
* Computes total and highest balances
* Tracks total transactions
* Displays account type distribution

---

## 6. Validation Rules

### Email Validation

* Must follow proper format
* Restricted to Gmail domain
* Only lowercase characters allowed

### Phone Number

* Must contain exactly 10 digits
* Only numeric characters allowed

### Name Validation

* Only alphabets and spaces allowed

### Transaction Rules

* Minimum balance must be maintained
* Withdrawal limit enforced
* Amount must be positive

---

## 7. File Handling and Data Persistence

* Data stored in `bank_accounts.dat`
* Uses delimiter-based format (`|`)
* Data loaded at program startup
* Data saved automatically on exit

### Sample Data Format

```
AccountNumber|Name|Email|Phone|Balance|Type|CreatedDate|LastTransaction|TransactionCount
```

---

## 8. Performance and Optimization

* Fast account lookup using hash indexing
* Efficient memory usage with dynamic arrays
* Controlled transaction history size
* Scalable design supporting large datasets

---

## 9. How to Run

### Compile

```bash
g++ main.cpp -o bank_system
```

### Execute

```bash
./bank_system
```

---

## 10. Project Structure

```
Bank-Management-System/
 ┣ main.cpp
 ┣ bank_accounts.dat
 ┗ README.md
```

---

## 11. Sample Workflow

1. Create a new account
2. Deposit money
3. Withdraw money
4. Transfer funds
5. Check balance
6. View transaction history

---

## 12. Key Highlights

* Real-world banking simulation
* Efficient data retrieval using hashing
* Strong validation and error handling
* Transaction tracking system
* Clean and modular design

---

## 13. Limitations

* Console-based interface
* No authentication system
* No database integration
* Single-user environment

---

## 14. Future Enhancements

* Graphical user interface
* Database integration (MySQL/SQLite)
* Authentication and security features
* Advanced reporting and analytics
* Multi-user support

---

## 15. Output Screenshots

### Main Menu Interface

<img width="759" height="494" alt="image" src="https://github.com/user-attachments/assets/cc3c533a-405b-45ff-9857-c2bbbe67f175" />


### Account Creation

<img width="723" height="795" alt="image" src="https://github.com/user-attachments/assets/fc6e15a3-738a-4aa7-ac61-0e20b77bf765" />


### Credit Transaction

<img width="757" height="835" alt="image" src="https://github.com/user-attachments/assets/23917f33-6138-4940-98c6-bad079a0a274" />


### Debit Transaction

<img width="752" height="825" alt="image" src="https://github.com/user-attachments/assets/e9a80990-3417-45ca-bee0-6d64be5047e4" />


### Fund Transfer

<img width="993" height="865" alt="image" src="https://github.com/user-attachments/assets/287f528e-c971-4fbf-a1b6-667eab6ecc4e" />


### Transaction History

<img width="960" height="817" alt="image" src="https://github.com/user-attachments/assets/306370f8-9b80-4d99-9102-92fbae48f3e0" />


### Account Overview

<img width="838" height="790" alt="image" src="https://github.com/user-attachments/assets/2e62f3dd-a02b-4013-a90f-0fd30b649875" />


### Bank Statistics

<img width="828" height="786" alt="image" src="https://github.com/user-attachments/assets/351fc820-5868-44fa-81ac-a8a15d0e7e10" />

---

## 16. Conclusion

This project demonstrates the design and implementation of a banking system using C++. It highlights the use of data structures, file handling, and object-oriented programming to build a scalable and efficient application. The system serves as a strong foundation for developing advanced financial management software.

---

