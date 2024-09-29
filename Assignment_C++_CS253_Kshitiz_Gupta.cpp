#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

const int year_max = 4000;
const int year_min = 1500;

struct date
{
    int d, m, y;
};

bool isLeap(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}

int leapYear_count(date d)
{
    int ye = d.y;

    if (d.m <= 2)
        ye--;

    return ye / 4 - ye / 100 + ye / 400;
}
// Returns true if given year is valid
bool Date_valid(int d, int m, int y)
{

    if (y > year_max ||
        y < year_min)
        return false;
    if (m < 1 || m > 12)
        return false;
    if (d < 1 || d > 31)
        return false;

    if (m == 2)
    {
        if (isLeap(y))
            return (d <= 29);
        else
            return (d <= 28);
    }

    if (m == 4 || m == 6 ||
        m == 9 || m == 11)
        return (d <= 30);

    return true;
}

const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// gets difference between two dates
int Diff_date(date dt1, date dt2)
{

    long int n1 = dt1.y * 365 + dt1.d;

    for (int i = 0; i < dt1.m - 1; i++)
        n1 += monthDays[i];

    n1 += leapYear_count(dt1);
    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++)
        n2 += monthDays[i];
    n2 += leapYear_count(dt2);

    return (n2 - n1);
}

int MoreDays(int day, int mon, int year)
{
    int offset = day;
    int t = mon - 1;
    if (t == 1 || t == 3 || t == 5 || t == 7 || t == 8 || t == 10)
        offset += 31;
    else if (t == 2)
        offset += 28;
    else
        offset += 30;

    if (isLeap(year) && mon > 2)
        offset += 1;

    return offset;
}

void revMoreDays(int offset, int y, int *d, int *m)
{
    int month[13] = {0, 31, 28, 31, 30, 31, 30,
                     31, 31, 30, 31, 30, 31};

    if (isLeap(y))
        month[2] = 29;

    int i;
    for (i = 1; i <= 12; i++)
    {
        if (offset <= month[i])
            break;
        offset = offset - month[i];
    }

    *d = offset;
    *m = i;
}

// Add x days to the given date.
date addOffset(int d1, int m1, int y1, int x)
{
    int offset1 = MoreDays(d1, m1, y1);
    int remDays = isLeap(y1) ? (366 - offset1) : (365 - offset1);

    int y2, offset2;
    if (x <= remDays)
    {
        y2 = y1;
        offset2 = offset1 + x;
    }

    else
    {
        x -= remDays;
        y2 = y1 + 1;
        int y2days = isLeap(y2) ? 366 : 365;
        while (x >= y2days)
        {
            x -= y2days;
            y2++;
            y2days = isLeap(y2) ? 366 : 365;
        }
        offset2 = x;
    }
    int m2, d2;
    revMoreDays(offset2, y2, &d2, &m2);
    return {d2, m2, y2};
}
/*Above functions have been taken from other sites,such as gfg, for calculating diffenece between dates, and checking their validity and other date related functionalities*/

/*Class definitions and related functions*/
class Car;
class Customer;
class Employee;
class Manager;

map<int, vector<int>> ListOfAllCars;
vector<pair<pair<int, string>, pair<string, int>>> List_customer;
vector<pair<pair<int, string>, pair<string, int>>> List_employee;

map<int, int> carOfCust;
map<int, int> carOfEmp;

vector<int> model_used;
vector<int> Cars_avail;

/*Have used maps for storing the database instead of using an external utility like MySQL*/
map<int, Car> List_car;
map<int, Customer> CustObjList;
map<int, Employee> EmpObjList;
map<int, Manager> ManagerList;

class Car
{
protected:
    int model;
    int condition;
    int availOrNot;
    int Car_price;

public:
    Car() {}
    Car(int model, int condition, int availOrNot, int Car_price)
    {
        this->model = model;
        this->condition = condition;
        this->availOrNot = availOrNot;
        this->Car_price = Car_price;
        model_used.push_back(model);
    }
    ~Car(){};
};

/*Have a class named Person which has almost all attributes and functions common to customer and employee*/
class Person
{
protected:
    int id, fine = 2;
    string name;
    string password;
    int record;
    vector<pair<int, char>> rented_Car;

    // The char parameter here has values 'C' and 'E' which stores whether the car is rented by a customer or employee
    vector<pair<pair<int, char>, pair<date, date>>> rented_cars_detail;

public:
    /*Declaration of all functions that will be inherited by customer and employee class*/
    int Print_fines(map<int, int>);
    void DueClear(int, map<int, int>);

