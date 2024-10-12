#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>

class Borrowable {
protected:
    std::string title;
    std::string author;
    bool isBorrowed;

public:
    Borrowable(const std::string& title, const std::string& author)
        : title(title), author(author), isBorrowed(false) {}

    virtual void borrow() {
        if (isBorrowed) {
            throw std::runtime_error("Tai lieu da duoc muon.");
        }
        isBorrowed = true;
        std::cout << "Da muon tai lieu: " << title << std::endl;
    }

    virtual void returnItem() {
        if (!isBorrowed) {
            throw std::runtime_error("Tai lieu chua duoc muon.");
        }
        isBorrowed = false;
        std::cout << "Da tra tai lieu: " << title << std::endl;
    }

    virtual void displayInfo() const {
        std::cout << "Tieu de: " << title << ", Tac gia: " << author
            << ", Da muon: " << (isBorrowed ? "Co" : "Khong") << std::endl;
    }

    virtual ~Borrowable() = default;

    bool getIsBorrowed() const {
        return isBorrowed;
    }

    const std::string& getTitle() const {
        return title;
    }

    const std::string& getAuthor() const {  // Phương thức truy cập author
        return author;
    }
};

class Magazine : public Borrowable {
public:
    Magazine(const std::string& title, const std::string& author)
        : Borrowable(title, author) {}
};

class User {
private:
    std::string username;
    std::string password;

public:
    User(const std::string& username, const std::string& password)
        : username(username), password(password) {}

    const std::string& getUsername() const {
        return username;
    }

    bool validatePassword(const std::string& pass) const {
        return password == pass;
    }
};

class Library {
private:
    std::vector<Borrowable*> items;
    std::vector<User> users;

public:
    void addItem(Borrowable* item) {
        items.push_back(item);
    }

    void addUser(const User& user) {
        users.push_back(user);
    }

    User* login(const std::string& username, const std::string& password) {
        for (auto& user : users) {
            if (user.getUsername() == username && user.validatePassword(password)) {
                return &user;
            }
        }
        throw std::runtime_error("Dang nhap that bai.");
    }

    void searchAndDisplay(const std::string& query) const {
        std::cout << "Ket qua tim kiem cho: " << query << std::endl;
        for (const auto& item : items) {
            if (item->getTitle().find(query) != std::string::npos ||
                item->getAuthor().find(query) != std::string::npos) { // Sửa để sử dụng getAuthor()
                item->displayInfo();
            }
        }
    }

    void sortItems() {
        std::sort(items.begin(), items.end(), [](const Borrowable* a, const Borrowable* b) {
            return a->getTitle() < b->getTitle();
            });
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Khong the mo file.");
        }
        for (const auto& item : items) {
            file << item->getTitle() << "," << item->getAuthor() << "," << item->getIsBorrowed() << "\n";
        }
        file.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Khong the mo file.");
        }
        std::string line;
        while (std::getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            std::string title = line.substr(0, pos1);
            std::string author = line.substr(pos1 + 1, pos2 - pos1 - 1);
            bool isBorrowed = (line.substr(pos2 + 1) == "1");
            Borrowable* item = new Magazine(title, author);
            if (isBorrowed) {
                item->borrow();
            }
            addItem(item);
        }
        file.close();
    }

    ~Library() {
        for (auto item : items) {
            delete item;
        }
    }
};

static void displayMenu() {
    std::cout << "==== Menu ====" << std::endl;
    std::cout << "1. Dang ky" << std::endl;
    std::cout << "2. Dang nhap" << std::endl;
    std::cout << "3. Tim kiem tai lieu" << std::endl;
    std::cout << "4. Luu du lieu" << std::endl;
    std::cout << "5. Thoat" << std::endl;
    std::cout << "================" << std::endl;
}

int main() {
    Library library;
    try {
        library.loadFromFile("library_data.txt");
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string username, password;
            std::cout << "Nhap username: ";
            std::cin >> username;
            std::cout << "Nhap password: ";
            std::cin >> password;
            library.addUser(User(username, password));
            std::cout << "Dang ky thanh cong!" << std::endl;
            break;
        }
        case 2: {
            std::string username, password;
            std::cout << "Nhap username: ";
            std::cin >> username;
            std::cout << "Nhap password: ";
            std::cin >> password;
            try {
                User* user = library.login(username, password);
                std::cout << "Dang nhap thanh cong!" << std::endl;
                // Các chức năng khác cho người dùng sau khi đăng nhập
            }
            catch (const std::runtime_error& e) {
                std::cout << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string query;
            std::cout << "Nhap tu khoa tim kiem: ";
            std::cin.ignore();
            std::getline(std::cin, query);
            library.searchAndDisplay(query);
            break;
        }
        case 4: {
            try {
                library.saveToFile("library_data.txt");
                std::cout << "Luu du lieu thanh cong!" << std::endl;
            }
            catch (const std::runtime_error& e) {
                std::cout << e.what() << std::endl;
            }
            break;
        }
        case 5:
            std::cout << "Thoat chuong trinh." << std::endl;
            return 0;
        default:
            std::cout << "Lua chon khong hop le. Vui long chon lai." << std::endl;
        }
    }
}
