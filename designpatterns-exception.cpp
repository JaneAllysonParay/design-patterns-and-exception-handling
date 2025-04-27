/* Paray, Jane Allyson L. | IT2C | Inteprog
This program is all about the Exercise: Design Patterns and Exception Handling in C++*/
#include <iostream>  // Header file for input and output in C++
#include <string>    // For string manipulation
#include <fstream>   // For file operations
#include <iomanip>   // For setting precision in output
#include <stdexcept> // For exception handling
using namespace std; // For convenience and easy use of 'cin','cout', and other names

// Helper function to uppercase a string
string toUpper(string str)
{
    for (char &c : str)
    {
        c = toupper(c);
    }
    return str;
}

// Centralized function to log order
void logOrder(int orderID, double amount, const string &method)
{
    ofstream logFile("order_log.txt", ios::app);
    if (logFile.is_open())
    {
        logFile << "Order ID: " << orderID
                << " has been successfully checked out and paid using " << method << endl;
        logFile.close();
    }
    else
    {
        cout << "Error: Unable to open log file!" << endl;
    }
}

// Get a valid integer input
int getValidInt(const string &prompt, int min, int max)
{
    string input;
    int value;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        try
        {
            if (input.empty())
            {
                throw invalid_argument("Input cannot be empty! Please enter a valid whole number.");
            }
            for (char c : input)
            {
                if (!isdigit(c))
                {
                    throw invalid_argument("Invalid input! Please enter a valid whole number.");
                }
            }
            value = stoi(input);
            if (value < min || value > max)
            {
                throw out_of_range("Input out of range! Please try again.");
            }
            return value;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
}

// Interface class (pure virtual)
class Interface
{
public:
    virtual void interface() = 0;
    virtual ~Interface() {}
};

// Product class
class Product
{
private:
    string id, name;
    double price;

public:
    // Constructor
    Product() : id(""), name(""), price(0.0) {}
    Product(string id, string name, double price) : id(id), name(name), price(price) {}

    // Getter methods
    string getID() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
};

// Payment abstract class
class Payment
{
public:
    virtual void processPayment(double amount, int oID) = 0;
    virtual string getMethodName() = 0;
    virtual ~Payment() {}
};

// Payment Methods: Cash
class Cash : public Payment
{
public:
    // Override method to get payment method name
    string getMethodName() override { return "Cash"; }

    // Override method to process payment
    void processPayment(double amount, int oID) override
    {
        cout << "Processing cash payment of Php " << fixed << setprecision(2) << amount
             << " for Order ID: " << oID << endl;
    }
};

// Payment Methods: Card
class Card : public Payment
{
public:
    // Override method to get payment method name
    string getMethodName() override { return "Credit/Debit Card"; }

    // Override method to process payment
    void processPayment(double amount, int oID) override
    {
        cout << "Processing card payment of Php " << fixed << setprecision(2) << amount
             << " for Order ID: " << oID << endl;
    }
};

// Payment Methods: GCash
class GCash : public Payment
{
public:
    // Override method to get payment method name
    string getMethodName() override { return "GCash"; }

    // Override method to process payment
    void processPayment(double amount, int oID) override
    {
        cout << "Processing GCash payment of Php " << fixed << setprecision(2) << amount
             << " for Order ID: " << oID << endl;
    }
};

// Singleton for Payment processing
class PaymentProcess
{
private:
    static PaymentProcess *instance;
    PaymentProcess() {}

public:
    // Singleton instance method
    static PaymentProcess *getInstance()
    {
        if (!instance)
        {
            instance = new PaymentProcess();
        }
        return instance;
    }
    // Method to process payment
    void pay(Payment *payment, double amount, int id)
    {
        payment->processPayment(amount, id);
    }
};

PaymentProcess *PaymentProcess::instance = nullptr; // Initialize static member

// Get a valid string input (Y/N validation)
string getValidAns(const string &prompt, bool (*isValid)(const string &))
{
    string input;
    bool valid = false;
    while (!valid)
    {
        cout << prompt;
        getline(cin, input);
        input = toUpper(input);

        if (input.empty())
        {
            cout << "Input cannot be empty! Please try again." << endl;
            continue;
        }
        if (!isValid(input))
        {
            cout << "Invalid input! Please enter Y or N." << endl;
            continue;
        }
        return input;
    }
    return ""; // This line will never be reached but necessary for the compiler
}

// Get a valid product ID
string getValidID(const string &prompt, bool (*isValid)(const string &))
{
    string input;
    bool valid = false;
    while (!valid)
    {
        cout << prompt;
        getline(cin, input);
        input = toUpper(input);

        try
        {
            if (input.empty())
            {
                throw invalid_argument("Input cannot be empty! Please enter a valid product ID.");
            }
            if (!isValid(input))
            {
                throw invalid_argument("Invalid input! Please enter a valid product ID.");
            }
            if (input.length() != 3)
            {
                throw out_of_range("Product ID should be 3 characters! Please enter a valid product ID.");
            }
            return input;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
    return ""; // This return is unreachable but necessary for the compiler
}

// View Orders class
class ViewOrders : public Interface
{
private:
    // Order structure to hold order details
    struct OrderDetails
    {
        int id;
        string method;
        double total;
        int qty[5];
        Product *products[5];
        int cartSize;
    };

    // Array to hold multiple orders
    OrderDetails orders[10];
    // Counter for the number of orders
    int orderCount;

public:
    // Constructor to initialize order count
    ViewOrders() : orderCount(0) {}

    // Method to set order details
    void setOrder(int id, string method, Product *products[], int qty[], int cartSize, double total)
    {
        if (orderCount >= 10)
        {
            cout << "Error: Maximum number of orders reached. Cannot store more orders." << endl;
            return;
        }

        orders[orderCount].id = id;
        orders[orderCount].method = method;
        orders[orderCount].total = total;
        orders[orderCount].cartSize = cartSize;

        for (int i = 0; i < cartSize; i++)
        {
            orders[orderCount].products[i] = products[i];
            orders[orderCount].qty[i] = qty[i];
        }

        orderCount++;
    }

    // Method to display all orders
    void interface() override
    {
        if (orderCount == 0)
        {
            cout << "No orders have been placed yet." << endl;
            return;
        }

        for (int i = 0; i < orderCount; i++)
        {
            cout << "==========================================================\n";
            cout << "                  CUSTOMER'S ORDERS\n";
            cout << "==========================================================\n";
            cout << "Order ID: " << orders[i].id << endl;
            cout << "Payment Method: " << orders[i].method << endl;
            cout << "Total Amount: Php " << fixed << setprecision(2) << orders[i].total << endl;
            cout << "Order Details: \n";
            cout << left << setw(12) << "Product ID" << "| " << setw(15) << "Name" << "| " << setw(10) << "Price" << "| " << setw(10) << "Quantity" << endl;
            cout << string(57, '-') << endl;
            for (int j = 0; j < orders[i].cartSize; ++j)
            {
                cout << left << setw(12) << orders[i].products[j]->getID() << "| " << setw(15) << orders[i].products[j]->getName()
                     << "| Php " << fixed << setprecision(2) << orders[i].products[j]->getPrice()
                     << "| " << orders[i].qty[j] << endl;
            }
            cout << "=========================================================\n";
        }
    }
};

// ShoppingCart class
class ShoppingCart : public Interface
{
private:
    Product *cart[5];
    int qty[5], cartSize;

public:
    // Constructor to initialize cart size and quantities
    ShoppingCart() : qty(), cartSize(0) {}

    // Method to add product to cart
    void addToCart(Product *product, int quantity)
    {
        if (cartSize < 5)
        {
            cart[cartSize] = product;
            qty[cartSize] = quantity;
            cartSize++;
        }
        else
        {
            cout << "Shopping cart is full!" << endl;
        }
    }
    // Method to remove product from cart
    void clearCart()
    {
        cartSize = 0;
    }

    // Method to display shopping cart contents
    void interface() override
    {
        if (cartSize == 0)
        {
            cout << "Shopping cart is empty!" << endl;
            return;
        }

        cout << "=========================================================\n";
        cout << "                       SHOPPING CART\n";
        cout << "=========================================================\n";
        cout << left << setw(12) << "Product ID" << "| " << setw(15) << "Name" << "| " << setw(10) << "Price" << "| " << setw(10) << "Quantity" << endl;
        cout << string(57, '-') << endl;
        for (int i = 0; i < cartSize; ++i)
        {
            cout << left << setw(12) << cart[i]->getID() << "| " << setw(15) << cart[i]->getName()
                 << "| Php " << fixed << setprecision(2) << cart[i]->getPrice()
                 << "| " << qty[i] << endl;
        }
        cout << "=========================================================\n";

        double total = 0.0;
        for (int i = 0; i < cartSize; i++)
        {
            total += cart[i]->getPrice() * qty[i];
        }
        cout << "Total Amount: Php " << fixed << setprecision(2) << total << endl;
    }

    // Method to checkout and process payment
    void checkout(ViewOrders &viewOrders)
    {
        if (cartSize == 0)
        {
            cout << "Your cart is empty! Please add products first." << endl;
            return;
        }

        double total = 0.0;
        for (int i = 0; i < cartSize; ++i)
        {
            total += cart[i]->getPrice() * qty[i];
        }

        string ans = getValidAns("Do you want to checkout all the products? [Y|N]: ", [](const string &input)
                                 { return input == "Y" || input == "N"; });
        if (ans == "Y")
        {
            interface();
            cout << "\n========================================================\n";
            cout << "                      PAYMENT METHOD\n";
            cout << "========================================================\n";
            cout << "1 - Cash\n";
            cout << "2 - Credit / Debit Card\n";
            cout << "3 - GCash\n";
            cout << "========================================================\n";
            int method = getValidInt("Choose payment method [1|2|3]: ", 1, 3);

            Payment *paymentMethod = nullptr;

            switch (method)
            {
            case 1:
                paymentMethod = new Cash();
                break;
            case 2:
                paymentMethod = new Card();
                break;
            case 3:
                paymentMethod = new GCash();
                break;
            default:
                cout << "Invalid payment method!" << endl;
                return;
            }

            // Static order ID for logging
            static int orderID = 0;
            orderID++;
            // Process payment using the singleton instance
            PaymentProcess::getInstance()->pay(paymentMethod, total, orderID);

            // Log after successful payment
            logOrder(orderID, total, paymentMethod->getMethodName());

            // Store order details in ViewOrders
            viewOrders.setOrder(orderID, paymentMethod->getMethodName(), cart, qty, cartSize, total);

            delete paymentMethod; // Clean up payment method object

            cout << "You have successfully checked out the products!\n";
            clearCart(); // Clear cart after successful checkout
        }
        else
        {
            cout << "Returning to menu..." << endl;
        }
    }
};

// Order class
class Order : public Interface
{
private:
    Product *products[5];

public:
    // Constructor to initialize products
    Order(Product *products[])
    {
        for (int i = 0; i < 5; ++i)
        {
            this->products[i] = products[i];
        }
    }

    // Method to display available products
    void interface() override
    {
        cout << "=========================================================\n";
        cout << "                  AVAILABLE PRODUCTS\n";
        cout << "=========================================================\n";
        cout << left << setw(12) << "Product ID" << "| " << setw(15) << "Name" << "| " << setw(12) << "Price" << endl;
        cout << string(57, '-') << endl;
        for (int i = 0; i < 5; ++i)
        {
            cout << left << setw(12) << products[i]->getID() << "| " << setw(15) << products[i]->getName()
                 << "| Php " << fixed << setprecision(2) << products[i]->getPrice() << endl;
        }
        cout << "========================================================\n";
    }

    // Method to choose a product and add it to the cart
    void chooseProduct(ShoppingCart &cart)
    {
        string ans;
        do
        {
            interface();
            string id = getValidID("Enter the ID of the product you want to add to the cart: ", [](const string &input)
                                   { return input.length() == 3; });
            Product *selected = nullptr;

            for (int i = 0; i < 5; ++i)
            {
                if (products[i]->getID() == id)
                {
                    selected = products[i];
                    break;
                }
            }

            if (selected != nullptr)
            {
                int qty = getValidInt("Enter quantity [1-5]: ", 1, 5);
                cart.addToCart(selected, qty);
                cout << "Product added successfully!\n\n";
            }
            else
            {
                cout << "Error: Product not found!" << endl;
            }

            ans = getValidAns("Do you want to add another product? [Y|N]: ", [](const string &input)
                              { return input == "Y" || input == "N"; });

        } while (ans == "Y");
        cout << "Returning to menu...\n\n";
    }
};

// Main function
int main()
{
    const int productCount = 5; // Number of products
    // Array of products
    Product *products[productCount] = {
        new Product("ABC", "T-Shirt", 499.99),
        new Product("CDE", "Pants", 799.99),
        new Product("EFG", "Shoes", 1499.99),
        new Product("GHI", "Windbreaker", 1299.99),
        new Product("IJK", "Socks", 199.99)};

    ShoppingCart cart;     // Shopping cart object
    Order order(products); // Order object
    ViewOrders viewOrders; // ViewOrders object

    int menu;
    do
    {
        try
        { // Display menu and get user choice
            menu = getValidInt(
                "=========================================================\n"
                "         Welcome to Jane's Online Store!\n"
                "=========================================================\n"
                "Menu\n"
                "1 - View Products\n"
                "2 - View Shopping Cart\n"
                "3 - View Orders\n"
                "4 - Exit\n"
                "---------------------------------------------------------\n"
                "Choose Menu [1|2|3|4]: ",
                1, 4);

            // Handle user choice
            switch (menu)
            {
            case 1:
                order.chooseProduct(cart);
                break;
            case 2:
                cart.interface();
                cart.checkout(viewOrders);
                break;
            case 3:
                viewOrders.interface();
                break;
            case 4:
                cout << "Exiting... Thank you for visiting!" << endl;
                break;
            default:
                throw out_of_range("Invalid option! Please choose only from 1-4.");
            }
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    } while (menu != 4);

    // Clean up dynamically allocated products
    for (int i = 0; i < productCount; i++)
    {
        delete products[i];
    }

    return 0;
}