#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
using namespace std;

// forward declaration of the classes.
class pay_slip;
class permanent_employee;
class contractual_employee;

// function prototypes.

int get_da_rate();
int get_hra_rate();
int get_ma();
int get_ptax();
int get_emp_no();
int search(int, permanent_employee *);
int search(int, contractual_employee *);
void generate_pay_slip(permanent_employee);
void generate_pay_slip(contractual_employee);
void compute_OT();

class employee
{
    // base class.

protected:
    int emp_no;
    char emp_name[40];
    char emp_add[80];
    char emp_desg[20];
    char emp_dept[20];

public:
    void getdata();
    void displaydata();
}; // end of base class definitions.

void employee::getdata()
{
    emp_no = get_emp_no();
    cout << "\nEnter the name of the employee:";
    cin.getline(emp_name, 40);
    cout << "\nEnter the address of the employee:";
    cin.getline(emp_add, 80);
    cout << "\nEnter the dsignation of the employee";
    cin.getline(emp_desg, 20);
    cout << "\nEnter the department of the employee";
    cin.getline(emp_dept, 20);
}

void employee::displaydata()
{
    cout << "\nEmp. No.: " << emp_no;
    cout << "\nName: " << emp_name;
    cout << "\nAddress: " << emp_add;
    cout << "\nDesignation: " << emp_desg;
    cout << "\nDepartment: " << emp_dept;
}

class permanent_employee : public employee
{
    // derived from base class

private:
    char emp_type[10];
    long emp_basic;
    long emp_da;
    long emp_hra;
    int emp_ma;
    long emp_pf;
    int ptax;
    int itax;
    long gross;
    long net;

public:
    void getdata();     // over-ridden function
    void displaydata(); // over ridden function
    void calculate();
    void store_in_file();
    friend int search(int, permanent_employee *);
    friend void generate_pay_slip(permanent_employee);
    friend void compute_OT();
};

void permanent_employee::getdata()
{
    employee::getdata(); // calling base getdata().

    try
    {
        // exception handling for negative basic.

        cout << "\nEnter the basic salary:";
        cin >> emp_basic;
        if (emp_basic < 0)
        {
            throw emp_basic;
        }
    }
    catch (...)
    {
        cout << "\nBasic should be positive";
        return;
    }
    try
    {
        /// exception handling for negative income tax.
        cout << "\nEnter the income tax to be deducted:";
        cin >> itax;
        if (itax < 0)
        {
            throw itax;
        }
    }
    catch (...)
    {
        cout << "\n Income Tax should be positive";
        return;
    }
    calculate();
}

void permanent_employee::calculate()
{
    int da, hra;

    strcpy(emp_type, "Permanent");
    da = get_da_rate();
    emp_da = emp_basic * da / 100; // da is calculated as a percentage of basic

    hra = get_hra_rate();
    emp_hra = emp_basic * hra / 100; // hra is calculated as a percentage of basic.

    emp_ma = get_ma();
    emp_pf = (emp_basic + emp_da) * 12 / 100; // pf is calculated as a percentage of basic + da.

    ptax = get_ptax();
    gross = emp_basic + emp_da + emp_hra + emp_ma; // calc. of gross
    net = gross - ptax - itax - emp_pf;            // calc net.
}

void permanent_employee::displaydata()
{
    employee::displaydata(); // calling base displaydata().

    cout << "\nEmployee type: " << emp_type;
    cout << "\nBasic: " << emp_basic;
    cout << "\nDearness Allowance: " << emp_da;
    cout << "\nHRA: " << emp_hra;
    cout << "\nMedical Allowance: " << emp_ma;
    cout << "\nPF:" << emp_pf;
    cout << "\nProfessional tax: " << ptax;
    cout << "\nIncome tax: " << itax;
    cout << "\nGross Salary: " << gross;
    cout << "\nNet Salary:" << net;
}

void permanent_employee::store_in_file()
{
    ofstream outfile;
    outfile.open("per_empf", ios::app); // open per_empf in append mode. // file per_empf stores details of permanent employees.
    outfile << emp_no << "\n";
    outfile << emp_name << "\n"
            << emp_add << "\n"
            << emp_desg << "\n";
    outfile << emp_dept << "\n"
            << emp_type << "\n"
            << emp_basic << "\n"
            << emp_da << "\n"
            << emp_hra << "\n"
            << emp_ma << "\n"
            << emp_pf << "\n"
            << ptax << "\n"
            << itax << "\n"
            << gross << "\n"
            << net << "\n";
    outfile.close(); // close file.
}

