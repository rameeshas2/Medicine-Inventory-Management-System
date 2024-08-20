#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib> // For system("cls")
using namespace std;

// Forward declaration
class SalesManagement;
void billing(SalesManagement &salesManager);
void viewSalesRecords();
void getMedicineData();

class UserLogin{
    string userName;
    string password;

public:
    void getUserInfo(){
        cout << "\nEnter your Username: " << endl;
        cin >> userName;
        cout << "\nChoose a password for your system: " << endl;
        cin >> password;
        cout << "Password set: " << password << endl;
    }

    UserLogin() {}
    ~UserLogin() {}

    string getUserName(){
        return userName;
    }

    string getPassword(){
        return password;
    }

    int verifyPassword(){
        string x;
        cout << "\nEnter the password: ";
        cin >> x;

        return x == password;
    }

    void passwordProtect(){
        const int maxAttempts = 3;
        int attempts = 0;

        while (attempts < maxAttempts){
            if (verifyPassword()){
                cout << "\nAccess Granted!\nWelcome User: " << getUserName() << endl;
                return; // Exit the function if the password is correct
            }
            else{
                cout << "Incorrect password! Please try again." << endl;
                attempts++;
            }
        }

        // If the loop completes without successful login, exit the program
        cout << "Maximum login attempts reached. Exiting the program." << endl;
        exit(0);
    }
};

// Medicine class definition
class Medicine{
protected:
    string id;
    string name;
    double price;
    int quantity;
    string type;
    string department;
    string supplier;
    int reorderPoint;

public:
    // Pure virtual functions
    virtual void addMed() = 0;
    virtual void deleteMed() = 0;
    virtual void updateMedQuantity() = 0;
    virtual void viewMed() = 0;
    virtual void searchMed() = 0;
    virtual void placeOrder() = 0;
    virtual void processOrders() = 0;
    virtual ~Medicine() {} // Virtual destructor
};

// MedicineInventory class definition
class MedicineInventory : public Medicine{
public:
    void addMed();
    void deleteMed();
    void updateMedQuantity();
    void viewMed();
    void searchMed();
    void placeOrder();
    int getAvailableQuantity(string &id);
    void processOrders();
    void decreaseAvailableQuantity(string &id, int quantity);
    void checkReorderPoint(string &id);
};

void MedicineInventory::addMed(){
    // Medicine m;
    cout << "Enter ID of medicine: ";
    cin >> id;
    cout << "Enter name of medicine: ";
    cin >> name;
    cout << "Enter price of medicine: ";
    cin >> price;
    cout << "Enter quantity of medicine: ";
    cin >> quantity;
    cout << "Enter type of medicine: ";
    cin >> type;
    cout << "Enter department of medicine: ";
    cin >> department;
    cout << "Enter supplier of medicine: ";
    cin >> supplier;
    cout << "Enter reorder point of medicine: ";
    cin >> reorderPoint;
    system("cls");

    fstream fileObj;
    fileObj.open("medicine.txt", ios::app);
    fileObj.seekp(0, ios::end); // Seek to the end of the file

    // id-name-price-quantity-type-dep-supp-reorder
     fileObj << id << endl << name << endl << price << endl << quantity << endl << type << endl << department << endl << supplier << endl << reorderPoint << endl;
    // Write medicine information to the file
    fileObj.close();

    cout << "\n\t\tNew Medicine information added successfully!" << endl<< endl<< endl;
    cout << "\n\n\n\nEnter 1 to return to Medicine Info\nEnter 2 to Exit the program." << endl;
    int c;
    cin >> c;
    if (c == 1){
        system("cls");
        getMedicineData();
    }
    else
        exit(0);
}

