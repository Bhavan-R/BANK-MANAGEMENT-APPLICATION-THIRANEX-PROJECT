#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <limits>
#include <cctype>

using namespace std;

struct Transaction {
    string timestamp;
    string type;
    double amount;
    double balanceAfter;
    string description;
};

struct Account {
    long long accountNumber;
    string customerName;
    string email;
    string phone;
    double balance;
    string accountType;
    string createdDate;
    string lastTransaction;
    int transactionCount;
    vector<Transaction> transactionHistory;
    string pin;  // Added PIN field
};

class BankManagementSystem {
private:
    vector<Account> accounts;
    unordered_map<long long, size_t> accountIndex;
    const string DATA_FILE = "bank_accounts.dat";
    const long long MAX_ACCOUNTS = 1000000;
    const double MIN_BALANCE = 500.0;
    const double MAX_WITHDRAWAL = 50000.0;
    const int MAX_TRANSACTIONS = 1000;
    const int MAX_PIN_ATTEMPTS = 3;

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool isValidEmail(const string& email) {
        string lowerEmail = email;
        transform(lowerEmail.begin(), lowerEmail.end(), lowerEmail.begin(), ::tolower);
        
        if (email.empty() || email.size() > 100 || !isValidEmailFormat(email)) {
            cout << "Invalid email format. Example: john.doe123@gmail.com" << endl;
            return false;
        }
        
        if (lowerEmail.find("@gmail.com") == string::npos) {
            cout << "Invalid email domain. Use Gmail only. Example: john.doe123@gmail.com" << endl;
            return false;
        }

        for (char c : email) {
            if (isupper(c)) {
                cout << "Email must be lowercase only. Example: john.doe123@gmail.com" << endl;
                return false;
            }
        }
        
        return true;
    }

    bool isValidEmailFormat(const string& email) {
        size_t atPos = email.find('@');
        if (atPos == string::npos || atPos == 0 || atPos == email.size() - 1) return false;
        
        size_t gmailPos = email.rfind("@gmail.com");
        if (gmailPos == string::npos) return false;
        
        string localPart = email.substr(0, atPos);
        if (localPart.empty() || localPart.size() > 30) return false;
        
        for (char c : localPart) {
            if (!isalnum(c) && c != '.' && c != '_') return false;
        }
        
        return gmailPos == email.size() - 10;
    }

