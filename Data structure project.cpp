//Group members Khadija Faisal(231647) Ujala Zaib(231689) Mehreen Ali(231692)
#include<iostream>
#include<iomanip> 
#include<sstream>
#include<fstream>
using namespace std;

// Node class for Linked List 
class Node {
private:
    string action;
    Node* next;

public:
    Node(string act) : action(act), next(nullptr) {}
    string getAction() { return action; }
    void setNext(Node* nxt) { next = nxt; }
    Node* getNext() { return next; }
};

// Custom Stack implementation
class Stack {
private:
    Node* top;

public:
    Stack() : top(nullptr) {}

    void push(const string& action) {
        Node* newNode = new Node(action);
        newNode->setNext(top);
        top = newNode;
    }

    string pop() {
        if (top == nullptr) {
            return "No actions to undo.";
        }
        string action = top->getAction();
        Node* temp = top;
        top = top->getNext();
        delete temp;
        return action;
    }

    bool isEmpty() { return top == nullptr; }

    ~Stack() {
        while (top) {
            Node* temp = top;
            top = top->getNext();
            delete temp;
        }
    }
};

// Custom Queue Implementation for Transaction History
class TransactionQueue {
private:
    struct TransactionNode {
        string data;
        TransactionNode* next;
        TransactionNode(const string& d) : data(d), next(nullptr) {}
    };

    TransactionNode* front;
    TransactionNode* rear;
    int size;
    int capacity;

public:
    TransactionQueue(int cap) : front(nullptr), rear(nullptr), size(0), capacity(cap) {}

    void addTransaction(const string& transaction) {
        TransactionNode* newNode = new TransactionNode(transaction);
        if (size == capacity) {
            // Remove the oldest transaction (front of the queue)
            TransactionNode* temp = front;
            front = front->next;
            delete temp;
            size--;
        }

        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    void displayHistory() {
        if (front == nullptr) {
            cout << "No transactions recorded.\n";
            return;
        }

        cout << "Transaction History:\n";
        TransactionNode* temp = front;
        while (temp) {
            cout << temp->data << "\n";
            temp = temp->next;
        }
    }

    void searchHistory(const string& keyword) {
        if (front == nullptr) {
            cout << "No transactions recorded.\n";
            return;
        }

        bool found = false;
        TransactionNode* temp = front;
        while (temp) {
            if (temp->data.find(keyword) != string::npos) {
                cout << temp->data << "\n";
                found = true;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "No transactions found with keyword: " << keyword << "\n";
        }
    }
    bool isEmpty() const {
        return front == nullptr;
    }
    int sizeofqueue() const {
        return size;
    }
    string getTransaction(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of bounds");
        }

        TransactionNode* temp = front;
        for (int i = 0; i < index; ++i) {
            temp = temp->next;
        }
        return temp->data;
    }



};

// Customer Class Definition
class Customer {
private:
    int accountNumber;
    string name;
    double balance;
    string cnic;
    Customer* next;

public:
    Customer(int accNumber, const string& custName, double initialBalance, string cno)
        : accountNumber(accNumber), name(custName), balance(initialBalance), cnic(cno), next(nullptr) {
    }

    int getAccountNumber() { return accountNumber; }
    string getName() { return name; }
    double getBalance() { return balance; }
    string getcnic() { return cnic; }

    void setBalance(double newBalance) { balance = newBalance; }
    void setName(const string& newName) { name = newName; }
    void setcnic(string cno) { cnic = cno; }
    Customer* getNext() { return next; }
    void setNext(Customer* nextCustomer) { next = nextCustomer; }

    void displayCustomer() {
        cout << "| " << setw(15) << accountNumber << " | "
            << setw(20) << name << " | "
            << setw(10) << balance << " | "
            << setw(17) << cnic << " |" << endl;
    }
};

// Bank Class Definition
class Bank {
private:
    Customer* customerHead;
    TransactionQueue transactionHistory;
    Stack undoStack;

    void cleanupCustomers() {
        while (customerHead) {
            Customer* temp = customerHead;
            customerHead = customerHead->getNext();
            delete temp;
        }
    }

public:
    Bank(size_t historyCapacity) : customerHead(nullptr), transactionHistory(historyCapacity) {}

    ~Bank() { cleanupCustomers(); }

    Customer* findCustomer(int accountNumber) {
        Customer* temp = customerHead;
        while (temp) {
            if (temp->getAccountNumber() == accountNumber) {
                return temp;
            }
            temp = temp->getNext();
        }
        return nullptr;
    }

