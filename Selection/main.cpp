//
//  main.cpp
//  Selection
//
//  Created by Александр on 17.03.16.
//  Copyright © 2016 Alexander Besedin. All rights reserved.
//
//  Выборка людей по параметрам из базы данных с поддержкой логических операторов.


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Date {
public:
    unsigned short int day;
    unsigned short int month;
    unsigned short int year;
    
public: Date(unsigned short int _day, unsigned short int _month, unsigned short int _year) {
        day = _day;
        month = _month;
        year = _year;
    }
};

class Person {
public:
    int index;
    string firstName;
    string secondName;
    string patronymic;
    string fullName = secondName + " " + firstName + " " + patronymic;
    Date birthday = Date(0, 0, 0);
    string institute;
    Date issue = Date(0, 0, 0);
    
public: Person(int _index, string _firstName, string _secondName, string _patronymic, string _birtday, string _institute, string _issue) {
        index = _index;
        firstName = _firstName;
        secondName = _secondName;
        patronymic = _patronymic;
        institute = _institute;
        birthday = getDate(_birtday);
        issue = getDate(_issue);
    }
    
    private: Date getDate(string _date){
        unsigned short int day;
        unsigned short int month;
        unsigned short int year;
        string temp = "";
        int i = 0;
        char symb = _date[i];
        do {
            temp += symb;
            i++;
            symb = _date[i];
        } while (symb != '.');
        day = atoi(temp.c_str());
        temp = "";
        symb = _date[++i];
        do {
            temp += symb;
            i++;
            symb = _date[i];
        } while (symb != '.');
        month = atoi(temp.c_str());
        temp = "";
        symb = _date[++i];
        do {
            temp += symb;
            i++;
            symb = _date[i];
        } while (symb != '\0');
        year = atoi(temp.c_str());
        Date date = Date(day, month, year);
        return date;
    }
    
};

bool lessString(string str, string lessThatStr) {
    if (str.length() < lessThatStr.length()) {
        return false;
    }
    for (int i = 0; i < lessThatStr.length(); i++) {
        if (str[i] != lessThatStr[i]) {
            return false;
        }
    }
    return true;
}

template <typename T>
void operator += (vector<T> &vect1, vector<T> &vect2) {
    for (int i = 0; i < vect2.size(); i++) {
        vect1.push_back(vect2[i]);
    }
}

string getWord(ifstream *data, char *symbol) {
    string word;
    data->get(*symbol);
    do {
        word.push_back(*symbol);
        data->get(*symbol);
     } while (!data->eof() && (*symbol != '\n') && (*symbol != '\0') && (*symbol != ' '));
    return word;
}

void printPersons(vector<Person> outList) {
    for (int i = 0; i < outList.size(); i++) {
        Person person = outList[i];
        cout << person.firstName << " " << person.secondName << " " << person.patronymic << " День рождения: " <<person.birthday.day << "." << person.birthday.month << "." << person.birthday.year << " Институт: " << person.institute << " Дата окончания: " << person.issue.day << "." << person.issue.month << "." << person.issue.year << endl;
    }
}

void readPersonData (vector<Person> *_list) {
    ifstream data;
    data.open("/Users/maria/Documents/Xcode/Selection/Selection/data");
    string key, value;
    map<string, string> dict;
    char symbol = ' ';
    int count = 1;
    dict.insert(pair<string, string>("-fn", ""));
    dict.insert(pair<string, string>("-sn", ""));
    dict.insert(pair<string, string>("-pt", ""));
    dict.insert(pair<string, string>("-bd", ""));
    dict.insert(pair<string, string>("-it", ""));
    dict.insert(pair<string, string>("-is", ""));
    while (!data.eof()) {
        do {
            key = getWord(&data, &symbol);
            value = getWord(&data, &symbol);
            dict[key] = value;
        } while (symbol != '\n' && !data.eof());
        Person person = Person(count, dict["-fn"], dict["-sn"], dict["-pt"], dict["-bd"], dict["-it"], dict["-is"]);
        _list->push_back(person);
        count++;
    }
    data.close();
}

bool authorization(string _login, string _password) {
    ifstream data;
    data.open("/Users/maria/Documents/Xcode/Selection/Selection/Users");
    string login, password;
    char symbol = ' ';
    while (!data.eof()) {
        login = getWord(&data, &symbol);
        password = getWord(&data, &symbol);
        if (login == _login && password == _password) {
            data.close();
            return true;
        }
    }
    data.close();
    return false;
}