void MedicineInventory::deleteMed() {
    string x;
    cout << "\nEnter the ID of the Medicine you want to Delete:" << endl;
    cin >> x;

    fstream fileObj("medicine.txt", ios::in);
    if (!fileObj) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    bool found = false;
    string id, name, type, department, supplier;
    double price;
    int quantity, reorderPoint;

    //First check if medicine exists
    while (fileObj >> id >> name >> price >> quantity >> type >> department >> supplier >> reorderPoint) {
        if (id == x) {
            cout << "\nMedicine found:\n";
            cout << "ID: " << id << endl;
            cout << "Name: " << name << endl;
            cout << "Price: " << price << endl;
            cout << "Quantity: " << quantity << endl;
            cout << "Type: " << type << endl;
            cout << "Department: " << department << endl;
            cout << "Supplier: " << supplier << endl;
            cout << "Reorder Point: " << reorderPoint << endl;
            found = true;
            break;
        }
    }
    fileObj.clear();  // Clear the eof flag
    fileObj.seekg(0, ios::beg);  // Move the read pointer back to the beginning of the file

    if (!found) {
        cout << "\nNo matching records found!" << endl;
        fileObj.close();
        return;
    }

    cout << "Are you sure you want to delete this profile?\n1.YES\n2.NO" << endl;
    int n;
    cin >> n;
    if (n != 1) {
        fileObj.close();
        return;
    }

    // Create a temporary file to write all records except the one to be deleted
    ofstream tempObj("temp.txt", ios::out);
    if (!tempObj) {
        cout << "Error: Unable to open temp file." << endl;
        fileObj.close();
        return;
    }

    // Copy all entries except the one to be deleted to the temp file
    while (fileObj >> id >> name >> price >> quantity >> type >> department >> supplier >> reorderPoint) {
        if (id != x) {
            tempObj << id << endl << name << endl << price << endl << quantity << endl
                    << type << endl << department << endl << supplier << endl << reorderPoint << endl;
        }
    }

    fileObj.close();
    tempObj.close();

    // Replace the original file with the temp file
    remove("medicine.txt");
    rename("temp.txt", "medicine.txt");

    system("cls");
    cout << "\n\t\tMedicine successfully deleted" << endl;
}