    bool isValidAccountNumber(int accNumber) {
        return accNumber > 0;
    }

    bool isValidAmount(double amount) {
        return amount > 0;
    }

    // Validate name (only alphabets)
    bool isValidName(const string& name) {
        for (char c : name) {
            if (!isalpha(c) && c != ' ') { // Allow spaces between first and last name
                return false;
            }
        }
        return true;
    }

    // Validate CNIC (13 digits with the correct format)
    bool isValidCNIC(const string& cnic) {
        if (cnic.length() != 15) return false;
        if (cnic[5] != '-' || !isdigit(cnic[0]) || !isdigit(cnic[1]) || !isdigit(cnic[2]) || !isdigit(cnic[3]) || !isdigit(cnic[4]) ||
            !isdigit(cnic[6]) || !isdigit(cnic[7]) || !isdigit(cnic[8]) || !isdigit(cnic[9]) || !isdigit(cnic[10]) ||
            !isdigit(cnic[11]) || !isdigit(cnic[12])) {
            return false;
        }
        return true;
    }

    // Manual validation for numeric amounts (deposit/withdraw)
    bool isValidNumericInput(double& input) {
        string str;
        cin >> str;
        stringstream ss(str);
        ss >> input;
        return ss.eof() && !str.empty() && str.find_first_not_of("0123456789.") == string::npos;
    }

    void addCustomer() {
        int accNumber;
        string name, cnic;
        double balance;

        // Validate Account Number
        while (true) {
            cout << "Enter Account Number: ";
            cin >> accNumber;
            cin.ignore(); // Clear any leftover newline characters in the buffer
            if (!isValidAccountNumber(accNumber)) {
                cout << "Invalid account number. Please enter a valid number greater than 0.\n";
            }
            else {
                break;
            }
        }

        // Validate Name (Only Alphabets)
        while (true) {
            cout << "Enter Name: ";
            getline(cin, name);
            if (!isValidName(name)) {
                cout << "Name must only contain alphabets and spaces. Please try again.\n";
            }
            else {
                break;
            }
        }

        // Validate Balance (Only Numeric)
        while (true) {
            cout << "Enter Initial Deposit Amount: ";
            if (!isValidNumericInput(balance) || balance <= 0) {
                cout << "Invalid amount! Please enter a valid positive number.\n";
            }
            else {
                break;
            }
        }

        // Validate CNIC
        while (true) {
            cout << "Enter CNIC (xxxxx-xxxxxxx-x): ";
            cin >> cnic;

            if (!isValidCNIC(cnic)) {
                cout << "Invalid CNIC format. Please enter a valid CNIC.\n";
            }
            else {
                break;
            }
        }

        Customer* newCustomer = new Customer(accNumber, name, balance, cnic);
        if (!customerHead) {
            customerHead = newCustomer;
        }
        else {
            Customer* temp = customerHead;
            while (temp->getNext()) {
                temp = temp->getNext();
            }
            temp->setNext(newCustomer);
        }
        cout << "Customer added successfully!\n";
        transactionHistory.addTransaction("Added customer " + name + ".");
    }

    void depositAmount() {
        int accountNumber;
        double amount;

        // Validate Account Number
        while (true) {
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (!isValidAccountNumber(accountNumber)) {
                cout << "Invalid account number.\n";
            }
            else {
                break;
            }
        }

        Customer* customer = findCustomer(accountNumber);
        if (!customer) {
            cout << "Account not found.\n";
            return;
        }

        // Validate Deposit Amount
        while (true) {
            cout << "Enter Amount to Deposit: ";
            if (!isValidNumericInput(amount) || amount <= 0) {
                cout << "Deposit amount must be a positive number.\n";
            }
            else {
                break;
            }
        }

        string undoAction = "Deposited " + to_string(amount) + " to account " + to_string(accountNumber);
        undoStack.push(undoAction);

        customer->setBalance(customer->getBalance() + amount);
        cout << "Amount deposited successfully! New Balance: " << customer->getBalance() << "\n";
        transactionHistory.addTransaction("Deposited " + to_string(amount) + " to account " + to_string(accountNumber) + ".");
    }