//  Selection of persons for one parameter.
//  _forSelect - a list on which the selection will be.
vector<Person> interimSelection(vector<Person> _forSelect, string _parameter, string key) {
    vector<Person> list;
    for (int i = 0; i < _forSelect.size(); i++) {
        if (_parameter == "-fn") {
            string firstName = _forSelect[i].firstName;
            if (firstName == key) {
                list.push_back(_forSelect[i]);
            }
            continue;
        }
        if (_parameter == "-sn") {
            if (_forSelect[i].secondName == key) {
                list.push_back(_forSelect[i]);
            }
            continue;
        }
        if (_parameter == "-pt") {
            if (_forSelect[i].patronymic == key) {
                list.push_back(_forSelect[i]);
            }
            continue;
        }
        if (lessString(_parameter, "-bd")) {
            unsigned short int intKey = atoi(key.c_str());
            bool existingDay = false, existingMonth = false, existingYear = false;
            if (_parameter == "-bdDD") {
                if (_forSelect[i].birthday.day == intKey) {
                    existingDay = true;
                    list.push_back(_forSelect[i]);
                }
                continue;
            }
            if (_parameter == "-bdMM") {
                if (_forSelect[i].birthday.month == intKey) {
                    existingMonth = true;
                    list.push_back(_forSelect[i]);
                }
                continue;
            }
            if (_parameter == "-bdYYYY") {
                if (_forSelect[i].birthday.year == intKey) {
                    existingYear = true;
                    list.push_back(_forSelect[i]);
                }
                continue;
            }
            if (_parameter == "-bdDD.MM.YYYY") {
                if (existingDay && existingMonth && existingYear) {
                    list.push_back(_forSelect[i]);
                }
            }
            continue;
        }
        if (_parameter == "-it") {
            if (_forSelect[i].institute == key) {
                list.push_back(_forSelect[i]);
            }
            continue;
        }
        if (lessString(_parameter, "-is")) {
            unsigned short int intKey = atoi(key.c_str());
            bool existingDay = false, existingMonth = false, existingYear = false;
            if (_parameter == "-isDD") {
                if (_forSelect[i].issue.day == intKey) {
                    existingDay = true;
                    list.push_back(_forSelect[i]);
                }
                continue;
            }
            if (_parameter == "-isMM") {
                if (_forSelect[i].issue.month == intKey) {
                    existingMonth = true;
                    list.push_back(_forSelect[i]);
                }
                continue;
            }
            if (_parameter == "-isYYYY") {
                if (_forSelect[i].issue.year == intKey) {
                    existingYear = true;
                    list.push_back(_forSelect[i]);
                }
                continue;
            }
            if (_parameter == "-isDD.MM.YYYY") {
                if (existingDay && existingMonth && existingYear) {
                    list.push_back(_forSelect[i]);
                }
            }
            continue;
        }
    }
    return list;
}

//  Selection of persons for a variety of parameters.
//  _list - list of all persons in data.
//  _selectionParameters - one or more parameters for selection with logical opertions.
vector<Person> select(vector<Person> _list, string _request) {
    vector<Person> selectedPersons;
    istringstream request(_request);
    bool isStart = true;
    while (!request.eof()) {
        char logicOperator = '|';
        if (isStart == false) {
            request >> logicOperator;
        }
        string parameter;
        request >> parameter;
        string key;
        request >> key;
        switch (logicOperator) {
            case '&':
                //  Select people from the already selected list.
                selectedPersons = interimSelection(selectedPersons, parameter, key);
                break;
                
            case '|':
                //  Select people from all data by parameter and key.
                vector<Person> personsWithOneParameter;
                personsWithOneParameter = interimSelection(_list, parameter, key);
                //  Delete the persons who are already in the list.
                for (int i = 0; i < selectedPersons.size(); i++) {
                    int j = 0;
                    while (personsWithOneParameter.empty() == false) {
                        if (selectedPersons[i].index == personsWithOneParameter[j].index) {
                            personsWithOneParameter.erase(personsWithOneParameter.begin() + (j-1));
                        } else {
                            j++;
                        }
                    }
                }
                //   Push back new persons.
                selectedPersons += personsWithOneParameter;
                break;
                
        }
        isStart = false;
    }
    return selectedPersons;
    
}

void printMenu() {
    cout << "Useable comands:\n";
    cout << "-select    People choose one or several parameters. For more information, type 'helpSelect'.\n";
    cout << "-CU        Change user.\n";
    cout << "-exit      Exit from the programm.\n";
    cout << "help       Use it to show this menu again.\n";
}

void printHelpBox() {
    cout << "People are used to select by various commands that indicate various characteristics of the person, with or without a combination of logic operations. All this is being written in a single line using a space.\n";
    cout << "Template of request: [command] key [logic operation] ...";
    cout << "Comands:\n";
    cout << "-fn             First name.\n";
    cout << "-sn             Second name.\n";
    cout << "-pt             Patronymic.\n";
    cout << "-bdXXXX         Birthday. Where XXXX can be DD - day, MM - month, YYYY - year.\n";
    cout << "-bdMM.DD.YYYY   The same birthday, but with exact date of birth.\n";
    cout << "-it             Institute.\n";
    cout << "-is             Issue. Where XXXX can be DD - day, MM - month, YYYY - year.\n";
    cout << "-isMM.DD.YYYY   The same issue, but with exact date of birth.\n";
}


int main() {
    vector<Person> list;
    string login, password;
    readPersonData(&list);
authorization:
    bool isStart = true;
    do {
        if (isStart == false) {
            cout << "\nIncorrect login or password. Please, try again.\n";
        }
        cout << "Login: ";
        cin >> login;
        cout << "Password: ";
        cin >> password;
        isStart = false;
    } while (authorization(login, password) == false);
menu:
    isStart = true;
    string answer;
    printMenu();
    do {
        cin >> answer;
        if (answer == "helpSelect") {
            printHelpBox();
        }
        if (answer == "-CU") {
            cout << endl;
            goto authorization;
        }
        if (answer == "-exit") {
            return 0;
        }
        if (answer == "help") {
            printMenu();
        }
        if (answer != "helpSelect" && answer != "-CU" && answer != "-exit" && answer != "help" && answer != "-select") {
            cout << "Incorrect input. Please, try again" << endl;
        }
    } while (answer != "-select");
    cout << "Input selection request:\n";
    string request;
    cin.ignore();
    getline(cin, request);
    vector<Person> outputPersons = select(list, request);
    cout << endl;
    printPersons(outputPersons);
    cout << endl;
    goto menu;
    
    return 0;
}