class contractual_employee : public employee
{
    // derived from base employee

private:
    char emp_type[12];
    long gross;
    int ptax;
    int itax;
    long net;

public:
    void getdata();
    void displaydata();
    void calculate();
    void store_in_file();
    friend int search(int, contractual_employee *);
    friend void generate_pay_slip(contractual_employee);
    friend void compute_OT();
};

void contractual_employee::getdata()
{
    // over-riddern function getdata()

    try
    {
        // exception for negative gross.
        cout << "\nEnter the gross salary: ";
        cin >> gross;
        if (gross < 0)
        {
            throw gross;
        }
    }
    catch (...)
    {
        cout << "\nGorss salary should be positive";
        return;
    }
    try
    {
        // exception for negative income tax

        cout << "\nEnter the income tax salary: ";
        cin >> itax;
        if (itax < 0)
        {
            throw itax;
        }
    }
    catch (...)
    {
        cout << "\nGross Income tax should be positive";
        return;
    }
    calculate();
}

void contractual_employee::calculate()
{
    strcpy(emp_type, "Contractual");
    ptax = get_ptax();
    net = gross - ptax - itax;
}

void contractual_employee::displaydata()
{
    // over-ridden displaydata()

    employee::displaydata(); // calling base displaydata()
    cout << "\nEmployee Type: " << emp_type;
    cout << "\nGross Salary: " << gross;
    cout << "\nProfessional tax: " << ptax;
    cout << "\nIncome tax: " << itax;
    cout << "\nNet Salary: " << net;
}

void contractual_employee::store_in_file()
{
    ofstream outfile;
    outfile.open("con_empf", ios::app); // open file in append mode. // file con_empf stores details of contractual employees.
    outfile << emp_no << "\n";
    outfile << emp_name << "\n"
            << emp_add << "\n"
            << emp_desg << "\n";
    outfile << emp_dept << "\n"
            << emp_type << "\n"
            << gross << "\n"
            << ptax << "\n"
            << itax << "\n"
            << net << "\n";
    outfile.close(); // close files.
}

int search(int no, permanent_employee *per)
{
    // func. defn. for searching permanent employees from file.

    ifstream infile;
    permanent_employee p;

    char ch;
    infile.open("per_empf"); // open file in read mode.
    if (!infile.eof())
    {
        // while not end of file.

        infile >> p.emp_no;
        infile.ignore(1000, '\n');
        infile.getline(p.emp_name, 40);
        infile.getline(p.emp_add, 80);
        infile.getline(p.emp_desg, 20);
        infile.getline(p.emp_dept, 20);
        infile >> p.emp_type >> p.emp_basic >> p.emp_da >> p.emp_hra >> p.emp_ma >> p.emp_pf >> p.ptax >> p.itax >> p.gross >> p.net;
        if (no == p.emp_no)
        {
            // looks for a match
            *per = p; // set pointer to object if found
            return 1;
        }
    }
    return 0; // return false if not found
}

int search(int no, contractual_employee *con)
{
    // func. defn. for searching contractual employees from file.

    ifstream infile;
    contractual_employee p;
    infile.open("con_empf"); // open file in read mode
    if (!infile.fail())      // checking if file exists.
    {
        while (!infile.eof())
        {
            // while not end of file
            infile >> p.emp_no;
            infile.ignore(1000, '\n');
            infile.getline(p.emp_name, 40);
            infile.getline(p.emp_add, 80);
            infile.getline(p.emp_desg, 20);
            infile.getline(p.emp_dept, 20);
            infile >> p.emp_type >> p.gross >> p.ptax >> p.itax >> p.net;
            if (no == p.emp_no)
            {
                // looks for a match
                *con = p; // set pointer to object if found.
                return 1; // return true.
            }
        }
        return 0; // return false if not found.
    }
}

