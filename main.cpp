#include <iostream>
#include <string>
#include <sstream>
#include "IDisplayable.h"
#include "ISearchable.h"
#include "IRatable.h"
#include "ITransactable.h"
using namespace std;

class Listing;
class Seller;
class Buyer;

class User : public IDisplayable {
protected:
    const int  userID;
    string     name;
    string     email;
    string     phone;
    string     password;
    static int totalUsers;

public:
    User() : userID(0) {
        totalUsers++;
    }

    User(int id, string n, string e, string ph, string pass)
        : userID(id), name(n), email(e), phone(ph), password(pass) {
        totalUsers++;
    }

    User(const User& u)
        : userID(u.userID), name(u.name),
          email(u.email), phone(u.phone),
          password(u.password) {}

    virtual void display() const = 0;

    int    getID()   const { return userID; }
    string getName() const { return name;   }

    bool operator==(const User& other) const {
        return userID == other.userID;
    }

    bool operator!=(const User& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& os, const User& u) {
        os << "[User #" << u.userID << " | " << u.name
           << " | " << u.email << "]";
        return os;
    }

    friend istream& operator>>(istream& is, User& u) {
        cout << "Enter name: ";   is >> u.name;
        cout << "Enter email: ";  is >> u.email;
        cout << "Enter phone: ";  is >> u.phone;
        return is;
    }

    static int getTotalUsers() { return totalUsers; }

    virtual ~User() {}
};
int User::totalUsers = 0;

class Vehicle : public IDisplayable,
                public ISearchable,
                public ITransactable {
protected:
    string brand;
    string model;
    int    year;
    float  price;
    int    mileage;
    bool   available;

public:
    Vehicle() : year(0), price(0), mileage(0), available(true) {}

    Vehicle(string b, string m, int y, float p, int mi)
        : brand(b), model(m), year(y),
          price(p), mileage(mi), available(true) {}

    virtual void display() const = 0;

    virtual bool matchesSearch(const string& keyword) const override {
        return (brand == keyword || model == keyword);
    }

    float  getPrice()     const override { return price; }
    bool   isAvailable()  const override { return available; }
    string getTitle()     const override {
        return brand + " " + model + " " + to_string(year);
    }

    void markSold() { available = false; }

    string getBrand()   const { return brand;   }
    string getModel()   const { return model;   }
    int    getYear()    const { return year;    }
    int    getMileage() const { return mileage; }

    bool operator==(const Vehicle& other) const {
        return (brand   == other.brand  &&
                model   == other.model  &&
                year    == other.year);
    }

    bool operator<(const Vehicle& other) const {
        return price < other.price;
    }

    bool operator>(const Vehicle& other) const {
        return price > other.price;
    }

    float operator+(const Vehicle& other) const {
        return price + other.price;
    }

    friend ostream& operator<<(ostream& os, const Vehicle& v) {
        os << v.brand << " " << v.model
           << " (" << v.year << ") - PKR " << v.price
           << " | " << v.mileage << " km";
        return os;
    }

    virtual ~Vehicle() {}
};

class Car : public Vehicle {
private:
    int    doors;
    string fuelType;
    bool   automatic;
    string color;
    string transmission;

public:
    Car() : Vehicle(), doors(4), automatic(true) {}

    Car(string b, string m, int y, float p, int mi,
        int d, string f, bool a, string c, string t)
        : Vehicle(b, m, y, p, mi),
          doors(d), fuelType(f),
          automatic(a), color(c), transmission(t) {}

    void display() const override {
        cout << "  [CAR]  " << brand << " " << model
             << " | Year: "  << year
             << " | Price: PKR " << price
             << " | Km: "    << mileage
             << " | "        << color
             << " | "        << fuelType
             << " | "        << transmission
             << " | Doors: " << doors << endl;
    }

    void displayDetails() const {
        display();
    }
    void displayDetails(bool showPrice) const {
        if (showPrice)
            cout << "  Price: PKR " << price << endl;
        else
            cout << "  " << brand << " " << model << " (price hidden)" << endl;
    }
    void displayDetails(string format) const {
        if (format == "short")
            cout << "  " << brand << " " << model << " " << year << endl;
        else
            display();
    }
};

