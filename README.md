**Overview**
BankFlow Manager is a comprehensive C++ banking system that implements core banking operations with advanced features like transaction history tracking, undo functionality, and HTML report generation. This system demonstrates object-oriented programming principles with custom implementations of linked lists, stacks, and queues.

**Key Features**
**Customer Management:**

Add new customers with validation for account numbers, names, and CNIC formats

View all customer details in a formatted display

Delete customer accounts

Transaction Processing:

Secure deposit and withdrawal operations with balance validation

Transaction history tracking with search capability

Undo functionality for recent transactions

Data Structures:

Custom linked list implementation for customer records

Stack for undo operations

Circular queue for transaction history with fixed capacity

Reporting:

Generate professional HTML reports of transaction history

Styled with CSS for better readability

Technical Implementation
Core Data Structures:

Node class for linked list operations

Stack class for LIFO undo operations

TransactionQueue (circular queue) for maintaining history

Validation System:

Account number validation

Name validation (alphabets only)

CNIC format validation (xxxxx-xxxxxxx-x)

Numeric input validation for amounts

Error Handling:

Comprehensive input validation

Graceful handling of edge cases
