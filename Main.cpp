#include <iostream>
#include <map>
#include <string>
using namespace std;

class Product {
private:
    double price;
    string name;
    string unit;

public:
    Product() : price(0), name(""), unit("") {};
    Product(string name, string unit, double price) : name(name), unit(unit), price(price) {}

    string getName() const {
        return name;
    }

    string getUnit() const {
        return unit;
    }

    double getPrice() const {
        return price;
    }
};

class Store {
private:
    map<string, Product> products;

public:
    void addProduct(string name, string unit, double price) {
        if (products.find(name) == products.end()) {
            products[name] = Product(name, unit, price);
        } else {
            cout << "Product already exists.\nReplace existing product? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                products[name] = Product(name, unit, price);
            }
        }
    }

    Product* getProduct(const string& name) {
        auto it = products.find(name);
        if (it != products.end()) {
            return &(it->second);
        } else {
            cout << "No product exists with the given name.\n";
            return nullptr;
        }
    }
};

class Bill {
private:
    double total_amount = 0;
    map<string, pair<Product*, int>> products;
    int discount_percentage = 0;

public:
    void addProduct(Product* product, int quantity) {
        if (product == nullptr) {
            cout << "Cannot add null product to bill.\n";
            return;
        }

        auto it = products.find(product->getName());
        if (it != products.end()) {
            cout << "Product already exists in bill.\nReplace existing quantity? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                it->second.second = quantity;
            }
        } else {
            products[product->getName()] = make_pair(product, quantity);
        }

        total_amount = 0;
        for (const auto& item : products) {
            total_amount += item.second.first->getPrice() * item.second.second;
        }
    }

    void setDiscount() {
        if (total_amount >1000) {
            discount_percentage = 10;
        }
        else if (discount_percentage > 100) {
            discount_percentage = 5;
        }
        else {
            discount_percentage = 1;
        }
    }

    double get_final_amount() const {
        return total_amount - (discount_percentage * total_amount / 100.0);
    }

    void display() const {
        cout << "Bill:\n";
        for (const auto& it : products) {
            cout << "Product: " << it.first
                 << "\tQuantity: " << it.second.second
                 << "\tPrice: " << it.second.first->getPrice()
                 << " * " << it.second.second
                 << " = " << it.second.second * it.second.first->getPrice() << endl;
        }
        cout << "Total amount: " << total_amount << endl;
        cout << "Discount percentage: " << discount_percentage << "%" << endl;
        cout << "Total amount payable: " << get_final_amount() << endl;
    }
};

void welcome(Bill& bill, Store& store) {
    cout << "Welcome to KPR Store\n";
    cout << "Please enter the number of products: ";
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Enter the name of the product: ";
        string name;
        cin >> name;

        Product* product = store.getProduct(name);
        if (product == nullptr) {
            cout << "Product not found. Skipping...\n";
            continue;
        }

        cout << "Enter the quantity required: ";
        int quantity;
        cin >> quantity;

        bill.addProduct(product, quantity);
    }
    bill.setDiscount();

    bill.display();
}

int main() {
    Store store;
    Bill bill;

    store.addProduct("Rice", "kg", 100);
    store.addProduct("Sugar", "kg", 50);
    store.addProduct("Biscuit", "packet", 10);
    store.addProduct("Milk", "litre", 70);

    welcome(bill, store);

    return 0;
}