class Bike : public Vehicle {
private:
    string bikeType;
    int    engineCC;
    bool   hasGear;
    string color;

public:
    Bike() : Vehicle(), engineCC(0), hasGear(true) {}

    Bike(string b, string m, int y, float p, int mi,
         string bt, int cc, bool gear, string c)
        : Vehicle(b, m, y, p, mi),
          bikeType(bt), engineCC(cc),
          hasGear(gear), color(c) {}

    void display() const override {
        cout << "  [BIKE] " << brand << " " << model
             << " | Year: "     << year
             << " | Price: PKR "<< price
             << " | Km: "       << mileage
             << " | "           << color
             << " | "           << bikeType
             << " | "           << engineCC << "cc"
             << " | Geared: "   << (hasGear ? "Yes" : "No") << endl;
    }

    void displayDetails() const { display(); }
    void displayDetails(bool showPrice) const {
        if (showPrice)
            cout << "  Price: PKR " << price << endl;
        else
            cout << "  " << brand << " " << model << " (price hidden)" << endl;
    }
    void displayDetails(const string& format) const {
        if (format == "short")
            cout << "  " << brand << " " << model << " " << year << endl;
        else
            display();
    }
};

class Listing : public IDisplayable,
                public ISearchable,
                public ITransactable {
private:
    const int  listingID;
    Vehicle*   vehicle;
    string     status;
    float      askingPrice;
    static int totalListings;

    friend class Admin;
    friend bool compareListings(const Listing& a, const Listing& b);

public:
    Listing(int id, Vehicle* v, float asking = 0)
        : listingID(id), vehicle(v),
          status("Pending"),
          askingPrice(asking > 0 ? asking : v->getPrice()) {
        totalListings++;
    }

    void approve() { status = "Approved"; }
    void remove()  { status = "Removed";  }

    void display() const override {
        cout << "  Listing #" << listingID
             << " [" << status << "]"
             << " | Asking: PKR " << askingPrice << endl;
        if (vehicle)
            vehicle->display();
        cout << "  -----------------------------------------" << endl;
    }

    bool matchesSearch(const string& keyword) const override {
        return vehicle ? vehicle->matchesSearch(keyword) : false;
    }

    float  getPrice()    const override { return askingPrice; }
    string getTitle()    const override {
        return vehicle ? vehicle->getTitle() : "Unknown";
    }
    bool   isAvailable() const override { return status == "Approved"; }

    bool operator==(const Listing& other) const {
        return listingID == other.listingID;
    }

    bool operator<(const Listing& other) const {
        return askingPrice < other.askingPrice;
    }

    friend ostream& operator<<(ostream& os, const Listing& l) {
        os << "Listing #" << l.listingID
           << " [" << l.status << "] PKR " << l.askingPrice;
        return os;
    }

    int    getID()        const { return listingID;   }
    string getStatus()    const { return status;      }
    string getBrand()     const {
        return vehicle ? vehicle->getBrand() : "";
    }
    Vehicle* getVehicle() const { return vehicle; }

    static int getTotalListings() { return totalListings; }

    ~Listing() {}
};
int Listing::totalListings = 0;

bool compareListings(const Listing& a, const Listing& b) {
    return a.askingPrice < b.askingPrice;
}

class Buyer : public User {
private:
    string   address;
    int      favCount;
    int      favorites[10];
    float    budget;

    friend class Admin;

public:
    Buyer(int id, string n, string e, string ph,
          string pass, string addr, float bud = 0)
        : User(id, n, e, ph, pass),
          address(addr), favCount(0), budget(bud) {}

    void display() const override {
        cout << "  [BUYER]  " << name
             << " | " << email
             << " | " << address << endl;
    }

    void saveFavorite(int listingID) {
        if (favCount < 10)
            favorites[favCount++] = listingID;
        else
            cout << "  Favorites list is full." << endl;
    }

    void viewFavorites() const {
        cout << "  Saved listings: ";
        for (int i = 0; i < favCount; i++)
            cout << favorites[i] << " ";
        cout << endl;
    }

    void sendMessage(string msg) const {
        cout << "  [MSG from " << name << "] " << msg << endl;
    }

    bool canAfford(const ITransactable& item) const {
        return budget >= item.getPrice();
    }

