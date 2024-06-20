#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

class Book {
    char bno[6]; // Book number
    char bname[50]; // Book name
    char aname[20]; // Author name
    
public:
    void createBook() {
        cout << "\nNEW BOOK ENTRY...\n";
        cout << "\nEnter Book No.: ";
        cin >> bno;
        cout << "Enter Book Name: ";
        cin.ignore();
        cin.getline(bname, 50);
        cout << "Enter Author Name: ";
        cin.getline(aname, 20);
        cout << "\nBook Created Successfully.";
    }
    
    void showBook() const {
        cout << "\nBook Number: " << bno;
        cout << "\nBook Name: " << bname;
        cout << "\nAuthor Name: " << aname << endl;
    }
    
    const char* getBno() const {
        return bno;
    }
    
    void modifyBook() {
        cout << "\nBook Number: " << bno;
        cout << "\nEnter New Book Name: ";
        cin.ignore();
        cin.getline(bname, 50);
        cout << "Enter New Author Name: ";
        cin.getline(aname, 20);
        cout << "\nBook Details Modified.";
    }
    
    void report() const {
        cout << setw(6) << left << bno << setw(50) << left << bname << setw(20) << left << aname << endl;
    }
};

class Student {
    char admno[6]; // Admission number
    char name[20]; // Student name
    char stbno[6]; // Student's issued book number
    int token; // Whether book issued (1) or not (0)
    
public:
    void createStudent() {
        cout << "\nNEW STUDENT ENTRY...\n";
        cout << "\nEnter Admission No.: ";
        cin >> admno;
        cout << "Enter Student Name: ";
        cin.ignore();
        cin.getline(name, 20);
        token = 0;
        stbno[0] = '\0';
        cout << "\nStudent Record Created Successfully.";
    }
    
    void showStudent() const {
        cout << "\nAdmission Number: " << admno;
        cout << "\nStudent Name: " << name;
        cout << "\nBooks Issued: " << token;
        if (token == 1) {
            cout << "\nBook Number: " << stbno;
        }
    }
    
    const char* getAdmno() const {
        return admno;
    }
    
    const char* getStbno() const {
        return stbno;
    }
    
    int getToken() const {
        return token;
    }
    
    void addToken(const char* bn) {
        token = 1;
        strcpy(stbno, bn);
    }
    
    void resetToken() {
        token = 0;
        stbno[0] = '\0';
    }
    
    void modifyStudent() {
        cout << "\nAdmission Number: " << admno;
        cout << "\nEnter New Student Name: ";
        cin.ignore();
        cin.getline(name, 20);
        cout << "\nStudent Details Modified.";
    }
    
    void report() const {
        cout << setw(6) << left << admno << setw(20) << left << name << setw(10) << left << token << endl;
    }
};

fstream fp, fp1;
Book bk;
Student st;

void writeBook() {
    char ch;
    fp.open("book.dat", ios::out | ios::app);
    do {
        bk.createBook();
        fp.write(reinterpret_cast<char*>(&bk), sizeof(Book));
        cout << "\n\nDo you want to add more records? (y/n): ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fp.close();
}

void writeStudent() {
    char ch;
    fp.open("student.dat", ios::out | ios::app);
    do {
        st.createStudent();
        fp.write(reinterpret_cast<char*>(&st), sizeof(Student));
        cout << "\n\nDo you want to add more records? (y/n): ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fp.close();
}

void displaySpecificBook(const char* n) {
    bool found = false;
    fp.open("book.dat", ios::in);
    while (fp.read(reinterpret_cast<char*>(&bk), sizeof(Book))) {
        if (strcmp(bk.getBno(), n) == 0) {
            bk.showBook();
            found = true;
            break;
        }
    }
    fp.close();
    if (!found) {
        cout << "\nBook not found.\n";
    }
}

void displaySpecificStudent(const char* n) {
    bool found = false;
    fp.open("student.dat", ios::in);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (strcmp(st.getAdmno(), n) == 0) {
            st.showStudent();
            found = true;
            break;
        }
    }
    fp.close();
    if (!found) {
        cout << "\nStudent not found.\n";
    }
}

void modifyBook() {
    char n[6];
    bool found = false;
    cout << "\nEnter Book Number to Modify: ";
    cin >> n;
    fp.open("book.dat", ios::in | ios::out);
    while (fp.read(reinterpret_cast<char*>(&bk), sizeof(Book)) && !found) {
        if (strcmp(bk.getBno(), n) == 0) {
            bk.showBook();
            bk.modifyBook();
            int pos = -static_cast<int>(sizeof(Book));
            fp.seekp(pos, ios::cur);
            fp.write(reinterpret_cast<char*>(&bk), sizeof(Book));
            cout << "\nBook details updated.\n";
            found = true;
        }
    }
    fp.close();
    if (!found) {
        cout << "\nBook not found.\n";
    }
}

void modifyStudent() {
    char n[6];
    bool found = false;
    cout << "\nEnter Admission Number to Modify: ";
    cin >> n;
    fp.open("student.dat", ios::in | ios::out);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student)) && !found) {
        if (strcmp(st.getAdmno(), n) == 0) {
            st.showStudent();
            st.modifyStudent();
            int pos = -static_cast<int>(sizeof(Student));
            fp.seekp(pos, ios::cur);
            fp.write(reinterpret_cast<char*>(&st), sizeof(Student));
            cout << "\nStudent details updated.\n";
            found = true;
        }
    }
    fp.close();
    if (!found) {
        cout << "\nStudent not found.\n";
    }
}

