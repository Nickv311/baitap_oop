
#include <iostream>
#include <string>
#include <vector>
#include <limits>

class Account {
protected:
    std::string accountNumber;
    std::string ownerName;
    double balance;

public:
    Account(const std::string& number, const std::string& name, double initialBalance)
        : accountNumber(number), ownerName(name), balance(initialBalance) {}

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Da nap " << amount << " vao tai khoan." << std::endl;
        }
        else {
            std::cout << "So tien khong hop le." << std::endl;
        }
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            std::cout << "Da rut " << amount << " tu tai khoan." << std::endl;
            return true;
        }
        std::cout << "Khong the rut tien. So du khong du hoac so tien khong hop le." << std::endl;
        return false;
    }

    void transfer(Account* toAccount, double amount) {
        if (withdraw(amount)) {
            toAccount->deposit(amount);
            std::cout << "Da chuyen " << amount << " tu tai khoan " << accountNumber
                << " den tai khoan " << toAccount->accountNumber << "." << std::endl;
        }
    }

    virtual void displayInfo() const {
        std::cout << "So tai khoan: " << accountNumber << std::endl;
        std::cout << "Chu tai khoan: " << ownerName << std::endl;
        std::cout << "So du: " << balance << std::endl;
    }

    virtual ~Account() = default;

    const std::string& getAccountNumber() const {
        return accountNumber;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& name, double initialBalance, double rate)
        : Account(number, name, initialBalance), interestRate(rate) {}

    void applyInterest() {
        double interest = balance * interestRate;
        deposit(interest);
        std::cout << "Da cong lai: " << interest << std::endl;
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << "Lai suat: " << (interestRate * 100) << "%" << std::endl;
    }
};

class FixedDepositAccount : public Account {
private:
    double interestRate;
    int term; // Thời hạn gửi tiền (tháng)

public:
    FixedDepositAccount(const std::string& number, const std::string& name, double initialBalance, double rate, int termMonths)
        : Account(number, name, initialBalance), interestRate(rate), term(termMonths) {}

    void applyInterest() {
        double interest = balance * interestRate;
        deposit(interest);
        std::cout << "Da cong lai: " << interest << std::endl;
    }

    void displayInfo() const override {
        Account::displayInfo();
        std::cout << "Lai suat: " << (interestRate * 100) << "%" << std::endl;
        std::cout << "Thoi han: " << term << " thang" << std::endl;
    }
};

class Bank {
private:
    std::vector<Account*> accounts;

public:
    void addAccount(Account* account) {
        accounts.emplace_back(account);
    }

    Account* findAccount(const std::string& number) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == number) {
                return account;
            }
        }
        std::cout << "Khong tim thay tai khoan." << std::endl;
        return nullptr;
    }

    void displayAllAccounts() const {
        for (const auto& account : accounts) {
            account->displayInfo();
            std::cout << "------------------------" << std::endl;
        }
    }

    ~Bank() {
        for (auto account : accounts) {
            delete account;
        }
    }
};

void displayMenu() {
    std::cout << "=== MENU NGAN HANG ===" << std::endl;
    std::cout << "1. Them tai khoan tiet kiem" << std::endl;
    std::cout << "2. Them tai khoan co dinh" << std::endl;
    std::cout << "3. Chuyen tien" << std::endl;
    std::cout << "4. Tim tai khoan" << std::endl;
    std::cout << "5. Hien thi tat ca tai khoan" << std::endl;
    std::cout << "0. Thoat" << std::endl;
}

int main() {
    Bank myBank;
    int choice;

    do {
        displayMenu();
        std::cout << "Nhap lua chon: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string number, name;
            double balance, rate;
            std::cout << "Nhap so tai khoan: ";
            std::cin >> number;
            std::cout << "Nhap ten chu tai khoan: ";
            std::cin >> name;
            std::cout << "Nhap so du: ";
            std::cin >> balance;
            std::cout << "Nhap lai suat: ";
            std::cin >> rate;
            myBank.addAccount(new SavingsAccount(number, name, balance, rate));
            break;
        }
        case 2: {
            std::string number, name;
            double balance, rate;
            int term;
            std::cout << "Nhap so tai khoan: ";
            std::cin >> number;
            std::cout << "Nhap ten chu tai khoan: ";
            std::cin >> name;
            std::cout << "Nhap so du: ";
            std::cin >> balance;
            std::cout << "Nhap lai suat: ";
            std::cin >> rate;
            std::cout << "Nhap thoi han (thang): ";
            std::cin >> term;
            myBank.addAccount(new FixedDepositAccount(number, name, balance, rate, term));
            break;
        }
        case 3: {
            std::string fromAccount, toAccount;
            double amount;
            std::cout << "Nhap so tai khoan chuyen tu: ";
            std::cin >> fromAccount;
            std::cout << "Nhap so tai khoan chuyen den: ";
            std::cin >> toAccount;
            std::cout << "Nhap so tien chuyen: ";
            std::cin >> amount;

            Account* fromAcc = myBank.findAccount(fromAccount);
            Account* toAcc = myBank.findAccount(toAccount);

            if (fromAcc && toAcc) {
                fromAcc->transfer(toAcc, amount);
            }
            break;
        }
        case 4: {
            std::string number;
            std::cout << "Nhap so tai khoan can tim: ";
            std::cin >> number;
            myBank.findAccount(number);
            break;
        }
        case 5:
            myBank.displayAllAccounts();
            break;
        case 0:
            std::cout << "Cam on ban da su dung dich vu!" << std::endl;
            break;
        default:
            std::cout << "Lua chon khong hop le. Vui long nhap lai!" << std::endl;
        }

    } while (choice != 0);

    return 0;
}