    Buyer& operator+=(int listingID) {
        saveFavorite(listingID);
        return *this;
    }
};

class Seller : public User, public IRatable {
private:
    string companyName;
    int    activeListings;
    float  rating;
    string location;

    friend bool topRatedSeller(const Seller& a, const Seller& b);

public:
    Seller(int id, string n, string e, string ph,
           string pass, string comp, string loc)
        : User(id, n, e, ph, pass),
          companyName(comp), activeListings(0),
          rating(0.0f), location(loc) {}

    void display() const override {
        cout << "  [SELLER] " << name
             << " | " << companyName
             << " | " << location
             << " | Rating: " << rating << endl;
    }

    void addListing()    { activeListings++;  }
    void removeListing() {
        if (activeListings > 0) activeListings--;
    }

    float getrating() const override { return rating; }
    void  updateRating(float r) override {
        if (r >= 0.0f && r <= 5.0f)
            rating = r;
        else
            cout << "  Invalid rating value." << endl;
    }

    bool operator==(const Seller& other) const {
        return (companyName == other.companyName &&
                location    == other.location);
    }

    friend ostream& operator<<(ostream& os, const Seller& s) {
        os << "[Seller: " << s.name
           << " | " << s.companyName
           << " | Rating: " << s.rating << "]";
        return os;
    }
};

bool topRatedSeller(const Seller& a, const Seller& b) {
    return a.rating > b.rating;
}

class Admin : public User {
private:
    const string platformName = "PakWheels Marketplace";
    int    approvedListings;
    int    removedListings;
    string role;

public:
    Admin(int id, string n, string e, string ph, string pass)
        : User(id, n, e, ph, pass),
          approvedListings(0), removedListings(0),
          role("Moderator") {}

    void display() const override {
        cout << "  [ADMIN]  " << name
             << " | Role: " << role
             << " | Platform: " << platformName << endl;
    }

    void approveListing(Listing& l) {
        l.approve();
        approvedListings++;
        cout << "  Admin approved: " << l << endl;
    }

    void removeListing(Listing& l) {
        l.remove();
        removedListings++;
        cout << "  Admin removed: " << l << endl;
    }

    void auditBuyerFavorites(const Buyer& b) const {
        cout << "  [AUDIT] Buyer " << b.name << " favorites: ";
        for (int i = 0; i < b.favCount; i++)
            cout << b.favorites[i] << " ";
        cout << endl;
    }

    void showStats() const {
        cout << "  Approved: " << approvedListings
             << " | Removed: " << removedListings << endl;
    }

    bool operator==(const Admin& other) const {
        return userID == other.userID;
    }
};

class Marketplace {
private:
    User*    users[20];
    Listing* listings[20];
    int      userCount;
    int      listingCount;
    const int MAX = 20;

public:
    Marketplace() : userCount(0), listingCount(0) {}

    void addUser(User* u) {
        if (userCount < MAX)
            users[userCount++] = u;
    }

    void addListing(Listing* l) {
        if (listingCount < MAX)
            listings[listingCount++] = l;
    }

    void searchByKeyword(const string& keyword) const {
        cout << "\n  Search results for \"" << keyword << "\":" << endl;
        bool found = false;
        for (int i = 0; i < listingCount; i++) {
            if (listings[i]->matchesSearch(keyword)) {
                listings[i]->display();
                found = true;
            }
        }
        if (!found)
            cout << "  No results found." << endl;
    }

    void searchByPrice(float minP, float maxP) const {
        cout << "\n  Listings between PKR " << minP
             << " and PKR " << maxP << ":" << endl;
        for (int i = 0; i < listingCount; i++) {
            float p = listings[i]->getPrice();
            if (p >= minP && p <= maxP)
                listings[i]->display();
        }
    }

    void displayAll() const {
        cout << "\n  ===== All Listings =====" << endl;
        for (int i = 0; i < listingCount; i++)
            listings[i]->display();
    }

    void displayUsers() const {
        cout << "\n  ===== Registered Users =====" << endl;
        for (int i = 0; i < userCount; i++)
            users[i]->display();
    }

    Marketplace& operator+=(Listing* l) {
        addListing(l);
        return *this;
    }
};