    void withdrawAmount() {
        int accountNumber;
        double amount;

        // Validate Account Number
        while (true) {
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (!isValidAccountNumber(accountNumber)) {
                cout << "Invalid account number.\n";
            }
            else {
                break;
            }
        }

        Customer* customer = findCustomer(accountNumber);
        if (!customer) {
            cout << "Account not found.\n";
            return;
        }

        // Validate Withdraw Amount
        while (true) {
            cout << "Enter Amount to Withdraw: ";
            if (!isValidNumericInput(amount) || amount <= 0 || amount > customer->getBalance()) {
                cout << "Invalid amount! You cannot withdraw more than the balance.\n";
            }
            else {
                break;
            }
        }

        string undoAction = "Withdrew " + to_string(amount) + " from account " + to_string(accountNumber);
        undoStack.push(undoAction);

        customer->setBalance(customer->getBalance() - amount);
        cout << "Amount withdrawn successfully! New Balance: " << customer->getBalance() << "\n";
        transactionHistory.addTransaction("Withdrew " + to_string(amount) + " from account " + to_string(accountNumber) + ".");
    }

    void displayCustomers() {
        if (!customerHead) {
            cout << "No customers in the system.\n";
            return;
        }

        Customer* temp = customerHead;
        cout << "Customer List:\n";
        cout << "Account Number\t\tName\t\tBalance\t\tCNIC\n";
        cout << "----------------------------------------------------------------------------\n";

        while (temp) {
            temp->displayCustomer();
            temp = temp->getNext();
        }
    }

    void deleteCustomer() {
        int accountNumber;
        cout << "Enter Account Number to Delete: ";
        cin >> accountNumber;

        if (!customerHead) {
            cout << "No customers to delete.\n";
            return;
        }

        Customer* temp = customerHead;
        Customer* prev = nullptr;

        while (temp && temp->getAccountNumber() != accountNumber) {
            prev = temp;
            temp = temp->getNext();
        }

        if (!temp) {
            cout << "Customer not found.\n";
            return;
        }

        // Save undo action
        string undoAction = "Deleted customer " + temp->getName() + " with account number " + to_string(accountNumber);
        undoStack.push(undoAction);

        if (prev) {
            prev->setNext(temp->getNext());
        }
        else {
            customerHead = temp->getNext();
        }

        delete temp;
        cout << "Customer deleted successfully.\n";
        transactionHistory.addTransaction("Deleted customer with account number " + to_string(accountNumber) + ".");
    }


    void undoTransaction() {
        string lastAction = undoStack.pop();

        if (lastAction == "No actions to undo.") {
            cout << lastAction << endl;
            return;
        }

        cout << "Undoing: " << lastAction << endl;

        // Parse the lastAction string to extract details
        size_t pos;
        if ((pos = lastAction.find("Deposited")) != string::npos) {
            // Undo a deposit
            int accountNumber;
            double amount;

            // Find the part of the string after "Deposited" and parse the values manually
            size_t amountPos = lastAction.find("Deposited") + 9;  // After "Deposited"
            size_t toAccountPos = lastAction.find("to account") + 11;  // After "to account"

            // Extract the amount
            string amountStr = lastAction.substr(amountPos, toAccountPos - amountPos - 12);  // Substring between "Deposited" and "to account"
            stringstream(amountStr) >> amount;

            // Extract the account number
            string accountStr = lastAction.substr(toAccountPos);
            stringstream(accountStr) >> accountNumber;

            Customer* customer = findCustomer(accountNumber);
            if (customer) {
                customer->setBalance(customer->getBalance() - amount);
                cout << "Reversed deposit of " << amount << " for account " << accountNumber << ". New Balance: " << customer->getBalance() << "\n";
            }
            else {
                cout << "Account " << accountNumber << " not found. Cannot undo deposit.\n";
            }
        }
        else if ((pos = lastAction.find("Withdrew")) != string::npos) {
            // Undo a withdrawal
            int accountNumber;
            double amount;

            // Extract the amount and account number from the string
            size_t amountPos = lastAction.find("Withdrew") + 8;  // After "Withdrew"
            size_t fromAccountPos = lastAction.find("from account") + 12;  // After "from account"

            // Extract the amount
            string amountStr = lastAction.substr(amountPos, fromAccountPos - amountPos - 12);
            stringstream(amountStr) >> amount;

            // Extract the account number
            string accountStr = lastAction.substr(fromAccountPos);
            stringstream(accountStr) >> accountNumber;

            Customer* customer = findCustomer(accountNumber);
            if (customer) {
                customer->setBalance(customer->getBalance() + amount);
                cout << "Reversed withdrawal of " << amount << " for account " << accountNumber << ". New Balance: " << customer->getBalance() << "\n";
            }
            else {
                cout << "Account " << accountNumber << " not found. Cannot undo withdrawal.\n";
            }
        }
        else if ((pos = lastAction.find("Deleted")) != string::npos) {
            // Undo a customer deletion
            // Additional logic is needed to store and restore deleted customers
            cout << "You can not undo an account once it is deleted.\n Add a New Customer Again";
        }
        else {
            cout << "Unknown action. Cannot undo.\n";
        }
    }