    void Rent_cars(int, int, date, char, map<int, int> &);
    void SeeRented_Cars();
    bool checkValidityOfReturn(int, date);
    bool RentedCar_check(int, char);
    void ReturnCar(int, int, map<int, int> &, date, char);
    void SeeDueDates();
};

/*This function checks if the return date entered by the customer/employee is valid,i.e. return date is greater than the date of issuing of the car*/
bool Person::checkValidityOfReturn(int modelId, date return_date)
{
    int i;

    for (i = 0; i < rented_cars_detail.size(); i++)
    {
        if (rented_cars_detail[i].first.first == modelId)
        {
            break;
        }
    }
    if (Diff_date(rented_cars_detail[i].second.first, return_date) < 0)
    {
        return false;
    }
    return true;
}

/*passing map as a parameter to distinguish between whether this function is called for customer or employee..based on that cust_dues map or emp_dues map will be used*/
void Person::ReturnCar(int ModelName, int cond, map<int, int> &whose_dues, date return_date, char who_is_returning)
{

    for (int i = 0; i < rented_cars_detail.size(); i++)
    {
        if (rented_cars_detail[i].first.first == ModelName && rented_cars_detail[i].first.second == who_is_returning)
        {
            Cars_avail.push_back(ModelName);

            ListOfAllCars[ModelName][1] = 1;

            if (ListOfAllCars[ModelName][0] > cond)
            {
                record = record - 1;
            }

            ListOfAllCars[ModelName][0] = cond;

            // No. of days after due time
            int days = Diff_date(rented_cars_detail[i].second.second, return_date);

            // if the number of days are more than 0, a fine of Rs 2(here) per day is imposed for this no. of days

            if (days > 0)
            {
                whose_dues[ModelName] = days * fine;
            }

            // If person is returning car after 15 days of due date(30 days from rent date), then his customer record will decrease
            if (days > 15)
            {
                record = (record - 1 >= 0) ? record - 1 : 0;
            }

            rented_cars_detail.erase(rented_cars_detail.begin() + i);
            for (int i = 0; i < rented_Car.size(); i++)
            {
                if (rented_Car[i].first == ModelName && rented_Car[i].second == who_is_returning)
                {
                    break;
                }
            }
            rented_Car.erase(rented_Car.begin() + i);
        }
    }
}

/*To see the booking date of a car and when they are expected to return the car*/
void Person::SeeDueDates()
{
    for (auto it : rented_cars_detail)
    {
        cout << "Model name: " << it.first.first << endl
             << "Booking date: " << it.second.first.d << "/" << it.second.first.m << "/" << it.second.first.y << endl;
        cout << "Due date: " << it.second.second.d << "/" << it.second.second.m << "/" << it.second.second.y << endl;
    }

    if (rented_cars_detail.size() == 0)
        cout << "You do not have any rented cars\n";
}

/*If record of person is good enough and the car he wants to rent is available, then it will be successful*/
void Person::Rent_cars(int id, int model, date rentdate, char who_is_renting, map<int, int> &car_rented_to_who)
{
    int i, flag = 0;
    if (record > rented_cars_detail.size())
    {

        for (i = 0; i < Cars_avail.size(); i++)
        {

            if (Cars_avail[i] == model)
            {
                flag = 1;
                break;
            }
        }

        if (flag && ListOfAllCars[model][1] != 0)
        {
            Cars_avail.erase(Cars_avail.begin() + i);
            rented_Car.push_back({model, who_is_renting});
            rented_cars_detail.push_back({{model, who_is_renting}, {rentdate, addOffset(rentdate.d, rentdate.m, rentdate.y, 15)}});
            ListOfAllCars[model][1] = 0;
            car_rented_to_who[model] = id;
            cout << "Successfully rented :)\n";
            cout << "Caution: You will have to pay a fine of Rs 2 per day after the due date. Deteriorating the car will lead to a decrease in your record and once it becomes 0, you can't rent any car\n";
        }

        else
            cout << "Sorry! We don't have car of this model available.\n";
    }
    else
        cerr << "Your ID have reached limit of renting cars.\n";
}

/*This function will let a customer/employee see the pending dues that they have(this will include dues/fine of the car if they have returned it later than the specified period of 15 days) */
int Person::Print_fines(map<int, int> whose_dues)
{
    if (whose_dues.size() == 0)
    {
        cout << "No Dues Pending:)\n"
             << endl;
        return 0;
    }
    else
    {
        for (auto it : whose_dues)
        {
            cout << "Car: " << it.first << "   "
                 << "dues: " << it.second << endl;
            cout << endl;
        }
        return 1;
    }
}