int main() {

    cout << "============================================" << endl;
    cout << "       PakWheels Car Marketplace v2.0       " << endl;
    cout << "============================================" << endl;

    Marketplace market;

    Buyer  b1(1, "Ali",   "ali@gmail.com",   "0300-1234567", "pass123", "Karachi",   3800000);
    Buyer  b2(4, "Sara",  "sara@gmail.com",  "0312-9876543", "sara456", "Islamabad", 2000000);
    Seller s1(2, "Ahmed", "ahmed@gmail.com", "0311-1112233", "abc123",  "AutoWorld", "Lahore");
    Seller s2(5, "Zara",  "zara@gmail.com",  "0321-4455667", "zara789", "BikeHub",   "Karachi");
    Admin  admin(3, "Usman", "admin@marketplace.pk", "000-0000000", "admin@123");

    market.addUser(&b1);
    market.addUser(&b2);
    market.addUser(&s1);
    market.addUser(&s2);
    market.addUser(&admin);

    Car c1("Toyota", "Corolla", 2020, 3500000, 50000, 4, "Petrol", true,  "White", "Automatic");
    Car c2("Honda",  "Civic",   2019, 3200000, 60000, 4, "Petrol", true,  "Black", "Automatic");
    Car c3("Suzuki", "Cultus",  2021, 1900000, 30000, 4, "Petrol", false, "Silver","Manual");

    Bike bk1("Yamaha", "YBR125",  2022, 290000, 8000,  "Standard", 125, true, "Red");
    Bike bk2("Honda",  "CB150F",  2023, 340000, 5000,  "Sports",   150, true, "Blue");
    Bike bk3("United", "US100",   2021, 120000, 15000, "Standard", 100, true, "Black");

    Listing l1(101, &c1);
    Listing l2(102, &c2);
    Listing l3(103, &c3);
    Listing l4(201, &bk1);
    Listing l5(202, &bk2);
    Listing l6(203, &bk3);

    market += &l1;
    market += &l2;
    market += &l3;
    market += &l4;
    market += &l5;
    market += &l6;

    cout << "\n--- Admin Actions ---" << endl;
    admin.approveListing(l1);
    admin.approveListing(l2);
    admin.approveListing(l4);
    admin.approveListing(l5);
    admin.removeListing(l6);

    cout << "\n--- Buyer Actions ---" << endl;
    b1 += 101;
    b1 += 201;
    b1.viewFavorites();
    b1.sendMessage("Is the Corolla still available?");

    cout << "  Can Ali afford Corolla? "
         << (b1.canAfford(l1) ? "Yes" : "No") << endl;

    cout << "\n--- Seller Actions ---" << endl;
    s1.addListing();
    s1.addListing();
    s1.updateRating(4.5f);
    s2.updateRating(4.0f);
    cout << "  " << s1 << endl;
    cout << "  Top rated? " << (topRatedSeller(s1, s2) ? s1.getName() : s2.getName()) << endl;

    market.displayAll();
    market.displayUsers();

    market.searchByKeyword("Toyota");
    market.searchByKeyword("Yamaha");
    market.searchByPrice(200000, 1500000);

    cout << "\n--- Operator Demos ---" << endl;
    cout << "  b1 == b2? " << (b1 == b2 ? "Same user" : "Different users") << endl;
    cout << "  Corolla == Civic? " << (c1 == c2 ? "Same" : "Different") << endl;
    cout << "  Corolla cheaper than Civic? " << (c1 < c2 ? "Yes" : "No") << endl;
    cout << "  Bundle price (Corolla + YBR): PKR " << (c1 + bk1) << endl;
    cout << "  Listing l1 < l2? " << (l1 < l2 ? "Yes" : "No") << endl;

    cout << "\n--- displayDetails() Overloading ---" << endl;
    c1.displayDetails();
    c1.displayDetails(true);
    c1.displayDetails("short");
    bk1.displayDetails();
    bk1.displayDetails(false);
    bk1.displayDetails("short");

    cout << "\n--- Admin Audit ---" << endl;
    admin.auditBuyerFavorites(b1);
    admin.showStats();

    cout << "\n--- Platform Stats ---" << endl;
    cout << "  Total registered users   : " << User::getTotalUsers()       << endl;
    cout << "  Total listings created   : " << Listing::getTotalListings() << endl;

    return 0;
}