void deleteBook() {
    char n[6];
    bool found = false;
    cout << "\nEnter Book Number to Delete: ";
    cin >> n;
    fp.open("book.dat", ios::in);
    fp1.open("temp.dat", ios::out);
    while (fp.read(reinterpret_cast<char*>(&bk), sizeof(Book))) {
        if (strcmp(bk.getBno(), n) != 0) {
            fp1.write(reinterpret_cast<char*>(&bk), sizeof(Book));
        } else {
            found = true;
        }
    }
    fp.close();
    fp1.close();
    if (found) {
        remove("book.dat");
        rename("temp.dat", "book.dat");
        cout << "\nBook deleted.\n";
    } else {
        cout << "\nBook not found.\n";
    }
}

void deleteStudent() {
    char n[6];
    bool found = false;
    cout << "\nEnter Admission Number to Delete: ";
    cin >> n;
    fp.open("student.dat", ios::in);
    fp1.open("temp.dat", ios::out);
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        if (strcmp(st.getAdmno(), n) != 0) {
            fp1.write(reinterpret_cast<char*>(&st), sizeof(Student));
        } else {
            found = true;
        }
    }
    fp.close();
    fp1.close();
    if (found) {
        remove("student.dat");
        rename("temp.dat", "student.dat");
        cout << "\nStudent deleted.\n";
    } else {
        cout << "\nStudent not found.\n";
    }
}

void displayAllBooks() {
    fp.open("book.dat", ios::in);
    if (!fp) {
        cout << "Error opening file.\n";
        return;
    }
    cout << "\n\n\t\tBook List\n\n";
    cout << setw(6) << left << "Book No." << setw(50) << left << "Book Name" << setw(20) << left << "Author Name" << endl;
    while (fp.read(reinterpret_cast<char*>(&bk), sizeof(Book))) {
        bk.report();
    }
    fp.close();
}

void displayAllStudents() {
    fp.open("student.dat", ios::in);
    if (!fp) {
        cout << "Error opening file.\n";
        return;
    }
    cout << "\n\n\t";
cout << "\t\tStudent List\n\n";
    cout << setw(6) << left << "Adm No." << setw(20) << left << "Student Name" << setw(10) << left << "Issued" << endl;
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student))) {
        st.report();
    }
    fp.close();
}

void issueBook() {
    char sn[6], bn[6];
    bool foundS = false, foundB = false;
    cout << "\nEnter Admission No.: ";
    cin >> sn;
    
    fp.open("student.dat", ios::in | ios::out);
    fp1.open("book.dat", ios::in | ios::out);
    
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student)) && !foundS) {
        if (strcmp(st.getAdmno(), sn) == 0) {
            foundS = true;
            if (st.getToken() == 0) {
                cout << "Enter Book No.: ";
                cin >> bn;
                
                while (fp1.read(reinterpret_cast<char*>(&bk), sizeof(Book)) && !foundB) {
                    if (strcmp(bk.getBno(), bn) == 0) {
                        foundB = true;
                        st.addToken(bk.getBno());
                        
                        int pos = -static_cast<int>(sizeof(st));
                        fp.seekp(pos, ios::cur);
                        fp.write(reinterpret_cast<char*>(&st), sizeof(Student));
                        
                        cout << "\nBook Issued Successfully.\n";
                    }
                }
                if (!foundB) {
                    cout << "Book not found.\n";
                }
            } else {
                cout << "Student has already issued a book.\n";
            }
        }
    }
    
    fp.close();
    fp1.close();
    
    if (!foundS) {
        cout << "Student not found.\n";
    }
}

