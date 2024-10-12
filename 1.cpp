#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using std::ifstream;
using std::ofstream;

class Employee
{
protected:
    int id;
    std::string name;
    int age;
    double salary;

public:
    Employee(int _id, std::string _name, int _age, double _salary)
        : id(_id), name(_name), age(_age), salary(_salary) {}

    virtual void displayInfo() const
    {
        std::cout << "ID: " << id << ", Tên: " << name << ", Tuổi: " << age << ", Lương: " << salary << std::endl;
    }
    virtual double getSalary() const
    {
        return salary;
    }
    void increaseSalary(double amount)
    {
        salary += amount;
    }
    int getId()
    {
        return id;
    }
    int getAge()
    {
        return age;
    }
    std::string getName()
    {
        return name;
    }

    std::string serialize() const
    {
        return std::to_string(id) + "," + name + "," + std::to_string(age) + "," + std::to_string(salary);
    }

    static Employee *deserialize(const std::string &data)
    {
        size_t pos = 0;
        int id = std::stoi(data.substr(pos, data.find(',', pos) - pos));
        pos = data.find(',', pos) + 1;

        std::string name = data.substr(pos, data.find(',', pos) - pos);
        pos = data.find(',', pos) + 1;

        int age = std::stoi(data.substr(pos, data.find(',', pos) - pos));
        pos = data.find(',', pos) + 1;

        double salary = std::stod(data.substr(pos));

        return new Employee(id, name, age, salary);
    }
};

class HourlyEmployee : public Employee
{
private:
    int workHours;

public:
    HourlyEmployee(int _id, std::string _name, int _age, int _workHours)
        : Employee(_id, _name, _age, 0), workHours(_workHours) {}
    double getSalary() const override
    {
        return workHours * 25000; // 1 gio 25k
    }
};

class Manager : public Employee
{
private:
    int teamSize;

public:
    Manager(int _id, std::string _name, int _age, double _salary, int _teamSize)
        : Employee(_id, _name, _age, _salary), teamSize(_teamSize) {}
    void displayInfo() const override
    {
        Employee::displayInfo();
        std::cout << "Số nhân viên quản lý: " << teamSize << std::endl;
    }
    double getSalary() const override
    {
        return salary + (teamSize * 1000); // Phụ cấp quản lý
    }
};

class Department
{
private:
    std::vector<Employee *> employees;

public:
    void addEmployee(Employee *emp)
    {
        employees.push_back(emp);
    }

    void displayAllEmployees() const {
        for (const auto& emp : employees) {
            emp->displayInfo();
        }
    }

    Employee *findEmployeeById(int id) const
    {
        for (const auto &emp : employees)
        {
            if (emp->getId() == id)
            {
                return emp;
            }
        }
        return nullptr; // Nếu không tìm thấy
    }

    Employee *findEmployeeByName(const std::string &name) const
    {
        for (const auto &emp : employees)
        {
            if (emp->getName() == name)
            {
                return emp;
            }
        }
        return nullptr; // Nếu không tìm thấy
    }

    void sortByAge()
    {
        sort(employees.begin(), employees.end(), [](Employee *a, Employee *b)
             { return a->getAge() < b->getAge(); });
    }

    void sortBySalary()
    {
        sort(employees.begin(), employees.end(), [](Employee *a, Employee *b)
             { return a->getSalary() < b->getSalary(); });
    }
    std::vector<Employee*>& getEmployees() {
    return employees;
    }

    ~Department()
    {
        for (auto emp : employees)
        {
            delete emp;
        }
    }
};

class Company
{
private:
    std::vector<Employee *> employees;

public:
    void addEmployee(Employee *emp)
    {
        employees.push_back(emp);
    }
    void displayAllEmployees() const
    {
        for (const auto &emp : employees)
        {
            emp->displayInfo();
            std::cout << "------------------------" << std::endl;
        }
    }
    double getTotalSalary() const
    {
        double total = 0;
        for (const auto &emp : employees)
        {
            total += emp->getSalary();
        }
        return total;
    }
    ~Company()
    {
        for (auto emp : employees)
        {
            delete emp;
        }
    }
};

void saveToFile(const std::vector<Employee *> &employees, const std::string &filename)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        std::cout << "Error opening file for writing." << std::endl;
        return;
    }

    for (const auto &emp : employees)
    {
        outFile << emp->serialize() << std::endl;
    }
    outFile.close();
}

void loadFromFile(std::vector<Employee *> &employees, const std::string &filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        std::cout << "Error opening file for reading." << std::endl;
        return;
    }

    std::string line;
    while (getline(inFile, line))
    {
        if (!line.empty())
        {
            employees.push_back(Employee::deserialize(line));
        }
    }
    inFile.close();
}

int main()
{
    Company myCompany;
    myCompany.addEmployee(new Employee(1, "Nguyễn Văn A", 30, 10000000));
    myCompany.addEmployee(new Employee(2, "Trần Thị B", 25, 8000000));
    myCompany.addEmployee(new Manager(3, "Lê Văn C", 40, 20000000, 5));
    std::cout << "Thông tin tất cả nhân viên:" << std::endl;
    myCompany.displayAllEmployees();
    std::cout << "Tổng lương công ty: " << myCompany.getTotalSalary() << std::endl;


    Department salesDept;
    salesDept.addEmployee(new Employee(1, "Nguyễn Văn A", 30, 10000000));
    salesDept.addEmployee(new Employee(2, "Trần Thị B", 25, 8000000));
    salesDept.addEmployee(new Manager(3, "Lê Văn C", 40, 20000000, 5));
    salesDept.addEmployee(new HourlyEmployee(4, "Quoc Anh", 20, 4));
    Employee* e1 = salesDept.findEmployeeById(2);
    Employee* e2 = salesDept.findEmployeeByName("Lê Văn C");
    salesDept.sortBySalary();
    std::cout << "\nSap xep theo luong: " << std::endl;
    salesDept.displayAllEmployees();
    salesDept.sortByAge();
    std::cout << "\nSap xep theo tuoi: " << std::endl;
    salesDept.displayAllEmployees();

    std::cout << "\n Ten nhan vien co id 2: " << e1->getName() << std::endl;
    std::cout << "\n Id nhan vien co ten Le Van C: " << e2->getId() << std::endl;
    saveToFile(salesDept.getEmployees(), "employees.txt");

    salesDept = Department();
    loadFromFile(salesDept.getEmployees(), "employees.txt");
    std::cout << "\nLay nhan vien tu file:" << std::endl;
    salesDept.displayAllEmployees();

    return 0;
}