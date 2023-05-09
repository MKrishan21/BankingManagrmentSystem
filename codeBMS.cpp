#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>

using namespace std;

class account
{
    int accountNumber;
    char name[50];
    int deposit;
    char type;

public:
    void createAccount();
    void showAccount() const;
    void modifyAccount();
    void depositMoney(int);
    void withdrawMoney(int);
    void generateReport() const;
    int getAccountNumber() const;
    int getDepositAmoumnt() const;
    char getAccountType() const;
};

void account::createAccount()
{
    system("clear");
    cout << "\nEnter the account number : ";
    cin >> accountNumber;
    cout << "\n\nEnter the name of the account holder : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nEnter type of the account (Current(C)/Saving(S)) : ";
    cin >> type;
    type = toupper(type);
    cout << "\nEnter the initial amount : ";
    cin >> deposit;
    cout << "\n\nUser account created.";
}

void account::showAccount() const
{
    cout << "\nAccount number : " << accountNumber;
    cout << "\nAccount holder name : ";
    cout << name;
    cout << "\nType of account : " << type;
    cout << "\nBalance amount : " << deposit;
}

void account::modifyAccount()
{
    cout << "\nAccount number : " << accountNumber;
    cout << "\nModify account holder name : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nModify type of account : ";
    cin >> type;
    type = toupper(type);
    cout << "\nModify balance amount : ";
    cin >> deposit;
}

void account::depositMoney(int x)
{
    deposit += x;
}

void account::withdrawMoney(int x)
{
    deposit -= x;
}

void account::generateReport() const
{
    cout << accountNumber << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}

int account::getAccountNumber() const
{
    return accountNumber;
}

int account::getDepositAmoumnt() const
{
    return deposit;
}

char account::getAccountType() const
{
    return type;
}

void write_account()
{
    account ac;
    ofstream outFile;
    outFile.open("emp-database.dat", ios::binary | ios::app);
    ac.createAccount();
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
    outFile.close();
}

void display_sp(int n)
{
    account ac;
    bool flag = false;
    ifstream inFile;
    inFile.open("emp-database.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\nBALANCE DETAILS\n";
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
    {
        if (ac.getAccountNumber() == n)
        {
            ac.showAccount();
            flag = true;
        }
    }
    inFile.close();
    if (flag == false)
        cout << "\n\nAccount number does not exist";
}

void modify_account(int n)
{
    bool found = false;
    account ac;
    fstream File;
    File.open("emp-database.dat", ios::binary | ios::in | ios::out);
    if (!File)
    {
        cout << "Database can't be accessed!, Please press any key to continue!";
        return;
    }
    while (!File.eof() && found == false)
    {
        File.read(reinterpret_cast<char *>(&ac), sizeof(account));
        if (ac.getAccountNumber() == n)
        {
            ac.showAccount();
            cout << "\n\nEnter the new details of account" << endl;
            ac.modifyAccount();
            int pos = (-1) * static_cast<int>(sizeof(account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\nRecord updated";
            found = true;
        }
    }
    File.close();
    if (found == false)
        cout << "\n\nRecord Not Found ";
}

void delete_account(int n)
{
    account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("emp-database.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    outFile.open("temporary.dat", ios::binary);
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
    {
        if (ac.getAccountNumber() != n)
        {
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(account));
        }
    }
    inFile.close();
    outFile.close();
    remove("emp-database.dat");
    rename("temporary.dat", "emp-database.dat");
    cout << "\n\nRecord deleted ..";
}

void display_all()
{
    system("clear");
    account ac;
    ifstream inFile;
    inFile.open("emp-database.dat", ios::binary);
    if (!inFile)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\n\nACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type    Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(account)))
    {
        ac.generateReport();
    }
    inFile.close();
}

void deposit_withdraw(int n, int option)
{
    int amt;
    bool found = false;
    account ac;
    fstream File;
    File.open("emp-database.dat", ios::binary | ios::in | ios::out);
    if (!File)
    {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (!File.eof() && found == false)
    {
        File.read(reinterpret_cast<char *>(&ac), sizeof(account));
        if (ac.getAccountNumber() == n)
        {
            ac.showAccount();
            if (option == 1)
            {
                cout << "\n\nTO DEPOSITS AMOUNT";
                cout << "\n\nEnter The amount to be deposited: ";
                cin >> amt;
                ac.depositMoney(amt);
            }
            if (option == 2)
            {
                cout << "\n\nTO WITHDRAW AMOUNT";
                cout << "\n\nEnter The amount to be withdraw: ";
                cin >> amt;
                int bal = ac.getDepositAmoumnt() - amt;
                if (bal < 0)
                    cout << "Insufficent balance";
                else
                    ac.withdrawMoney(amt);
            }
            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur); // fn1353
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\nRecord Updated";
            found = true;
        }
    }
    File.close();
    if (found == false)
        cout << "\n\nRecord Not Found ";
}

int main()
{
    char ch;
    int num;
    do
    {
        system("clear");
        cout << "\n\n======================\n";
        cout << "BANK MANAGEMENT SYSTEM";
        cout << "\n======================\n";

        cout << "    ::MAIN MENU::\n";
        cout << "\n1. NEW ACCOUNT";
        cout << "\n2. DEPOSIT AMOUNT";
        cout << "\n3. WITHDRAW AMOUNT";
        cout << "\n4. BALANCE ENQUIRY";
        cout << "\n5. ALL ACCOUNT HOLDER LIST";
        cout << "\n6. CLOSE AN ACCOUNT";
        cout << "\n7. MODIFY AN ACCOUNT";
        cout << "\n8. EXIT";
        cout << "\n\nSelect Your Option (1-8): ";
        cin >> ch;

        switch (ch)
        {
        case '1':
            write_account();
            break;
        case '2':
            system("clear");
            cout << "\n\nEnter The account No. : ";
            cin >> num;
            deposit_withdraw(num, 1);
            break;
        case '3':
            system("clear");
            cout << "\n\nEnter The account No. : ";
            cin >> num;
            deposit_withdraw(num, 2);
            break;
        case '4':
            system("clear");
            cout << "\n\nEnter The account No. : ";
            cin >> num;
            display_sp(num);
            break;
        case '5':
            display_all();
            break;
        case '6':
            system("clear");
            cout << "\n\nEnter The account No. : ";
            cin >> num;
            delete_account(num);
            break;
        case '7':
            system("clear");
            cout << "\n\nEnter The account No. : ";
            cin >> num;
            modify_account(num);
            break;
        default:
            cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (ch != '8');
    return 0;
}