void generate_pay_slip(permanent_employee per)
{
    // function to generate pay slips of permanent employees
    int no;
    char fl_name[8], num[8], emp[8], c;

    cout << "\n\nEnter month no (1..12):   ";
    cin >> no;
    if (no > 12 || no < 0)
    {
        cout << "\n\nInvalid month number";
        return;
    }
    system("clear");
    cout << ": : : : : : : : : : : : : : Pay Slilf for Month number" << no << ": : : : : : : : : : : : :";
    per.calculate();   // perform calculation.
    per.displaydata(); // display data about employee.
    // create file name to store pay slip of employee by concatenation employee no. with month no. with an underscore character in between.
    sprintf(num, "%d_", no);
    sprintf(emp, "%d", per.emp_no);
    strcpy(fl_name, num);
    strcat(fl_name, emp);
    fl_name[strlen(num) + strlen(emp)] = '\0';
    ofstream outfile(fl_name);
    // store pay slip in file.

    outfile << ": : : : : : : : : : : : Pay Slip for Month number" << no << ": : : : : : : : : : :\n\n";
    outfile << "Emp. Number:  " << per.emp_no << "\n";
    outfile << "Emp. Name:   " << per.emp_name << "\n";
    outfile << "Emp. Add:   " << per.emp_add << "\n";
    outfile << "Emp. Designation:   " << per.emp_desg << "\n";
    outfile << "Emp. Department:   " << per.emp_dept << "\n";
    outfile << "Emp. Type:    " << per.emp_type << "\n"
            << "Emp. Basic:   " << per.emp_basic << "\n"
            << "Dearness Allowance:  " << per.emp_da << "\n"
            << "House Rent Allowance:   " << per.emp_hra << "\n"
            << "Medical Allowance:  " << per.emp_ma << "\n"
            << "Provident Fund:   " << per.emp_pf << "\n"
            << "Professional tax:   " << per.ptax << "\n"
            << "Income tax:  " << per.itax << "\n"
            << "Gross Salary:  " << per.gross << "\n"
            << "Net Salary:   " << per.net << "\n";
    outfile.close();
}

void generate_pay_slip(contractual_employee per)
{
    // function to generate  pay slips of contractual employees
    int no;
    char fl_name[8], num[8], emp[8], c;
    cout << "\n\nEnter month no (1..12):  ";
    cin >> no;
    if (no > 12 || no < 0)
    {
        cout << "\n\nInvalid month number";
        return;
    }
    system("clear");

    per.calculate();   // perform calculation
    per.displaydata(); // display details of contractual employee
    // create file name to store pay slip of contractual employee by concatenation employee no. with month no. with an underscore character in between

    sprintf(num, "%d_", no);
    sprintf(emp, "%d", per.emp_no);
    strcpy(fl_name,num);
    strcat(fl_name, emp);
    fl_name[strlen(num)+strlen(emp)]='\0';
    ofstream outfile(fl_name); // store pay slip  in file.

    outfile<<"::::::::::::::::: Pay Slip for Month number  "<<no<<":::::::::::::::";
    outfile<<"Emp. Number:  "<< per.emp_no<<"\n";
    outfile<<"Emp. Name:  "<< per.emp_name<<"\n";
    outfile<<"Emp. Add:  "<< per.emp_add<<"\n";
    outfile<<"\nEmp. Designation:  "<< per.emp_desg<<"\n";
    outfile<<"Emp. Department:  "<< per.emp_dept<<"\n";
    outfile<<"Emp. Type:  "<< per.emp_type<<"\n"<<"Gross Salary: "<<per.gross<<"\n"<<"Professional Tax:  "<<per.ptax<<"\n"<<"Income Tax:  "<<per.itax<<"\n"<<"Net Salary:  "<<per.net<<"\n";

    outfile.close();
}