void MedicineInventory::updateMedQuantity(){
    string searchID;
    cout << "Enter the ID of the medicine you want to update: ";
    cin >> searchID;

    ifstream inputFile("medicine.txt");
    if (!inputFile){
        cout << "Error: Unable to open file." << endl;
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile){
        cout << "Error: Unable to open file." << endl;
        return;
    }

    string id, name, type, department, supplier;
    double price;
    int quantity, reorderPoint;
    bool found = false;

    while (inputFile >> id >> name >> price >> quantity >> type >> department >> supplier >> reorderPoint) {
        if (id == searchID) {
            cout << "\nMedicine found:\n";
            cout << "ID: " << id << endl;
            cout << "Name: " << name << endl;
            cout << "Price: " << price << endl;
            cout << "Current Quantity: " << quantity << endl;

            int newQuantity;
            cout << "Enter new quantity: ";
            cin >> newQuantity;

            tempFile << id << endl << name << endl << price << endl << newQuantity << endl
                    << type << endl << department << endl << supplier << endl << reorderPoint << endl;

            found = true;
            cout << "Medicine quantity with Id " << id << " is updated\nNew quantity is : " << newQuantity << endl;
        } else {
            tempFile << id << endl << name << endl << price << endl << quantity << endl
                    << type << endl << department << endl << supplier << endl << reorderPoint << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    // Replace the original file with the temp file
    remove("medicine.txt"); //cstudio
    rename("temp.txt", "medicine.txt");

    if (!found){
        cout << "Medicine with ID " << searchID << " not found." << endl;
    }

    int choice;
    cout << "\n\nEnter 1 to return to Medicine Inventory\nEnter 2 to Exit." << endl;
    cin >> choice;

    switch (choice){
    case 1:
        system("cls");
        getMedicineData();
        break;
    case 2:
        exit(0);
        break;
    default:
        cout << "Invalid choice. Exiting the program" << endl;
        exit(0);
        break;
    }
}

void MedicineInventory::viewMed(){
    ifstream fileObj("medicine.txt");

    if (!fileObj.is_open()){ //iostream
        cout << "Error: Unable to open file." << endl;
        return;
    }

    string id, name, type, department, supplier;
    double price;
    int quantity, reorderPoint;
    // id-name-price-quantity-type-dep-supp-reorder

    cout << "\n\t\t MEDICINE INVENTORY\n";
    cout << "---------------------------------------------\n";

    while (fileObj >> id >> name >> price >> quantity >> type >> department >> supplier >> reorderPoint){
        cout << "\n1. ID: " << id << endl << "2. Name: " << name << endl << "3. Price: " << price << endl << "4. Quantity: " << quantity 
        << endl <<"5. Type: "<< type << endl<<"6. Department: "<< department << endl<<"7. Supplier"<< supplier<<endl<<"8. Re-Order Point: " <<reorderPoint<<endl << endl;
    }

    fileObj.close();

    cout << "\n\nEnter 1 to return to Medicine Inventory\nEnter 2 to Exit." << endl;
    int c;
    cin >> c;
    if (c == 1)
    {
        system("cls");
        getMedicineData();
    }
    else
        exit(0);
}

void MedicineInventory::searchMed(){
    string searchID;
    cout << "Enter the ID of the medicine you want to search: ";
    cin >> searchID;

    fstream fileObj;
    fileObj.open("medicine.txt");

    if (!fileObj){
        cout << "Error: Unable to open file." << endl;
        return;
    }

    string id, name, type, department, supplier;
    double price;
    int quantity, reorderPoint;
    bool found = false;

    while (fileObj >> id >> name >> price >> quantity >> type >> department >> supplier >> reorderPoint) {
        if (id == searchID) {
            cout << "\nMedicine found:\n";
            cout << "ID: " << id << endl;
            cout << "Name: " << name << endl;
            cout << "Price: " << price << endl;
            cout << "Quantity: " << quantity << endl;
            cout << "Type: " << type << endl;
            cout << "Department: " << department << endl;
            cout << "Supplier: " << supplier << endl;
            cout << "Reorder Point: " << reorderPoint << endl;
            found = true;
            break;
        }
    }


    fileObj.close();

    if (!found){
        cout << "Medicine with ID " << searchID << " not found." << endl;
    }

    cout << "\n\n\n\nEnter 1 to return to Medicine Info\nEnter 2 to Exit the program." << endl;
    int c;
    cin >> c;
    if (c == 1){
        system("cls");
        getMedicineData();
    }
    else
        exit(0);
}

void MedicineInventory::placeOrder(){
    string id, supplier;
    int quantity;
    cout << "Enter the ID of the medicine you want to order: ";
    cin >> id;
    cout << "Enter the supplier: ";
    cin >> supplier;

    int availableQuantity = getAvailableQuantity(id);
    if (availableQuantity == -1){
        cout << "Error: Medicine with ID " << id << " not found in inventory." << endl;
        return;
    }

    cout << "Enter quantity: ";
    cin >> quantity;

    // Check if the quantity being ordered exceeds the available quantity
    if (quantity > availableQuantity){
        cout << "Error: Quantity exceeds available stock (" << availableQuantity << ")." << endl;
        return;
    }

    ofstream orderFile("orders.txt", ios::app);
    if (orderFile.is_open()){
        orderFile << id << endl << quantity << endl;
        orderFile.close();
        cout << "Order placed successfully!" << endl;
    }
    else{
        cout << "Error: Unable to place order." << endl;
    }
}

// Helper function to get the available quantity for a given medicine ID
int MedicineInventory::getAvailableQuantity(string &id){
    ifstream fileObj("medicine.txt");
    if (!fileObj.is_open()){
        cout << "Error: Unable to open file." << endl;
        return -1;
    }

    // id-name-price-quantity-type-dep-supp-reorder
    string medID, name, type, department, supplier;
    double price;
    int reorderPoint;
    int availableQuantity;
    while (fileObj >> medID >> name >> price >> availableQuantity >>type >>department>> supplier>>reorderPoint){
        if (medID == id){
            fileObj.close();
            return availableQuantity;
        }
    }

    fileObj.close();
    return -1; // Medicine with the specified ID not found
}

void MedicineInventory::decreaseAvailableQuantity(string &id, int quantity){
    ifstream inputFile("medicine.txt");
    if (!inputFile){
        cout << "Error: Unable to open file." << endl;
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile){
        cout << "Error: Unable to open file." << endl;
        return;
    }
    // id-name-price-quantity-type-dep-supp-reorder

    string medID, name, type, department, supplier;
    double price;
    int reorderPoint;
    int availableQuantity;
    bool found = false;

    // Search for the medicine by ID and update its quantity
    while (inputFile >> medID >> name >> price >> availableQuantity >>type >>department>> supplier>>reorderPoint){
        if (medID == id){
            tempFile << medID << endl << name << endl << price << endl << availableQuantity - quantity << endl << type << endl 
            << department << endl <<supplier << endl <<reorderPoint<< endl;
            found = true;
        }
        else{
            tempFile << medID << endl << name << endl << price << endl << availableQuantity << endl << type << endl 
            << department << endl <<supplier << endl <<reorderPoint<< endl;
        }
    }

    inputFile.close();
    tempFile.close();

    // Replace the original file with the temp file
    remove("medicine.txt");
    rename("temp.txt", "medicine.txt");

    if (!found){
        cout << "Medicine with ID " << id << " not found." << endl;
    }
}

// SalesManagement class definition
class SalesManagement{
public:
    int totalQuantitySold;
    double totalSales;
    // Member functions
    SalesManagement() : totalQuantitySold(0), totalSales(0.0) {}

    void addSaleRecord(int quantity, double totalPrice);
        
    double getTotalSales() {
        ifstream salesFile("sales_records.txt");
        if (!salesFile.is_open()) {
            cout << "Error: Unable to open sales records file." << endl;
            return 0.0;
        }

        double totalSales = 0.0;
        double price;
        while (salesFile >> price) {
            totalSales += price;
        }
        salesFile.close();

        return totalSales;
    }

    void addSalesRecord(string &id, int quantity, double price){
        ofstream salesFile("sales.txt", ios::app);
        if (!salesFile){
            cout << "Error: Unable to open sales file." << endl;
            return;
        }

        salesFile << id << endl << quantity << endl << price << endl;
        salesFile.close();

            // Update total sales
        totalSales += price * quantity;
    }

};

void SalesManagement::addSaleRecord(int quantity, double totalPrice){
    ofstream salesFile("sales_records.txt", ios::app);
    if (salesFile){
        salesFile << "Quantity: " << quantity << endl << "Total Price: " << totalPrice << endl;
        cout << "Sale record added successfully!" << endl;
    }
    else{
        cout << "Error: Unable to add sale record." << endl;
    }
    salesFile.close();
}

void MedicineInventory::processOrders(){
    ifstream orderFile("orders.txt");
    if (!orderFile){
        cout << "Error: Unable to open orders file." << endl;
        return;
    }

    SalesManagement salesManager;
    string id;
    int orderQuantity;

    // Process each order from the file
    while (orderFile >> id >> orderQuantity){
        int availableQuantity = getAvailableQuantity(id);
        if (availableQuantity >= orderQuantity){
            decreaseAvailableQuantity(id, orderQuantity);

            // Get the price of the medicine
            double price = 0.0;
            ifstream fileObj("medicine.txt");
            if (fileObj.is_open()){
                string medId, medName, department, supplier, type;
                double medPrice;
                int medQuantity , reorderPoint;
                while (fileObj >> medId >> medName >> medPrice >> medQuantity >>type >>department>> supplier>>reorderPoint){
                    if (medId == id){
                        price = medPrice;
                        break;
                    }
                }
                fileObj.close();
            }

            salesManager.addSaleRecord( orderQuantity, price * orderQuantity);
            cout << "Order processed for Medicine ID: " << id << " Quantity: " << orderQuantity << endl;
        }
        else{
            cout << "Insufficient quantity for Medicine ID: " << id << endl;
        }
    }

    orderFile.close();
}

// OrderManager class definition
class OrderManager{
public:
    // Member functions
    void placeOrder(Medicine &medicine);
    void processOrders(Medicine &medicine);
};

void OrderManager::placeOrder(Medicine &medicine){
    medicine.placeOrder();
}

void OrderManager::processOrders(Medicine &medicine){
    medicine.processOrders();
}

class Billing : public Medicine{
private:
    Medicine &med;

public:
    double total;
    string medicineId;
    int quantity;
    // Member functions
    Billing(Medicine &medicine) : med(medicine) {}
    void generateBill();
    void calculateTotal(double price);
    void displayBill();

    void addMed() {}
    void deleteMed() {}
    void updateMedQuantity() {}
    void viewMed() {}
    void searchMed() {}

    void placeOrder(){
        med.placeOrder();
    }
    void processOrders(){
        med.placeOrder();
    }
};

void Billing::generateBill(){
    cout << "Enter medicine ID: ";
    cin >> medicineId;
    cout << "Enter quantity: ";
    cin >> quantity;

    ifstream fileObj("medicine.txt");

    if (!fileObj.is_open()){
        cout << "Error: Unable to open file." << endl;
        return;
    }

    bool found = false;

    // Check if the entered medicine ID exists in the inventory and if the requested quantity is available
    string id, name;
    double price;
    int availableQuantity;
    while (fileObj >> id >> name >> price >> availableQuantity){
        if (id == medicineId){
            if (quantity > availableQuantity){
                cout << "Error: Requested quantity exceeds available stock." << endl;
                fileObj.close();
                return;
            }
            found = true;
            break;
        }
    }
    fileObj.close();

    if (!found){
        cout << "Error: Medicine with ID " << medicineId << " not found." << endl;
        return;
    }
}

void Billing::calculateTotal(double price){
    total = quantity * price;
    cout << "Total amount: Rs. " << total << endl;
}

void Billing::displayBill(){
    cout << "\nBilling Details:" << endl;
    cout << "Medicine ID: " << medicineId << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Total Amount: Rs." << fixed << setprecision(2) << total << endl;
}

void billing(SalesManagement &salesManager){
    MedicineInventory medInventory;
    Billing bill(medInventory);
    string id, name, type, department, supplier;
    double price;
    int quantity, reorderPoint;

    cout << "Enter the details for billing:" << endl;
    bill.generateBill();

    ifstream fileObj("medicine.txt");

    if (!fileObj.is_open()){
        cout << "Error: Unable to open file." << endl;
        return;
    }

    bool found = false;

    while (fileObj >> id >> name >> price >> quantity>>type >>department>> supplier>>reorderPoint){
        if (id == bill.medicineId){
            if (quantity < bill.quantity){
                cout << "Error: Quantity not available in the inventory." << endl;
                return;
            }

            bill.calculateTotal(price);
            bill.displayBill();
            salesManager.addSaleRecord( bill.quantity, bill.total);
            // salesManager.viewSalesSummary();
            found = true;
            break;
        }
    }

    fileObj.close();

    if (!found){
        cout << "Medicine with ID " << bill.medicineId << " not found." << endl;
    }

    cout << "\n\nEnter 1 to return to Medicine Info\nEnter 2 to Exit the program." << endl;
    int c;
    cin >> c;
    if (c == 1){
        system("cls");
        getMedicineData();
    }
    else{
        exit(0);
    }
}

int main(){
    UserLogin u1;
    u1.getUserInfo();
    u1.passwordProtect();
    system("cls");

    SalesManagement *salesManager = new SalesManagement();
    MedicineInventory *medicineInventory = new MedicineInventory();
    OrderManager *orderManager = new OrderManager();

    int choice;

    system("cls");
    do{
        cout << "\n                         MAIN MENU" << endl;
        cout << "			1. Medicine Management" << endl;
        cout << "			2. Order Management" << endl;
        cout << "			3. Billing" << endl;
        cout << "			4. Exit" << endl;
        cout << "Enter the number corresponding to your choice: ";
        cin >> choice;

        system("cls");

        switch (choice){
        case 1:
            // Medicine management options
            system("cls");
            getMedicineData();
            break;
        case 2:
            // Order management options
            int orderChoice;
            cout << "\n                         Order Management" << endl;
            cout << "			1. Place Order" << endl;
            cout << "			2. Process Orders" << endl;
            cout << "\t                3. View Sales Records" << endl; // Added option to view sales records
            cout << "Enter the number corresponding to your choice: ";
            cin >> orderChoice;
            system("cls");
            switch (orderChoice){
            case 1:
                system("cls");
                orderManager->placeOrder(*medicineInventory);
                break;
            case 2:
                system("cls");
                orderManager->processOrders(*medicineInventory);
                break;
            case 3:
                system("cls");
                viewSalesRecords(); // Call viewSalesRecords function
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
            }
            break;
        case 3:
            // Billing functionality
            system("cls");
            billing(*salesManager); // Pass the SalesManager object
            break;
        case 4:
            cout << "Exiting the program" << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
        }
    } while (choice != 4);

    delete salesManager;
    delete medicineInventory;
    delete orderManager;

    return 0;
}

void getMedicineData(){
    SalesManagement *salesManager = new SalesManagement();
    MedicineInventory *medInv = new MedicineInventory();

    int choice_med;
    cout << "\n                         WELCOME TO MEDICINE INVENTORY" << endl
         << endl;
    cout << "            1. Add Medicine" << endl;
    cout << "            2. Delete Medicine" << endl;
    cout << "            3. Update Medicine Quantity" << endl;
    cout << "            4. View Medicine Inventory" << endl;
    cout << "            5. Search Medicine" << endl;
    cout << "            6. Billing" << endl;
    cout << "            7. Return to Main Menu" << endl;
    cout << "            8. Exit" << endl; // Adjusted numbering for consistency
    cout << right << setw(80) << setfill('-') << "----------" << endl;

    cout << right << setw(60) << setfill(' ') << "\nEnter the number corresponding to your choice:" << endl
         << endl;
    cin >> choice_med;
    system("cls");

    switch (choice_med){
    case 1:
        medInv->addMed();
        break;
    case 2:
        medInv->deleteMed();
        break;
    case 3:
        medInv->updateMedQuantity();
        break;
    case 4:
        medInv->viewMed();
        break;
    case 5:
        medInv->searchMed();
        break;
    case 6:
        billing(*salesManager);
        break;
    case 7:
        // Return to Main Menu
        break;
    case 8:
        exit(0);
        break;
    default:
        cout << "Invalid choice!" << endl;
        break;
    }

    delete salesManager;
    delete medInv;
}

// Function to view sales records
void viewSalesRecords() {
    ifstream salesFile("sales.txt");
    if (!salesFile) {
        cout << "Error: Unable to open sales file." << endl;
        return;
    }

    cout << "\n\t\t SALES RECORDS\n";
    cout << "---------------------------------------------\n";

    string id;
    int quantity;
    double price;

    while (salesFile >> id >> quantity >> price) {
        cout << "Medicine ID: " << id << endl;
        cout << "Quantity Sold: " << quantity << endl;
        cout << "Total Price: Rs. " << fixed << setprecision(2) << price << endl;
        cout << "---------------------------------------------\n";
    }

    salesFile.close();
}