/*If dues for given model is available in person's dues list, then it will be cleared*/
void Person::DueClear(int ModelName, map<int, int> person_dues)
{
    if (person_dues.find(ModelName) != person_dues.end())
    {
        cout << "Dues cleared:)" << endl;
        person_dues.erase(ModelName);
    }
    else
    {
        cout << "You have no dues of this model of the car\n";
    }
}

bool Person::RentedCar_check(int ModelName, char who_is_renting)
{
    for (int i = 0; i < rented_Car.size(); i++)
    {
        if (rented_Car[i].first == ModelName && rented_Car[i].second == who_is_renting)
        {
            return true;
        }
    }
    return false;
}

void Person::SeeRented_Cars()
{
    for (auto it : rented_cars_detail)
    {
        vector<int> temp_car = ListOfAllCars[it.first.first];

        cout << "Model name: " << it.first.first << ", Condition: " << temp_car[0] << ", Rent price: " << temp_car[2] << endl;
    }

    if (rented_cars_detail.size() == 0)
        cout << "You do not have any rented cars\n";
}

/*Customer class inherited from Person Class, since all the functions have already been defined in Public of Person class, all those functions will be inherited*/
class Customer : public Person
{
protected:
public:
    map<int, int> cust_dues;
    vector<int> Cust_ids;
    Customer() {}
    Customer(int Customer_id, string Customer_name, string Customer_password, int Customer_record)
    {
        this->id = Customer_id;
        this->name = Customer_name;
        this->password = Customer_password;
        this->record = Customer_record;
        Cust_ids.push_back(Customer_id);
    }
    ~Customer(){};
};

/*Employee class inherited from Person Class, since all the functions have already been defined in Public of Person class, all those functions will be inherited*/
class Employee : public Person
{
protected:
    /*Discount of 15% to employees*/
    float discount = 0.15;

public:
    map<int, int> emp_dues;
    vector<int> Emp_ids;

    Employee() {}
    Employee(int Employee_id, string Employee_name, string Employee_password, int Employee_record)
    {
        this->id = Employee_id;
        this->name = Employee_name;
        this->password = Employee_password;
        this->record = Employee_record;
        Emp_ids.push_back(Employee_id);
    }
    ~Employee(){};
};

class Manager
{
private:
    int Manager_id;
    string Manager_name;
    string Manager_password;

public:
    vector<int> Manager_ids;
    Manager() {}
    Manager(int Manager_id, string Manager_name, string Manager_password)
    {
        this->Manager_id = Manager_id;
        this->Manager_name = Manager_name;
        this->Manager_password = Manager_password;
        Manager_ids.push_back(Manager_id);
    }
    ~Manager(){};

    void Add_car(int, int, int, int);
    void CarUpdate(int, int, int, int);
    void Car_Delete(int);
    void FindCar(int);
    void add_CustOrEmp(int, string, string, int, char);
    void UpdatePerson_details(int, string, string, int, vector<pair<pair<int, string>, pair<string, int>>> &);
    void DeletePerson_details(int, char, vector<pair<pair<int, string>, pair<string, int>>> &);
    void SeeRented_Cars(map<int, int>);
};

void Manager::Add_car(int ModelName, int condition, int availOrNot, int Car_price)
{
    if (List_car.find(ModelName) == List_car.end())
    {
        Car car_object(ModelName, condition, availOrNot, Car_price);
        List_car[ModelName] = car_object;

        ListOfAllCars[ModelName] = {condition, availOrNot, Car_price};
        Cars_avail.push_back(ModelName);

        cout << "Car Successfully added" << endl;
    }

    else
    {
        cout << "Car already present" << endl;
    }
}

/*Function to update car*/
void Manager::CarUpdate(int ModelName, int cond, int availability, int newprice)
{
    List_car.erase(ModelName);
    ListOfAllCars.erase(ModelName);

    Car car_object(ModelName, cond, availability, newprice);
    List_car[ModelName] = car_object;

    ListOfAllCars[ModelName] = {cond, availability, newprice};
    cout << "Successfully Updated!!" << endl;
}

/*Function to Delete car*/
void Manager::Car_Delete(int ModelName)
{
    int i = 0;

    ListOfAllCars.erase(ModelName);
    for (i = 0; i < Cars_avail.size(); i++)
    {
        if (Cars_avail[i] == ModelName)
        {
            break;
        }
    }
    Cars_avail.erase(Cars_avail.begin() + i);
    List_car.erase(ModelName);
    cout << "Successfully deleted" << endl;
}