void depositBook() {
    char sn[6], bn[6];
    bool foundS = false, foundB = false;
    int days, fine;
    cout << "\nEnter Admission No.: ";
    cin >> sn;
    
    fp.open("student.dat", ios::in | ios::out);
    fp1.open("book.dat", ios::in | ios::out);
    
    while (fp.read(reinterpret_cast<char*>(&st), sizeof(Student)) && !foundS) {
        if (strcmp(st.getAdmno(), sn) == 0) {
            foundS = true;
            if (st.getToken() == 1) {
                while (fp1.read(reinterpret_cast<char*>(&bk), sizeof(Book)) && !foundB) {
                    if (strcmp(bk.getBno(), st.getStbno()) == 0) {
                        foundB = true;
                        bk.showBook();
                        
                        cout << "\nEnter number of days book was issued for: ";
                        cin >> days;
                        
                        if (days > 15) {
                            fine = (days - 15) * 1;
                            cout << "\nFine to be paid: $" << fine << endl;
                        }
                        
                        st.resetToken();
                        
                        int pos = -static_cast<int>(sizeof(st));
                        fp.seekp(pos, ios::cur);
                        fp.write(reinterpret_cast<char*>(&st), sizeof(Student));
                        
                        cout << "\nBook Deposited Successfully.\n";
                    }
                }
                if (!foundB) {
                    cout << "Book not found.\n";
                }
            } else {
                cout << "Student has not issued any book.\n";
            }
        }
    }
    
    fp.close();
    fp1.close();
    
    if (!foundS) {
        cout << "Student not found.\n";
    }
}

void start() {
    cout << "\n\n\n\t\tLIBRARY MANAGEMENT SYSTEM";
    cout << "\n\n\t\tBY: Chahat\n";
    cout << "\nPress any key to continue...";
    cin.get();
}

void adminMenu() {
    char ch;
    start();
    
    do {
        system("cls");
        cout << "\n\n\n\tADMINISTRATOR MENU";
        cout << "\n\n\t1. CREATE STUDENT RECORD";
        cout << "\n\n\t2. DISPLAY ALL STUDENTS RECORD";
        cout << "\n\n\t3. DISPLAY SPECIFIC STUDENT RECORD";
        cout << "\n\n\t4. MODIFY STUDENT RECORD";
        cout << "\n\n\t5. DELETE STUDENT RECORD";
        cout << "\n\n\t6. CREATE BOOK";
        cout << "\n\n\t7. DISPLAY ALL BOOKS";
        cout << "\n\n\t8. DISPLAY SPECIFIC BOOK";
        cout << "\n\n\t9. MODIFY BOOK RECORD";
        cout << "\n\n\t10. DELETE BOOK RECORD";
        cout << "\n\n\t11. BACK TO MAIN MENU";
        cout << "\n\n\tPlease enter your choice (1-11): ";
        cin >> ch;
        
        switch (ch) {
            case '1':
                system("cls");
                writeStudent();
                break;
            case '2':
                system("cls");
                displayAllStudents();
                break;
            case '3': {
                system("cls");
                char num[6];
                cout << "\n\n\tEnter Admission No.: ";
                cin >> num;
                displaySpecificStudent(num);
                break;
            }
            case '4':
                system("cls");
                modifyStudent();
                break;
            case '5':
                system("cls");
                deleteStudent();
                break;
            case '6':
                system("cls");
                writeBook();
                break;
            case '7':
                system("cls");
                displayAllBooks();
                break;
            case '8': {
                system("cls");
                char num[6];
                cout << "\n\n\tEnter Book No.: ";
                cin >> num;
                displaySpecificBook(num);
                break;
            }
            case '9':
                system("cls");
                modifyBook();
                break;
            case '10':
                system("cls");
                deleteBook();
                break;
            case '11':
                break; // This will exit the do-while loop and return to main menu
            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }
        
        if (ch != '11') {
            cout << "\n\nPress any key to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while (ch != '11');
}


void userMenu() {
    char ch;
    
    do {
        system("cls");
        cout << "\n\n\n\tUSER MENU";
        cout << "\n\n\t1. ISSUE BOOK";
        cout << "\n\n\t2. DEPOSIT BOOK";
        cout << "\n\n\t3. BACK TO MAIN MENU";
        cout << "\n\n\tPlease enter your choice (1-3): ";
        cin >> ch;
        
        switch (ch) {
            case '1':
                system("cls");
                issueBook();
                break;
            case '2':
                system("cls");
                depositBook();
                break;
            case '3':
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }
        
        cout << "\n\nPress any key to continue...";
        cin.ignore();
        cin.get();
        
    } while (ch != '3');
}

int main() {
    char ch;
    start();
    
    do {
        system("cls");
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t1. ADMINISTRATOR MENU";
        cout << "\n\n\t2. USER MENU";
        cout << "\n\n\t3. EXIT";
        cout << "\n\n\tPlease select your option (1-3): ";
        cin >> ch;
        
        switch (ch) {
            case '1':
                system("cls");
                adminMenu();
                break;
            case '2':
                system("cls");
                userMenu();
                break;
            case '3':
                cout << "\nExiting program...\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }
        
        cout << "\n\nPress any key to continue...";
        cin.ignore();
        cin.get();
        
    } while (ch != '3');
    
    return 0;
}