void compute_OT(){
    //function to compute overtime dues.
    ifstream infile;
    permanent_employee p;
    contractual_employee c;
    int hours;
    ofstream outfile;
    infile.open("per_empf");
    outfile.open("per_payroll");
    system("clear");
    cout<<"\n\nComputation of overtime dues for permanent employee in progress..............";
    while(!infile.eof()){
        infile>>p.emp_no;
        infile.ignore(1000,'\n');
        infile.getline(p.emp_name, 40);
        infile.getline(p.emp_add, 80);
        infile.getline(p.emp_desg, 20);
        infile.getline(p.emp_dept, 20);
        infile>>p.emp_type>>p.emp_basic>>p.emp_da>>p.emp_hra>>p.emp_ma>>p.emp_pf>>p.ptax>>p.itax>>p.gross>>p.net;
        if(infile.eof())// needed to check if eof flag has been set else last employee details will be read twic.
            break;
        cout<<"\n Enter the overtime hours for employee no .   "<<p.emp_no<<" : ";
        cin>>hours;
        p.net=p.net + (hours*400); // over time rate is 400 per hours.
        outfile<<p.emp_no<<"\n";
        outfile<<p.emp_name<<"\n"<<p.emp_add<<"\n"<<p.emp_desg<<"\n";
        outfile<<p.emp_dept<<"\n"<<p.emp_type<<"\n"<<p.emp_basic<<"\n"<<p.emp_da<<"\n"<<p.emp_hra<<"\n"<<p.emp_ma<<"\n"<<p.emp_pf<<"\n"<<p.ptax<<"\n"<<p.itax<<"\n"<<p.gross<<"\n"<<hours*400<<"\n"<<p.net<<"\n";
    }
    infile.close();
    outfile.close();
    cout<<"\nOvertime dues computation for permanent employees complete";
    cin.get();
    infile.open("con_empf");
    outfile.open("con_payroll");
    system("clear");
    cout<<"\n\n Computation of overtime dues for contractual employee in progrss..........";
    while(!(infile.eof())){
        infile>>c.emp_no;
        infile.ignore(1000,'\n');
        infile.getline(c.emp_name, 40);
        infile.getline(c.emp_add, 80);
        infile.getline(c.emp_desg,20);
        infile.getline(c.emp_dept,20);
        infile>>c.emp_type>>c.gross>>c.ptax>>c.itax>>c.net;
        if(infile.eof())
            break;
        cout<<"\nEnter the overtime hours for employee no. "<<c.emp_no<<" : ";
        cin>>hours;
        c.net = c.net + (hours*400);
        outfile<<c.emp_no<<"\n";
        outfile<<c.emp_name<<"\n"<<c.emp_add<<"\n"<<c.emp_desg<<"\n";
        outfile<<c.emp_dept<<"\n"<<c.emp_type<<"\n"<<c.gross<<"\n"<<c.ptax<<"\n"<<c.itax<<"\n"<<hours*400<<"\n"<<c.net<<"\n"; 
    }
    infile.close();
    outfile.close();
    cout<<"\nOvertime Dues computation for contractual employee complete";
    cin.get();
    system("clear");
    cout<<"\nThe Overtime dues for the employees are as follows: \n\n";
    infile.open("per_payroll");
    cout.setf(ios::left, ios::adjustfield); //display output in formatted fashion
    cout.width(10);
    cout<<"Emp No.";
    cout.setf(ios::left,ios::adjustfield);
    cout.width(40);
    cout<<"Emp. Name";
    cout.setf(ios:: left, ios::adjustfield);
    cout.width(10);
    cout<<"Overtime Dues"<<"\n\n";
    while(!infile.eof()){
        infile>>p.emp_no;
        infile.ignore(1000,'\n');
        infile.getline(p.emp_name,40);
        infile.getline(p.emp_add,80);
        infile.getline(p.emp_desg, 20);
        infile.getline(p.emp_dept, 20);
        infile>>p.emp_type>>p.emp_basic>>p.emp_da>>p.emp_hra>>p.emp_ma>>p.emp_pf>>p.ptax>>p.itax>> p.gross>> hours>>p.net;
        if(infile.eof())//needed to avoid duplication of last employee details.
            break;
        cout.setf(ios::left, ios::adjustfield);
        cout.width(10);
        cout<<p.emp_no;
        cout.setf(ios::left, ios::adjustfield);
        cout.width(10);
        cout<<hours<<"\n";
    }
    infile.close();
    infile.open("con_payroll");
    while(!(infile.eof())){
        infile>>c.emp_no;
        infile.ignore(1000,'\n');
        infile.getline(p.emp_name,40);
        infile.getline(p.emp_add,80);
        infile.getline(p.emp_desg, 20);
        infile.getline(p.emp_dept, 20);
        infile>>c.emp_type>>c.gross>>c.ptax>>c.itax>>hours>>c.net;
        if(infile.eof())
            break;
        cout.setf(ios::left, ios::adjustfield);
        cout.width(10);
        cout<<c.emp_no;
        cout.setf(ios::left, ios::adjustfield);
        cout.width(40);
        cout<<c.emp_name;
        cout.width(10);
        cout<<hours<<"\n";
    }
    infile.close();
}

int get_da_rate(){
    //function to retrieve da rate from file
    int da;
    ifstream infile("da_file");
    infile>>da;
    infile.close();
    return da;

}
int get_hra_rate(){
    //function for retrieving hra rate from file.
    int hra;
    ifstream infile("hra_file");
    infile>>hra;
    infile.close();
    return hra;
}

int get_ma(){
    //function to retrieve medical allowance from file 
    int ma;
    ifstream infile("ma_file");
    infile>>ma;
    infile.close();
    return ma;

}

int get_ptax(){
    //function to retrieve professional tax from file.
    int ptax;
    ifstream infile("ptax_fl");
    infile>>ptax;
    infile.close();
    return ptax;
}

void set_da_rate(int da){
    // function to set the da rate 
    ofstream outfile("da_file");
    outfile<<da;
    outfile.close();
}

void set_hra_rate(int hra){
    //function to set the hra rate
    ofstream outfile("hra_file");
    outfile<<hra;
    outfile.close();
}