void Manager::add_CustOrEmp(int id, string name, string password, int record, char EmpOrCust)
{
    if (EmpOrCust == 'E')
    {
        if (EmpObjList.find(id) == EmpObjList.end())
        {

            cout << "Successfully added" << endl;

            Employee obj(id, name, password, record);
            EmpObjList[id] = obj;
            List_employee.push_back({{id, name}, {password, record}});
        }
        else
        {
            cout << "This person is already present in the database" << endl;
        }
    }

    else
    {
        if (CustObjList.find(id) == CustObjList.end())
        {

            cout << "Customer Successfully added" << endl;

            Customer obj(id, name, password, record);
            CustObjList[id] = obj;
            List_customer.push_back({{id, name}, {password, record}});
        }
        else
        {
            cout << "This customer is already present in the database" << endl;
        }
    }
}

void Manager::UpdatePerson_details(int id, string name, string password, int record, vector<pair<pair<int, string>, pair<string, int>>> &list)
{
    cout << "Successfully Updated!!" << endl;

    for (int i = 0; i < list.size(); i++)
    {
        if (id == list[i].first.first)
        {
            list[i].first.second = name;
            list[i].second.first = password;
            list[i].second.second = record;
        }
    }
}

void Manager::DeletePerson_details(int id, char EmpOrCust, vector<pair<pair<int, string>, pair<string, int>>> &list)
{

    int i;
    cout << "Person deleted!!" << endl;
    for (i = 0; i < list.size(); i++)
    {
        if (id == list[i].first.first)
        {
            break;
        }
    }
    list.erase(list.begin() + i);

    if (EmpOrCust == 'C')
        CustObjList.erase(id);
    else if (EmpOrCust == 'E')
        EmpObjList.erase(id);
}

/*Using this function, manager will be able to see cars rented by customers and employees*/
void Manager::SeeRented_Cars(map<int, int> mp)
{
    if (mp.size() == 0)
    {
        cout << "No cars are currently rented." << endl;
    }

    for (auto it : mp)
    {
        cout << "Car model is: " << it.first << " which is rented to Customer with Customer id: " << it.second << endl;
    }
}

void print_available_cars(int discount)
{
    for (auto it : Cars_avail)
    {

        vector<int> temp = ListOfAllCars[it];
        if (ListOfAllCars[it][1] != 0)
        {
            cout << "Model name: " << it << ", Condition: " << temp[0] << ", Rent price: " << (temp[2] * (1 - discount)) << endl;
        }
    }
}