    bool isValidPhone(const string& phone) {
        if (phone.size() != 10) return false;
        for (char c : phone) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    bool isValidName(const string& name) {
        if (name.empty() || name.size() > 50) return false;
        for (char c : name) {
            if (!isalpha(c) && !isspace(c)) return false;
        }
        return true;
    }

    bool isValidPin(const string& pin) {
        if (pin.length() != 4) return false;
        for (char c : pin) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    bool verifyPin(long long accountNum, const string& inputPin) {
        if (!accountExists(accountNum)) return false;
        
        auto it = accountIndex.find(accountNum);
        size_t index = it->second;
        const Account& account = accounts[index];
        
        return account.pin == inputPin;
    }

    bool accountExists(long long accountNum) {
        return accountIndex.find(accountNum) != accountIndex.end();
    }

    string formatRupees(double amount) {
        stringstream ss;
        ss << "Rs." << fixed << setprecision(2) << amount;
        return ss.str();
    }

    string formatTimestamp(const chrono::system_clock::time_point& time) {
        auto time_t = chrono::system_clock::to_time_t(time);
        stringstream ss;
        ss << put_time(localtime(&time_t), "%Y-%m-%d %H:%M");
        return ss.str();
    }

    bool authenticateAccount(long long accountNum) {
        string inputPin;
        int attempts = 0;
        
        while (attempts < MAX_PIN_ATTEMPTS) {
            cout << "Enter PIN for account " << accountNum << " (" << (MAX_PIN_ATTEMPTS - attempts) << " attempts left): ";
            cin >> inputPin;
            clearInputBuffer();
            
            if (verifyPin(accountNum, inputPin)) {
                return true;
            }
            
            attempts++;
            if (attempts < MAX_PIN_ATTEMPTS) {
                cout << "Incorrect PIN. ";
            }
        }
        
        cout << "Account locked due to too many incorrect PIN attempts." << endl;
        return false;
    }

    void addTransaction(Account& account, const string& type, double amount, const string& description = "") {
        Transaction trans;
        trans.timestamp = formatTimestamp(chrono::system_clock::now());
        trans.type = type;
        trans.amount = amount;
        trans.balanceAfter = account.balance;
        trans.description = description;
        
        account.transactionHistory.push_back(trans);
        account.lastTransaction = trans.timestamp;
        account.transactionCount++;
        
        if (account.transactionHistory.size() > MAX_TRANSACTIONS) {
            account.transactionHistory.erase(account.transactionHistory.begin());
        }
    }

    Account parseAccountData(const string& line) {
        Account account;
        stringstream ss(line);
        string field;

        getline(ss, field, '|'); account.accountNumber = stoll(field);
        getline(ss, account.customerName, '|');
        getline(ss, account.email, '|');
        getline(ss, account.phone, '|');
        getline(ss, field, '|'); account.balance = stod(field);
        getline(ss, account.accountType, '|');
        getline(ss, account.createdDate, '|');
        getline(ss, account.lastTransaction, '|');
        getline(ss, field, '|'); account.transactionCount = stoi(field);
        getline(ss, account.pin, '|');  // Added PIN parsing

        return account;
    }

    string serializeAccount(const Account& account) {
        stringstream ss;
        ss << account.accountNumber << "|" << account.customerName << "|" << account.email << "|"
           << account.phone << "|" << fixed << setprecision(2) << account.balance << "|"
           << account.accountType << "|" << account.createdDate << "|" << account.lastTransaction << "|"
           << account.transactionCount << "|" << account.pin;  // Added PIN serialization
        return ss.str();
    }

    void saveDatabase() {
        ofstream file(DATA_FILE);
        if (file.is_open()) {
            for (const auto& account : accounts) {
                file << serializeAccount(account) << endl;
            }
            file.close();
        }
    }

    void loadDatabase() {
        accounts.clear();
        accountIndex.clear();
        ifstream file(DATA_FILE);
        string line;

        if (file.is_open()) {
            while (getline(file, line)) {
                if (!line.empty()) {
                    accounts.push_back(parseAccountData(line));
                }
            }
            file.close();
            buildIndex();
        }
    }

    void buildIndex() {
        accountIndex.clear();
        for (size_t i = 0; i < accounts.size(); i++) {
            accountIndex[accounts[i].accountNumber] = i;
        }
    }

public:
    BankManagementSystem() {
        loadDatabase();
        cout << "Bank Management System initialized. Accounts loaded: " << accounts.size() << endl;
    }

    ~BankManagementSystem() {
        saveDatabase();
    }

    long long generateAccountNumber() {
        return 100000000000LL + accounts.size() * 100 + 1;
    }

    bool createAccount() {
        if (accounts.size() >= MAX_ACCOUNTS) {
            cout << "Bank capacity reached. Cannot create new accounts." << endl;
            return false;
        }

        Account newAccount;
        newAccount.accountNumber = generateAccountNumber();
        newAccount.balance = 1000.0;
        auto now = chrono::system_clock::now();
        newAccount.createdDate = formatTimestamp(now);
        newAccount.lastTransaction = newAccount.createdDate;
        newAccount.transactionCount = 0;

        cout << "\n=== Create New Account ===" << endl;
        cout << "Account Number: " << newAccount.accountNumber << endl << endl;

        string nameInput;
        do {
            cout << "Customer Name (letters only): ";
            getline(cin, nameInput);
            if (!isValidName(nameInput)) {
                cout << "Invalid name. Use letters only." << endl;
            }
        } while (!isValidName(nameInput));
        newAccount.customerName = nameInput;

        string emailInput;
        cout << "Email Address: ";
        do {
            getline(cin, emailInput);
            if (!isValidEmail(emailInput)) {
                cout << "Try again: ";
            }
        } while (!isValidEmail(emailInput));
        newAccount.email = emailInput;

        string phoneInput;
        do {
            cout << "Phone Number (10 digits): ";
            getline(cin, phoneInput);
            if (!isValidPhone(phoneInput)) {
                cout << "Phone must be exactly 10 digits." << endl;
            }
        } while (!isValidPhone(phoneInput));
        newAccount.phone = phoneInput;

        cout << "Account Type (SAVINGS/CURRENT): ";
        getline(cin, newAccount.accountType);
        transform(newAccount.accountType.begin(), newAccount.accountType.end(), newAccount.accountType.begin(), ::toupper);

        // Set PIN for new account
        string pinInput;
        do {
            cout << "Set 4-digit PIN: ";
            cin >> pinInput;
            clearInputBuffer();
            if (!isValidPin(pinInput)) {
                cout << "PIN must be exactly 4 digits." << endl;
            }
        } while (!isValidPin(pinInput));
        newAccount.pin = pinInput;

        accounts.push_back(newAccount);
        accountIndex[newAccount.accountNumber] = accounts.size() - 1;
        
        cout << "\nAccount created successfully!" << endl;
        cout << "Initial Balance: " << formatRupees(newAccount.balance) << endl;
        cout << "PIN set successfully. Keep it secure!" << endl;
        return true;
    }

    bool creditAmount(long long accountNum, double amount) {
        if (!accountExists(accountNum)) {
            cout << "Account number does not exist." << endl;
            return false;
        }

        if (!authenticateAccount(accountNum)) {
            return false;
        }

        if (amount <= 0) {
            cout << "Amount must be positive." << endl;
            return false;
        }

        auto it = accountIndex.find(accountNum);
        size_t index = it->second;
        Account& account = accounts[index];
        
        double oldBalance = account.balance;
        account.balance += amount;
        account.lastTransaction = formatTimestamp(chrono::system_clock::now());
        account.transactionCount++;
        
        addTransaction(account, "CREDIT", amount, "Cash Deposit");
        
        cout << "Credit successful!" << endl;
        cout << "Amount credited: " << formatRupees(amount) << endl;
        cout << "Previous balance: " << formatRupees(oldBalance) << endl;
        cout << "New balance: " << formatRupees(account.balance) << endl;
        return true;
    }

    bool debitAmount(long long accountNum, double amount) {
        if (!accountExists(accountNum)) {
            cout << "Account number does not exist." << endl;
            return false;
        }

        if (!authenticateAccount(accountNum)) {
            return false;
        }

        if (amount <= 0 || amount > MAX_WITHDRAWAL) {
            cout << "Invalid amount. Maximum limit: " << formatRupees(MAX_WITHDRAWAL) << endl;
            return false;
        }

        auto it = accountIndex.find(accountNum);
        size_t index = it->second;
        Account& account = accounts[index];
        
        if (account.balance - amount < MIN_BALANCE) {
            cout << "Insufficient balance. Minimum required: " << formatRupees(MIN_BALANCE) << endl;
            cout << "Current balance: " << formatRupees(account.balance) << endl;
            return false;
        }

        double oldBalance = account.balance;
        account.balance -= amount;
        account.lastTransaction = formatTimestamp(chrono::system_clock::now());
        account.transactionCount++;
        
        addTransaction(account, "DEBIT", amount, "Cash Withdrawal");
        
        cout << "Debit successful!" << endl;
        cout << "Amount debited: " << formatRupees(amount) << endl;
        cout << "Previous balance: " << formatRupees(oldBalance) << endl;
        cout << "New balance: " << formatRupees(account.balance) << endl;
        return true;
    }

    void viewTransactionHistory(long long accountNum) {
        if (!accountExists(accountNum)) {
            cout << "Account number does not exist." << endl;
            return;
        }

        if (!authenticateAccount(accountNum)) {
            return;
        }

        auto it = accountIndex.find(accountNum);
        size_t index = it->second;
        const Account& account = accounts[index];
        
        cout << "\n" << string(120, '=') << endl;
        cout << "TRANSACTION HISTORY - Account: " << accountNum << endl;
        cout << string(120, '=') << endl;
        cout << left << setw(20) << "Date/Time" 
             << setw(12) << "Type" 
             << setw(15) << "Amount" 
             << setw(18) << "Balance After" 
             << setw(35) << "Description" << endl;
        cout << string(120, '-') << endl;

        if (account.transactionHistory.empty()) {
            cout << "No transactions found." << endl;
        } else {
            for (int i = account.transactionHistory.size() - 1; i >= 0; i--) {
                const auto& trans = account.transactionHistory[i];
                cout << left << setw(20) << trans.timestamp.substr(11)
                     << setw(12) << trans.type 
                     << setw(15) << formatRupees(trans.amount)
                     << setw(18) << formatRupees(trans.balanceAfter)
                     << setw(35) << trans.description << endl;
            }
        }
        cout << string(120, '=') << endl;
        cout << "Total Transactions: " << account.transactionCount << endl;
    }

    void checkBalance(long long accountNum) {
        if (!accountExists(accountNum)) {
            cout << "Account number does not exist." << endl;
            return;
        }

        if (!authenticateAccount(accountNum)) {
            return;
        }

        auto it = accountIndex.find(accountNum);
        size_t index = it->second;
        const Account& account = accounts[index];
        
        cout << "\n" << string(60, '=') << endl;
        cout << "ACCOUNT STATEMENT" << endl;
        cout << string(60, '=') << endl;
        cout << left << setw(20) << "Account Number:" << account.accountNumber << endl;
        cout << left << setw(20) << "Customer:" << account.customerName << endl;
        cout << left << setw(20) << "Account Type:" << account.accountType << endl;
        cout << left << setw(20) << "Available Balance:" << formatRupees(account.balance) << endl;
        cout << left << setw(20) << "Total Transactions:" << account.transactionCount << endl;
        cout << left << setw(20) << "Last Transaction:" << account.lastTransaction << endl;
        cout << string(60, '=') << endl;
    }

    void transferFunds() {
        long long fromAcc, toAcc;
        double amount;

        cout << "Enter sender account number: ";
        while (!(cin >> fromAcc) || !accountExists(fromAcc)) {
            clearInputBuffer();
            cout << "Sender account does not exist. Enter valid account: ";
        }

        if (!authenticateAccount(fromAcc)) {
            return;
        }

        cout << "Enter receiver account number: ";
        while (!(cin >> toAcc) || !accountExists(toAcc) || fromAcc == toAcc) {
            clearInputBuffer();
            cout << "Receiver account invalid or same as sender. Enter valid account: ";
        }

        cout << "Enter transfer amount: ";
        while (!(cin >> amount) || amount <= 0 || amount > MAX_WITHDRAWAL) {
            clearInputBuffer();
            cout << "Invalid amount (max " << formatRupees(MAX_WITHDRAWAL) << "): ";
        }
        clearInputBuffer();

        cout << "\nProcessing transfer from " << fromAcc << " to " << toAcc << " (" << formatRupees(amount) << ")" << endl;
        
        auto fromIt = accountIndex.find(fromAcc);
        auto toIt = accountIndex.find(toAcc);
        Account& fromAccount = accounts[fromIt->second];
        Account& toAccount = accounts[toIt->second];
        
        double fromOldBalance = fromAccount.balance;
        double toOldBalance = toAccount.balance;
        
        if (debitAmount(fromAcc, amount)) {
            creditAmount(toAcc, amount);
            addTransaction(fromAccount, "TRANSFER-OUT", amount, "Transfer to " + to_string(toAcc));
            addTransaction(toAccount, "TRANSFER-IN", amount, "Transfer from " + to_string(fromAcc));
            cout << "Transfer completed successfully!" << endl;
        } else {
            cout << "Transfer failed." << endl;
        }
    }

    void displayAllAccounts() {
        if (accounts.empty()) {
            cout << "No accounts in database." << endl;
            return;
        }

        cout << "\n" << string(160, '=') << endl;
        cout << "BANK ACCOUNTS OVERVIEW" << endl;
        cout << string(160, '=') << endl;
        cout << left << setw(15) << "Account No" 
             << setw(25) << "Customer Name" 
             << setw(40) << "Email" 
             << setw(15) << "Phone" 
             << setw(15) << "Balance" 
             << setw(12) << "Type" 
             << setw(18) << "Transactions" << endl;
        cout << string(160, '-') << endl;

        for (const auto& account : accounts) {
            cout << left << setw(15) << account.accountNumber
                 << setw(25) << account.customerName.substr(0, 24)
                 << setw(40) << account.email.substr(0, 39)
                 << setw(15) << account.phone
                 << setw(15) << formatRupees(account.balance)
                 << setw(12) << account.accountType
                 << setw(18) << account.transactionCount << endl;
        }
        cout << string(160, '=') << endl;
        cout << "Total Accounts: " << accounts.size() << endl;
    }

    void showBankStatistics() {
        if (accounts.empty()) {
            cout << "No data available." << endl;
            return;
        }

        double totalBalance = 0;
        double maxBalance = 0;
        int totalTransactions = 0;
        unordered_map<string, pair<int, double>> typeStats;

        for (const auto& account : accounts) {
            totalBalance += account.balance;
            maxBalance = max(maxBalance, account.balance);
            totalTransactions += account.transactionCount;
            typeStats[account.accountType].first++;
            typeStats[account.accountType].second += account.balance;
        }

        cout << "\n=== Bank Statistics ===" << endl;
        cout << "Total Accounts: " << accounts.size() << endl;
        cout << "Total Deposits: " << formatRupees(totalBalance) << endl;
        cout << "Highest Balance: " << formatRupees(maxBalance) << endl;
        cout << "Total Transactions: " << totalTransactions << endl;
        cout << "\nAccount Types:" << endl;
        for (const auto& stat : typeStats) {
            cout << "  " << stat.first << ": " << stat.second.first 
                 << " accounts, Total " << formatRupees(stat.second.second) << endl;
        }
    }

    long long getValidAccountNumber() {
        long long accNum;
        cout << "Enter Account Number: ";
        while (!(cin >> accNum) || !accountExists(accNum)) {
            clearInputBuffer();
            cout << "Account does not exist. Enter valid account number: ";
        }
        clearInputBuffer();
        return accNum;
    }

    double getValidAmount() {
        double amount;
        cout << "Enter Amount: ";
        while (!(cin >> amount) || amount <= 0) {
            clearInputBuffer();
            cout << "Invalid amount. Enter positive value: ";
        }
        clearInputBuffer();
        return amount;
    }

    void displayMainMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    BANK MANAGEMENT SYSTEM" << endl;
        cout << string(60, '=') << endl;
        cout << "1.  Create New Account" << endl;
        cout << "2.  Credit Amount" << endl;
        cout << "3.  Debit Amount" << endl;
        cout << "4.  Check Balance" << endl;
        cout << "5.  View Transaction History" << endl;
        cout << "6.  Transfer Funds" << endl;
        cout << "7.  View All Accounts" << endl;
        cout << "8.  Bank Statistics" << endl;
        cout << "0.  Exit System" << endl;
        cout << string(60, '=') << endl;
        cout << "Enter choice: ";
    }

    void run() {
        int choice;
        while (true) {
            displayMainMenu();
            while (!(cin >> choice) || choice < 0 || choice > 8) {
                clearInputBuffer();
                cout << "Invalid input. Enter 0-8: ";
            }
            clearInputBuffer();

            switch (choice) {
                case 1:
                    createAccount();
                    break;
                case 2: {
                    long long acc = getValidAccountNumber();
                    double amt = getValidAmount();
                    creditAmount(acc, amt);
                    break;
                }
                case 3: {
                    long long acc = getValidAccountNumber();
                    double amt = getValidAmount();
                    debitAmount(acc, amt);
                    break;
                }
                case 4: {
                    long long acc = getValidAccountNumber();
                    checkBalance(acc);
                    break;
                }
                case 5: {
                    long long acc = getValidAccountNumber();
                    viewTransactionHistory(acc);
                    break;
                }
                case 6:
                    transferFunds();
                    break;
                case 7:
                    displayAllAccounts();
                    break;
                case 8:
                    showBankStatistics();
                    break;
                case 0:
                    cout << "\nSystem shutdown. All data saved." << endl;
                    return;
                default:
                    cout << "Invalid selection." << endl;
            }
        }
    }
};

int main() {
    BankManagementSystem bank;
    bank.run();
    return 0;
}