    void searchTransactionHistory() {
        string keyword;
        cout << "Enter keyword to search in transaction history: ";
        cin.ignore();
        getline(cin, keyword);
        transactionHistory.searchHistory(keyword);
    }

    void generateReport() {
        ofstream reportFile("TransactionReport.html");

        if (!reportFile) {
            cout << "Error creating the report file.\n";
            return;
        }

        // HTML and CSS content
        reportFile << "<!DOCTYPE html>\n";
        reportFile << "<html lang=\"en\">\n<head>\n";
        reportFile << "<meta charset=\"UTF-8\">\n";
        reportFile << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
        reportFile << "<title>Bank Management System - Transaction Report</title>\n";
        reportFile << "<style>\n";
        reportFile << "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #FFDAB9; margin: 0; padding: 0; }\n"; // Light orange background
        reportFile << "header { background-color: #4CAF50; color: white; text-align: center; padding: 20px; }\n";
        reportFile << "table { width: 80%; margin: 20px auto; border-collapse: collapse; }\n";
        reportFile << "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }\n";
        reportFile << "th { background-color: #4CAF50; color: white; }\n";
        reportFile << "tr:nth-child(even) { background-color: #f2f2f2; }\n";
        reportFile << "tr:hover { background-color: #ddd; }\n";
        reportFile << "footer { background-color: #4CAF50; color: white; text-align: center; padding: 10px; position: absolute; bottom: 0; width: 100%; }\n";
        reportFile << "</style>\n";
        reportFile << "</head>\n<body>\n";

        // Header section
        reportFile << "<header>\n";
        reportFile << "<h1>Bank Management System</h1>\n";
        reportFile << "<h2>Transaction Report</h2>\n";
        reportFile << "</header>\n";

        // Transactions Table
        reportFile << "<table>\n";
        reportFile << "<tr><th>Transaction Number</th><th>Details</th></tr>\n";



        // Loop through transaction history
        if (transactionHistory.isEmpty()) {
            reportFile << "<tr><td colspan='2'>No transactions available.</td></tr>\n";
        }
        else {
            for (int i = 0; i < transactionHistory.sizeofqueue(); ++i) {
                reportFile << "<tr>\n";
                reportFile << "<td>" << i + 1 << "</td>\n";
                reportFile << "<td>" << transactionHistory.getTransaction(i) << "</td>\n";
                reportFile << "</tr>\n";
            }

        }

        reportFile << "</table>\n";
        reportFile << "</body>\n</html>";

        reportFile.close();
        cout << "Transaction report generated successfully: TransactionReport.html\n";
    }

};

int main() {
    Bank bank(10); // Initialize bank with capacity for 10 transactions

    int choice;
    do {
        system("cls");
        cout << "\n** Bank Management System **\n";
        cout << "1. Add Customers\n";
        cout << "2. Display Customers\n";
        cout << "3. Deposit Amount\n";
        cout << "4. Withdraw Amount\n";
        cout << "5. Delete Customer\n";
        cout << "6. Undo Last Transaction\n";
        cout << "7. Search Transaction History\n";
        cout << "8. Generate Report\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bank.addCustomer();
            system("pause");
            break;
        case 2:
            bank.displayCustomers();
            system("pause");
            break;
        case 3:
            bank.depositAmount();
            system("pause");
            break;
        case 4:
            bank.withdrawAmount();
            system("pause");
            break;
        case 5:
            bank.deleteCustomer();
            system("pause");
            break;
        case 6:
            bank.undoTransaction();
            system("pause");
            break;
        case 7:
            bank.searchTransactionHistory();
            system("pause");
            break;
        case 8:
            bank.generateReport();
            system("pause");
            break;


        case 9:

            cout << "Exiting... Thank you for using the system.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 9);

    return 0;
}