int main()
{
    /*customer(id,name,password,record)*/
    /*record=3 is best*/
    /*Same goes for employees*/
    Customer Cust1(1, "KimHayoon", "#Cust1", 3);
    CustObjList[1] = Cust1;
    List_customer.push_back({{1, "KimHayoon"}, {"#Cust1", 3}});

    Customer Cust2(2, "JangUk", "#Cust2", 3);
    CustObjList[2] = Cust2;
    List_customer.push_back({{2, "JangUk"}, {"#Cust2", 3}});

    Customer Cust3(3, "Naksu", "#Cust3", 3);
    CustObjList[3] = Cust3;
    List_customer.push_back({{3, "Naksu"}, {"#Cust3", 3}});

    Customer Cust4(4, "SeoYul", "#Cust4", 3);
    CustObjList[4] = Cust4;
    List_customer.push_back({{4, "SeoYul"}, {"#Cust4", 3}});

    Customer Cust5(5, "MuDoeki", "#Cust5", 3);
    CustObjList[5] = Cust5;
    List_customer.push_back({{5, "MuDoeki"}, {"#Cust5", 3}});

    /*Employees object*/
    Employee Emp1(1, "BuYoen", "#emp1", 3);
    EmpObjList[1] = Emp1;
    List_employee.push_back({{1, "BuYoen"}, {"#emp1", 3}});

    Employee Emp2(2, "ChoYeon", "#emp2", 3);
    EmpObjList[2] = Emp2;
    List_employee.push_back({{2, "ChoYeon"}, {"#emp2", 3}});

    Employee Emp3(3, "Namjoon", "#emp3", 3);
    EmpObjList[3] = Emp3;
    List_employee.push_back({{3, "Namjoon"}, {"#emp3", 3}});

    Employee Emp4(4, "Taehyoung", "#emp4", 3);
    EmpObjList[4] = Emp4;
    List_employee.push_back({{4, "Taehyoung"}, {"#emp4", 3}});

    Employee Emp5(5, "Ren", "#emp5", 3);
    EmpObjList[5] = Emp5;
    List_employee.push_back({{5, "Ren"}, {"#emp5", 3}});

    /* car(model,condition,availability,price) */
    /*Initialising objects*/
    /*Availability=1 means it is available, 0 means it is unavailable(Either rented or marked unavailable by the manager)*/
    /*Condition =3(good), 2(average), 1(bad)*/
    Car car1(1, 3, 1, 1000);
    Cars_avail.push_back(1);
    ListOfAllCars[1] = {3, 1, 1000};
    List_car[1] = car1;

    Car car2(2, 3, 1, 2000);
    Cars_avail.push_back(2);
    ListOfAllCars[2] = {3, 1, 2000};
    List_car[1] = car2;

    Car car3(3, 3, 1, 3000);
    Cars_avail.push_back(3);
    ListOfAllCars[3] = {3, 1, 3000};
    List_car[1] = car3;

    Car car4(4, 3, 1, 4000);
    Cars_avail.push_back(4);
    ListOfAllCars[4] = {3, 1, 4000};
    List_car[1] = car4;

    Car car5(5, 3, 1, 5000);
    Cars_avail.push_back(5);
    ListOfAllCars[5] = {3, 1, 5000};
    List_car[1] = car5;

    /*Manager object*/
    /*Assuming there is only one manager*/
    Manager manager1(1, "king", "manager_password");
    ManagerList[1] = manager1;

    int y = 1;

    // Functioning Starts from here

    cout << "Welcome to our Car Renting Platform!! Hoping you will have a great experience:)" << endl;
    while (y == 1)
    {
        cout << "Type 1 for customer, 2 for employee, 3 for manager and 0 to exit" << endl;
        int choice, custID;
        cin >> choice;
        int x = 0;

        if (choice == 1)
        {
            cout << "Enter your customer ID(integer).\n";
            cin >> custID;
            if (CustObjList.find(custID) == CustObjList.end())
            {
                cerr << "You are not a customer.\n";
                break;
            }

            else
            {
                while (x != 1)
                {

                    cout << "Type 1 to see all available cars" << endl;
                    cout << "Type 2 to rent a car" << endl;
                    cout << "Type 3 to see your rented cars" << endl;
                    cout << "Type 4 to see due(return) dates of all the cars that you have rented" << endl;
                    cout << "Type 5 to return a car" << endl;
                    cout << "Type 6 to see/clear dues" << endl;
                    cout << "Type 7 to go to the Home Page" << endl;

                    int option1;
                    cin >> option1;
                    if (option1 != 1 && option1 != 2 && option1 != 3 && option1 != 4 && option1 != 5 && option1 != 6 && option1 != 7)
                    {
                        cout << "Invalid Input..Exiting:)\n";
                        break;
                    }
                    if (option1 == 1)
                    {
                        // print_available_cars();
                        print_available_cars(0);

                        x = 0;
                        cout << endl;
                        continue;
                    }

                    else if (option1 == 2)
                    {

                        cout << "Which model would you like to rent? Type the model name e.g.1, or 2\n";

                        int model_in, day, month, year;
                        cin >> model_in;

                        cout << "Rent date(format-(dd mm yyyy)): " << endl;
                        cin >> day >> month >> year;

                        while (!Date_valid(day, month, year))
                        {

                            cerr << "Not a valid date!! " << endl;

                            cerr << "Enter a valid date(format-(dd mm yyyy)): " << endl;
                            cin >> day >> month >> year;
                        }

                        CustObjList[custID].Rent_cars(custID, model_in, {day, month, year}, 'C', carOfCust);
                        cout << endl;

                        continue;
                    }

                    else if (option1 == 3)
                    {
                        CustObjList[custID].SeeRented_Cars();
                        cout << endl;
                        continue;
                    }

                    else if (option1 == 4)
                    {
                        CustObjList[custID].SeeDueDates();
                        cout << endl;
                        continue;
                    }
                    else if (option1 == 5)
                    {
                        int y = 0;
                        int day, month, year, con;
                        int return_car_model;

                        while (y != 1)
                        {
                            cout << "Enter the car's model id which you want to return: " << endl;
                            cin >> return_car_model;
                            if ((CustObjList[custID].RentedCar_check(return_car_model, 'C')))
                            {

                                cout << "Return date(format-(dd mm yyyy)): " << endl;
                                cin >> day >> month >> year;

                                while (!Date_valid(day, month, year))
                                {
                                    cerr << "Not a valid date" << endl;

                                    cerr << "Enter a valid Return date(format-(dd mm yyyy)): " << endl;
                                    cin >> day >> month >> year;
                                }

                                while (!CustObjList[custID].checkValidityOfReturn(return_car_model, {day, month, year}))
                                {

                                    cout << "Enter correct return date(format-(dd mm yyyy)): " << endl;
                                    cin >> day >> month >> year;
                                }

                                cout << "The car condition when you rented the car: " << ListOfAllCars[return_car_model][0] << endl
                                     << "Enter current condition of the car(scale- 1 to 3, where 3 is the best): " << endl;

                                cin >> con;
                                while (con > ListOfAllCars[return_car_model][0] && con < 1)
                                {
                                    cerr << "Enter a valid input as mentioned above\n";
                                    cin >> con;
                                }

                                CustObjList[custID].ReturnCar(return_car_model, con, CustObjList[custID].cust_dues, {day, month, year}, 'C');
                                y = 1;
                                cout << "the car has been successfully returned!!" << endl;

                                carOfCust.erase(return_car_model);

                                cout << endl;
                            }
                            else
                            {
                                cerr << "You have not rented this car\n"
                                     << endl;
                                break;
                            }
                        }
                    }
                    else if (option1 == 6)
                    {
                        int input, carmodel;

                        cout << "Your dues are: " << endl;
                        cout << endl;
                        if (CustObjList[custID].Print_fines(CustObjList[custID].cust_dues) == 1)
                        {

                            cout << "If you want to clear your dues right now type 1 else type 0 to return to main menu" << endl;
                            cin >> input;
                            if (input == 1)
                            {
                                cout << "Enter the Car model of which you want to clear dues: " << endl;
                                cin >> carmodel;
                                CustObjList[custID].DueClear(carmodel, CustObjList[custID].cust_dues);
                                continue;
                            }
                            else if (input == 0)
                            {
                                continue;
                            }

                            else
                            {
                                cerr << "Invalid Input!! Directing you to main menu" << endl;
                                continue;
                            }
                        }
                    }

                    else if (option1 == 7)
                    {
                        break;
                    }

                    else
                        cerr << "Invalid input!!\n";
                }
            }
        }
        else if (choice == 2)
        {
            int empID;

            cout << "Enter your Employee ID(integer)\n";
            cin >> empID;

            if (EmpObjList.find(empID) == EmpObjList.end())
            {
                cerr << "You are not an Employee.\n";
                break;
            }

            else
            {
                while (x != 1)
                {

                    cout << "Type 1 to see all available cars" << endl;
                    cout << "Type 2 to rent a car" << endl;
                    cout << "Type 3 to see your rented cars" << endl;
                    cout << "Type 4 to see due dates of all the cars that you have rented" << endl;
                    cout << "Type 5 to return a car" << endl;
                    cout << "Type 6 to see/clear dues" << endl;
                    cout << "Type 7 to go to the Home Page" << endl;

                    int option1;
                    cin >> option1;
                    if (option1 != 1 && option1 != 2 && option1 != 3 && option1 != 4 && option1 != 5 && option1 != 6 && option1 != 7)
                    {
                        cout << "Invalid Input..Exiting:)\n";
                        break;
                    }
                    if (option1 == 1)
                    {
                        print_available_cars(0.15);

                        x = 0;
                        cout << endl;
                        continue;
                    }

                    else if (option1 == 2)
                    {

                        cout << "Which model do you want to rent? Type the model name..e.g.1, or 2\n";

                        int model_in, day, month, year;
                        cin >> model_in;

                        cout << "Rent date(format-day(DD),month(MM),year(YYYY)): " << endl;
                        cin >> day >> month >> year;

                        while (!Date_valid(day, month, year))
                        {

                            cerr << "Not a valid date!! " << endl;

                            cerr << "Enter a valid date(format-(dd mm yyyy)): " << endl;
                            cin >> day >> month >> year;
                        }

                        EmpObjList[empID].Rent_cars(empID, model_in, {day, month, year}, 'E', carOfEmp);
                        cout << endl;

                        continue;
                    }

                    else if (option1 == 3)
                    {
                        EmpObjList[empID].SeeRented_Cars();
                        cout << endl;
                        continue;
                    }

                    else if (option1 == 4)
                    {
                        EmpObjList[empID].SeeDueDates();
                        cout << endl;
                        continue;
                    }
                    else if (option1 == 5)
                    {
                        int y = 0;
                        int day, month, year, con;
                        int return_car_model;

                        while (y != 1)
                        {
                            cout << "Enter the car's model id which you want to return: " << endl;
                            cin >> return_car_model;
                            if ((EmpObjList[empID].RentedCar_check(return_car_model, 'E')))
                            {

                                cout << "Return date(format-day(DD),month(MM),year(YYYY)): " << endl;
                                cin >> day >> month >> year;

                                while (!Date_valid(day, month, year))
                                {
                                    cerr << "Not a valid date" << endl;

                                    cerr << "Enter a valid Return date(format-(dd mm yyyy)): " << endl;
                                    cin >> day >> month >> year;
                                }

                                while (!EmpObjList[empID].checkValidityOfReturn(return_car_model, {day, month, year}))
                                {
                                    cout << "Enter correct return date(format-day(DD),month(MM),year(YYYY)): " << endl;
                                    cin >> day >> month >> year;
                                }

                                cout << "Car condition when you rented the car: " << ListOfAllCars[return_car_model][0] << endl
                                     << "Enter current condition of the car(scale- 1 to 3, where 3 is the best): " << endl;

                                cin >> con;
                                while (con > ListOfAllCars[return_car_model][0] && con < 1)
                                {
                                    cerr << "Enter a valid input as mentioned above\n";
                                    cin >> con;
                                }

                                EmpObjList[empID].ReturnCar(return_car_model, con, EmpObjList[empID].emp_dues, {day, month, year}, 'E');
                                y = 1;
                                cout << "the car has been successfully returned!!" << endl;
                                cout << endl;

                                carOfEmp.erase(return_car_model);
                            }
                            else
                            {
                                cerr << "You have not rented this car\n"
                                     << endl;
                                break;
                            }
                        }
                    }
                    else if (option1 == 6)
                    {
                        int input, carmodel;

                        cout << "Your dues are: " << endl;
                        cout << endl;
                        if (EmpObjList[empID].Print_fines(EmpObjList[empID].emp_dues) == 1)
                        {

                            cout << "If you want to clear your dues right now type 1 else type 0 to return to main menu" << endl;
                            cin >> input;
                            if (input == 1)
                            {
                                cout << "Enter the Car model of which you want to clear dues: " << endl;
                                cin >> carmodel;
                                EmpObjList[empID].DueClear(carmodel, EmpObjList[empID].emp_dues);
                                continue;
                            }
                            else if (input == 0)
                            {
                                continue;
                            }

                            else
                            {
                                cerr << "Invalid Input!! Directing you to main menu" << endl;
                                continue;
                            }
                        }
                    }

                    else if (option1 == 7)
                    {
                        break;
                    }

                    else
                        cerr << "Invalid input!!\n";
                }
            }
        }
        else if (choice == 3)
        {
            int Manager_id;
            int x = 0;
            cout << "Enter your id" << endl;
            cin >> Manager_id;

            if (ManagerList.find(Manager_id) == ManagerList.end())
            {
                cerr << "You are not a Manager.\n";
                break;
            }
            else
            {
                while (x == 0)
                {
                    int option1;
                    cout << "Type 1 to add a car" << endl;
                    cout << "Type 2 to update details of a existing car" << endl;
                    cout << "Type 3 to delete a car" << endl;
                    cout << "Type 4 to add an Employee" << endl;
                    cout << "Type 5 to add a Customer" << endl;
                    cout << "Type 6 to update a customer's details" << endl;
                    cout << "Type 7 to update a employee's details" << endl;
                    cout << "Type 8 to delete a customer" << endl;
                    cout << "Type 9 to delete an employee" << endl;
                    cout << "Type 10 to see all the cars" << endl;
                    cout << "Type 11 to see all rented cars of customers" << endl;
                    cout << "Type 12 to see all rented cars of employees" << endl;
                    cout << "Type 13 to return to Home Page" << endl;

                    cin >> option1;
                    if (option1 != 1 && option1 != 2 && option1 != 3 && option1 != 4 && option1 != 5 && option1 != 6 && option1 != 7 && option1 != 8 && option1 != 9 && option1 != 10 && option1 != 11 && option1 != 12 && option1 != 13)
                    {
                        cout << "Invalid Input..Exiting:)\n";
                        break;
                    }
                    int CarModel;
                    if (option1 == 1)
                    {

                        cout << "Enter the Car model id: " << endl;
                        cin >> CarModel;

                        if (ListOfAllCars.find(CarModel) == ListOfAllCars.end())
                        {
                            int newcond, newprice;
                            cout << "Enter New Car's condition (between 1 to 3) and the price you want to set: " << endl;
                            cin >> newcond >> newprice;

                            ManagerList[Manager_id].Add_car(CarModel, newcond, 1, newprice);
                        }
                        else
                        {
                            cerr << "Car already present" << endl;
                        }
                    }
                    else if (option1 == 2)
                    {
                        int CarModel, newcond, newaval, newprice;
                        cout << "Enter existing car's model:" << endl;
                        cin >> CarModel;

                        if (ListOfAllCars.find(CarModel) == ListOfAllCars.end())
                        {
                            cerr << "Car with model is not present!!" << endl;
                        }
                        else
                        {
                            cout << "Enter updated car condition (between 1 to 3), updated availibility, and updated rental price" << endl;
                            cin >> newcond >> newaval >> newprice;
                            ManagerList[Manager_id].CarUpdate(CarModel, newcond, newaval, newprice);
                        }
                    }
                    else if (option1 == 3)
                    {

                        int CarModel;
                        cout << "Enter car's model you want to delete:" << endl;
                        cin >> CarModel;

                        if (ListOfAllCars.find(CarModel) == ListOfAllCars.end())
                        {
                            cerr << "Car with model is not present!!" << endl;
                        }
                        else
                        {
                            ManagerList[Manager_id].Car_Delete(CarModel);
                        }
                    }
                    else if (option1 == 4)
                    {
                        int emp_id, cusrecord;
                        string name, password;
                        cout << "Enter Employee's id: " << endl;
                        cin >> emp_id;
                        cout << "Enter Employee details" << endl
                             << "Enter Employee's Name: " << endl
                             << "Enter Employee's password: " << endl
                             << "Enter Employee's record: " << endl;
                        cin >> name >> password >> cusrecord;

                        ManagerList[Manager_id].add_CustOrEmp(emp_id, name, password, cusrecord, 'E');
                    }

                    else if (option1 == 5)
                    {

                        int cust_id, cusrecord;
                        string name, password;
                        cout << "Enter Customer's id: " << endl;
                        cin >> cust_id;
                        cout << "Enter Customer details" << endl
                             << "Enter Customer's Name: " << endl
                             << "Enter Customer's password: " << endl
                             << "Enter Customer's record: " << endl;
                        cin >> name >> password >> cusrecord;

                        ManagerList[Manager_id].add_CustOrEmp(cust_id, name, password, cusrecord, 'C');
                    }
                    else if (option1 == 6)
                    {

                        int cus_id, cusrecord;
                        string name, password;
                        cout << "Enter the customer's id of the customer whom you want to update the details" << endl;
                        cin >> cus_id;

                        if (CustObjList.find(cus_id) != CustObjList.end())
                        {

                            cout << "Enter Customer details" << endl
                                 << "Enter updated Customer's Name: " << endl
                                 << "Enter updated Customer's password: " << endl
                                 << "Enter updated Customer's record: " << endl;
                            cin >> name >> password >> cusrecord;

                            ManagerList[Manager_id].UpdatePerson_details(cus_id, name, password, cusrecord, List_customer);
                        }
                        else
                        {
                            cerr << "Customer with this id is not in the database" << endl;
                        }
                    }
                    else if (option1 == 7)
                    {

                        int emp_id, emprecord;
                        string name, password;
                        cout << "Enter the employee's id of the employee whom you want to update the details" << endl;
                        cin >> emp_id;

                        if (EmpObjList.find(emp_id) != EmpObjList.end())
                        {

                            cout << "Enter Employee details" << endl
                                 << "Enter updated Employee's Name: " << endl
                                 << "Enter updated Employee's password: " << endl
                                 << "Enter updated Employee's record: " << endl;
                            cin >> name >> password >> emprecord;

                            ManagerList[Manager_id].UpdatePerson_details(emp_id, name, password, emprecord, List_employee);
                        }
                        else
                        {
                            cerr << "Employee with this is id is not in the database" << endl;
                        }
                    }
                    else if (option1 == 8)
                    {

                        int id;
                        cout << "Enter the customer's id of the customer whom you want to delete: " << endl;
                        cin >> id;

                        if (CustObjList.find(id) != CustObjList.end())
                        {

                            ManagerList[Manager_id].DeletePerson_details(id, 'C', List_customer);
                        }
                        else
                        {
                            cerr << "Customer with this is id is not in the database" << endl;
                        }
                    }
                    else if (option1 == 9)
                    {

                        int id;
                        cout << "Enter the employee's id of the employee whom you want to delete: " << endl;
                        cin >> id;

                        if (EmpObjList.find(id) != EmpObjList.end())
                        {

                            ManagerList[Manager_id].DeletePerson_details(id, 'E', List_employee);
                        }
                        else
                        {
                            cerr << "Employee with this is id is not in the database" << endl;
                        }
                    }
                    else if (option1 == 10)
                    {

                        print_available_cars(0);

                        cout << endl;
                    }
                    else if (option1 == 11)
                    {

                        ManagerList[Manager_id].SeeRented_Cars(carOfCust);
                        cout << endl;
                    }
                    else if (option1 == 12)
                    {

                        ManagerList[Manager_id].SeeRented_Cars(carOfEmp);
                        cout << endl;
                    }

                    else if (option1 == 13)
                    {
                        x = 1;
                        break;
                    }
                    else
                    {
                        cerr << "Invalid Input" << endl;
                        break;
                    }
                }
            }
        }

        else if (choice == 0)
        {
            cout << "Exiting :)" << endl;
            y = 0;
            break;
        }

        else
        {
            cerr << "Invalid Input!!" << endl;
            break;
        }
    }
    return 0;
}