void set_ma(int ma){
    //function to set the medical allowance
    ofstream outfile("ma_file");
    outfile<<ma;
    outfile.close();
}


void set_ptax(int ptax){
    //function to set the  professional tax
    ofstream outfile("ptax_fl");
    outfile<<ptax;
    outfile.close();
}

int get_emp_no(){
    //function for auto-generating employee number
    ifstream infile;
    int no;
    infile.open("empno_fl");
    if(!infile.fail()){
        infile>>no;
        no = no+1;
        infile.close();
    }
    else no = 1;
    ofstream outfile("empno_fl");
    outfile<<no;
    outfile.close();
    return no;
}

int main(){
    int choice, emp_type, no;
    int da,hra, ma ,ptax;
    permanent_employee per_person;
    contractual_employee con_person;
    while(1){
        system('clear');
        cout<<"\n :::::::::::: MENU:::::::::::";
        cout<<"\n1. New Employee";
        cout<<"\n2. Generate Pay Slip";
        cout<<"\n3. Set Dearness Allowance Rate";
        cout<<"\n4. Set HRA Rate";
        cout<<"\n5. Set Professional tax";
        cout<<"\n6. Set Medical Allowance Rate";
        cout<<"\n7. Display Detail of Employee";
        cout<<"\n8. Compute Overtime Dues for Employees";
        cout<<"\n9. Exit";
        cout<<"\n\nEnter your choice:: ";
        cin>>choice;
        fflush(stdin);
        system("clear");
        switch(choice){
            case 1: // Entering new employee details.
                cout<<"\nEnter the employee type";
                cout<<"\n1. Permanent Employee";
                cout<<"\n2. Contractual Employee";
                cin>>emp_type;
                switch(emp_type){
                    case 1: // permanent employee
                        per_person.getdata();
                        per_person.store_in_file();
                        break;
                    case 2: // contractual employee.
                        con_person.getdata();
                        con_person.store_in_file();
                        break;
                    default: 
                        cout<<"\n\n Wrong type";
                        break;
                }
                break;
            case 2: // generate payslip
                cout<<"\n\nEnter Employee no.:  ";
                cin>>no;
                cout<<"\n\n Enter Employee type:   ";
                cout<<"\n1.   Permanent Employee";
                cout<<"\n2.   Contractual Employee";
                cin>>emp_type;
                switch(emp_type){
                    case 1: // for permanent employee
                        if((search(no, &per_person)) == 0){
                            cout<<"\n\n Invalid Employee number";
                        }
                        else generate_pay_slip(per_person);
                        break;
                    case 2: // for contractual employee
                        if((search(no, &con_person))==0){
                            cout<<"\n\n Invalid Employee number";
                        }
                        else generate_pay_slip(con_person);
                        break;
                    default:
                        cout<<"\n\nWrong type";
                        break;
                }
                break;
            case 3: // Set DA rate
                cout<<"\n\nEnter new Dearness Allowance rate:  ";
                cin>>da;
                set_da_rate(da);
                break;
            case 4: // Set HRA rate
                cout<<"\n\nEnter new HRA rate:   ";
                cin>>hra;
                set_hra_rate(hra);
                break;
            case 5: // Set Professional Tax
                cout<<"\n\n Enter new Professional Tax:  ";
                cin>>ptax;
                set_ptax(ptax);
                break;
            case 6: // Set Medical Allowance
                cout<<"\n\nEnter new Medical Allowance:   ";
                cin>>ma;
                set_ma(ma);
                break;
            case 7: // Search for Employee.
            cout<<"\n\nEnter Employee no.: ";
            cin>>no;
            cout<<"\n\n Enter Employee type:  ";
            cout<<"\n1. Permanent Employee";
            cout<<"\n2. Contractual Employee";
            cin>>emp_type;
            switch(emp_type){
                case 1: // Permanent Employee
                    if((search(no, &per_person))== 0){
                        cout<<"\n\nInvalid Employee number";
                    }
                    else per_person.displaydata();
                    break;
                case 2: // Contractual Employee
                    if((search(no, &con_person))==0){
                        cout<<"\n\nInvalid Employee number";
                    }
                    else con_person.displaydata();
                    break;
                default:
                    cout<<"\n\nWrong type";
                    break;
                }
                 break;
            case 8: // Compute Overtime dues for employees.
                compute_OT();
                break;
            case 9: // Exit System
                exit(0);
            default:
                cout<<"\n\nWrong choice";
        }// end of switch case for main menu
    }// End of While loop
    return 0;
}// End of Main function.
