// g++ -lpthread main.cpp sqlite3.o -o main.exe -static -std=c++11
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <set>
#include "sqlite3.h"

using namespace std;

static int route_callback(void *NotUsed, int argc, char **argv, char **ColNames) {
    cout << left << setw(3) << argv[0] << " ";
    cout << right << setw(25) << argv[1] << " ";
    cout << right << setw(25) << argv[2] << " ";
    cout << endl;
    return 0;
}

static int dest_callback(void *NotUsed, int argc, char **argv, char **ColNames) {
    cout << left << setw(3) << argv[0] << " ";
    cout << right << setw(25) << argv[1] << " ";
    cout << endl;
    return 0;
}

int ps() {
    cin.sync();
    cin.ignore();
    system("cls");
    return(0);
}

int chng_destination(sqlite3 *db) {
    set<int> existed;
    int rc;
    sqlite3_stmt *stmt;
    string str1, str2;
    string checkstatement = "select * from destination where name like \"";
    string upstatement = "update or fail destination set (name) = (\"";
    str1 = "0";
    sqlite3_prepare_v2(db, "select destination_ID from destination;", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        existed.insert(sqlite3_column_int(stmt, 0));
        rc = sqlite3_step(stmt);
    }
    cout << "Enter number of destination point which you want to change (0 - exit):" << endl;
    cin.clear();
    cin.sync();
    cin >> str1;
    if (str1 != "0") {
        if (existed.find(stoi(str1)) != existed.end()) {
            cin.clear();
            cin.sync();
            cout << "Enter new name:" << endl;
            cin >> str2;
            sqlite3_prepare_v2(db, (checkstatement + str2 + "\";").c_str(), -1, &stmt, NULL);
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                cout << "Error: this destination exists already" << endl;
            } else {
                sqlite3_prepare_v2(db, (upstatement + str2 + "\") where destination_ID = " + str1 + " ;").c_str(), -1, &stmt, NULL);
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE) {
                    cout << "Error:" << sqlite3_errstr(rc) << endl;
                    ps();
                }
            }
        } else {
            cout << "Error: incorrect number" << endl;
        }
    }
    sqlite3_finalize(stmt);
    return(0);
}

int chng_route(sqlite3 *db) {
    set<int> existed;
    int rc;
    sqlite3_stmt *stmt;
    string str1, str2, str3;
    string checkstatement = "select name from destination where destination_ID = ";
    string checkstatement2 = "select * from route";
    string selstatement = "select * from destination;";
    string upstatement = "update or fail route set (start_ID,end_ID) = (";
    str1 = "0";
    sqlite3_prepare_v2(db, "select route_ID from route;", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        existed.insert(sqlite3_column_int(stmt, 0));
        rc = sqlite3_step(stmt);
    }
    cout << "Enter number of route which you want to change (0 - exit):" << endl;
    cin.clear();
    cin.sync();
    cin >> str1;
    if (str1 != "0") {
        if (existed.find(stoi(str1)) != existed.end()) {
            cin.clear();
            cin.sync();
            cout << "ID" << internal << setw(25) << "Destination" << endl;
            sqlite3_exec(db, selstatement.c_str(), dest_callback, 0, NULL);
            cout << "Enter from:" << endl;
            cin >> str2;
            sqlite3_prepare_v2(db, (checkstatement + str2 + ";").c_str(), -1, &stmt, NULL);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_ROW) {
                cout << "Error: this destination isn't exists " << endl;
            } else {
                cin.sync();
                cout << "Enter to:" << endl;
                cin >> str3;
                sqlite3_prepare_v2(db, (checkstatement + str3 + ";").c_str(), -1, &stmt, NULL);
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_ROW) {
                    cout << "Error: this destination isn't exists " << endl;
                } else {
                    sqlite3_prepare_v2(db, (checkstatement2 + " where start_ID = " + str2 + " and end_ID = " + str3 + ";").c_str(), -1, &stmt, NULL);
                    rc = sqlite3_step(stmt);
                    if (rc == SQLITE_ROW) {
                        cout << "Error: this route is already exists " << endl;
                        ps();
                    } else {
                        if (str2 == str3) {
                            cout << "Error, same start / end" << endl;
                            ps();
                        } else {
                            sqlite3_prepare_v2(db, (upstatement + str2 + ", " + str3 + ") where route_ID = " + str1 + " ;").c_str(), -1, &stmt, NULL);
                            rc = sqlite3_step(stmt);
                            if (rc != SQLITE_DONE) {
                                cout << "Error:" << sqlite3_errstr(rc) << endl;
                                ps();
                            }
                        }
                    }
                }
            }
        } else {
            cout << "Error: incorrect number" << endl;
        }
    }
    sqlite3_finalize(stmt);
    return(0);
}

int del_destination(sqlite3 *db) {
    set<int> existed;
    int rc;
    sqlite3_stmt *stmt;
    string str;
    string delstatement = "delete from destination where destination_ID = ";
    str = "0";
    sqlite3_prepare_v2(db, "select destination_ID from destination;", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        existed.insert(sqlite3_column_int(stmt, 0));
        rc = sqlite3_step(stmt);
    }
    cout << "Enter number of destination which you want to delete (resricted!) (0 - exit):" << endl;
    cin.clear();
    cin.sync();
    cin >> str;
    if (str != "0") {
        if (existed.find(stoi(str)) != existed.end()) {
            sqlite3_prepare_v2(db, (delstatement + str + ";").c_str(), -1, &stmt, NULL);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                cout << "Error:" << sqlite3_errstr(rc) << endl;
            }
        } else {
            cout << "Error: incorrect number" << endl;
        }
    }
    sqlite3_finalize(stmt);
    return(0);
}

int del_route(sqlite3 *db) {
    set<int> existed;
    int rc;
    sqlite3_stmt *stmt;
    string str;
    string delstatement = "delete from route where route_ID = ";
    str = "0";
    sqlite3_prepare_v2(db, "select route_ID from route;", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        existed.insert(sqlite3_column_int(stmt, 0));
        rc = sqlite3_step(stmt);
    }
    cout << "Enter number of route which you want to delete (0 - exit):" << endl;
    cin.clear();
    cin.sync();
    cin >> str;
    if (str != "0") {
        if (existed.find(stoi(str)) != existed.end()) {
            sqlite3_prepare_v2(db, (delstatement + str + ";").c_str(), -1, &stmt, NULL);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                cout << "Error:" << sqlite3_errstr(rc) << endl;
                ps();
            }
        } else {
            cout << "Error: incorrect number" << endl;
        }
    }
    sqlite3_finalize(stmt);
    return(0);
}

int add_destination(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    string checkstatement = "select * from destination where name like \"";
    string addstatement = "insert or fail into destination(name) values(\"";
    string name;
    cin.sync();
    cout << "enter new destination name:" << endl;
    cin >> name;
    sqlite3_prepare_v2(db, (checkstatement + name + "\";").c_str(), -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        cout << "Error: this destination exists already" << endl;
    } else {
        sqlite3_prepare_v2(db, (addstatement + name + "\");").c_str(), -1, &stmt, NULL);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            cout << "Error:" << sqlite3_errstr(rc) << endl;
        }
    }
    sqlite3_finalize(stmt);
    return(0);
}

int add_route(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    string selstatement = "select * from destination;";
    string checkstatement = "select name from destination where destination_ID = ";
    string checkstatement2 = "select * from route";
    string addstatement = "insert or fail into route(start_ID,end_ID) values(";
    string str1, str2;
    while(true) {
        cin.sync();
        cout << "ID" << internal << setw(25) << "Destination" << endl;
        sqlite3_exec(db, selstatement.c_str(), dest_callback, 0, NULL);
        cout << "Enter from:" << endl;
        cin >> str1;
        sqlite3_prepare_v2(db, (checkstatement + str1 + ";").c_str(), -1, &stmt, NULL);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            cout << "Error: this destination isn't exists " << endl;
            ps();
            continue;
        }
        cin.sync();
        cout << "Enter to:" << endl;
        cin >> str2;
        sqlite3_prepare_v2(db, (checkstatement + str2 + ";").c_str(), -1, &stmt, NULL);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            cout << "Error: this destination isn't exists " << endl;
            ps();
            continue;
        }
        if (str1 == str2) {
            cout << "Error, same start and end" << endl;
            ps();
            continue;
        }
        sqlite3_prepare_v2(db, (checkstatement2 + " where start_ID = " + str1 + " and end_ID = " + str2 + ";").c_str(), -1, &stmt, NULL);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            cout << "Error: this route is already exists " << endl;
            ps();
            continue;
        }
        break;
    }
    sqlite3_prepare_v2(db, (addstatement + str1 + ", " + str2 + ");").c_str(), -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cout << "Error:" << sqlite3_errstr(rc) << endl;
        ps();
    }
    sqlite3_finalize(stmt);
    return(0);
}

int search_destination(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    string statement = "select * from destination";
    string name;
    char ch = 0;
    cout << "Enter partial name of destination point:" << endl;
    cin >> name;
    while (ch != '3') {
        sqlite3_prepare_v2(db, (statement + " where name like \"%" + name + "%\";").c_str(), -1, &stmt, NULL);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            cout << "There are no such routes!" << endl;
            break;
        } else {
            cout << "ID" << internal << setw(25) << "Name" << endl;
            while (rc == SQLITE_ROW) {
                cout << left << setw(3) << sqlite3_column_int(stmt, 0) << " ";
                cout << right << setw(25) << sqlite3_column_text(stmt, 1) << " ";
                cout << endl;
                rc = sqlite3_step(stmt);
            }
            cout << "\n1. change existed destination\n2. delete destination\n3. return to full table\n";
            cin.clear();
            cin.sync();
            cin >> ch;
            switch (ch) {
                case '1': chng_destination(db); ps(); break;
                case '2': del_destination(db); ps(); break;
                case '3': system("cls"); break;
                default: cout << "wrong choise, please, try again!" << endl; ps(); break;
            }
        }
    }
    sqlite3_finalize(stmt);
    return(0);
 }

int search_route(sqlite3 *db) {
    int rc;
    sqlite3_stmt *stmt;
    string statement = "select ID1, name1, name2 from (select route_ID as ID1, name as name1 from route join destination on (start_ID = destination_ID)) join (select route_ID as ID2, name as name2 from route join destination on (end_ID = destination_ID)) on (ID1 = ID2)";
    string name;
    char ch = 0;
    cout << "Enter partial name of destination point:" << endl;
    cin >> name;
    while (ch != '3') {
        sqlite3_prepare_v2(db, (statement + " where name1 like \"%" + name + "%\" or name2 like \"%" + name + "%\";").c_str(), -1, &stmt, NULL);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            cout << "There are no such routes!" << endl;
            break;
        } else {
            cout << "ID" << internal << setw(25) << "Start" << internal << setw(25) << "End" << endl;
            while (rc == SQLITE_ROW) {
                cout << left << setw(3) << sqlite3_column_int(stmt, 0) << " ";
                cout << right << setw(25) << sqlite3_column_text(stmt, 1) << " ";
                cout << right << setw(25) << sqlite3_column_text(stmt, 2) << " ";
                cout << endl;
                rc = sqlite3_step(stmt);
            }
            cout << "\n1. change existed route\n2. delete route\n3. return to full table\n";
            cin.clear();
            cin.sync();
            cin >> ch;
            switch (ch) {
                case '1': chng_route(db); ps(); break;
                case '2': del_route(db); ps(); break;
                case '3': system("cls"); break;
                default: cout << "wrong choise, please, try again!" << endl; ps(); break;
            }
        }
    }
    sqlite3_finalize(stmt);
    return(0);
 }

int table_destination(sqlite3 *db) {
    string sort_str = ";";
    char ch = 0;
    string statement = "select * from destination";
    while (ch != '7') {
        cout << "ID" << internal << setw(25) << "Name" << endl;
        sqlite3_exec(db, (statement + sort_str).c_str(), dest_callback, 0, NULL);
        cout << "\n1. add destination\n2. change existed destination\n3. delete destination\n4. sort by name\n5. reset sorted\n6. search by name\n7. return to main menu\n";
        cin.clear();
        cin.sync();
        cin >> ch;
        switch (ch) {
            case '1': add_destination(db); ps(); break;
            case '2': chng_destination(db); ps(); break;
            case '3': del_destination(db); ps(); break;
            case '4': sort_str = " order by name ASC;"; ps(); break;
            case '5': sort_str = " order by destination_ID ASC;"; ps(); break;
            case '6': search_destination(db); ps(); break;
            case '7': system("cls"); break;
            default: cout << "wrong choise, please, try again!" << endl; ps(); break;
        }
    }
    return(0);
}


int table_route(sqlite3 *db) {
    string sort_str = ";";
    char ch = 0;
    string statement = "select ID1, name1, name2 from (select route_ID as ID1, name as name1 from route join destination on (start_ID = destination_ID)) join (select route_ID as ID2, name as name2 from route join destination on (end_ID = destination_ID)) on (ID1 = ID2)";
    while (ch != '8') {
        cout << "ID" << internal << setw(25) << "Start" << internal << setw(25) << "End" << endl;
        sqlite3_exec(db, (statement + sort_str).c_str(), route_callback, 0, NULL);
        cout << "\n1. add route\n2. change existed route\n3. delete route\n4. sort by start point\n5. sort by end point\n6. reset sorted\n7. search by destination point\n8. return to main menu\n";
        cin.clear();
        cin.sync();
        cin >> ch;
        switch (ch) {
            case '1': add_route(db); ps(); break;
            case '2': chng_route(db); ps(); break;
            case '3': del_route(db); ps(); break;
            case '4': sort_str = " order by name1 ASC;"; ps(); break;
            case '5': sort_str = " order by name2 ASC;"; ps(); break;
            case '6': sort_str = " order by ID1 ASC;"; ps(); break;
            case '7': search_route(db); ps(); break;
            case '8': system("cls"); break;
            default: cout << "wrong choise, please, try again!" << endl; ps(); break;
        }
    }
    return(0);
}

int main() {
    char ch = 0;
    string statement;
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("waypoints.db", &db);
    if (rc) {
        cout << "Can't open database:" << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return(1);
    } else {
        cout << "Database is opened" << endl;
    }
    statement = "pragma foreign_keys=on;";
    rc = sqlite3_exec(db, statement.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        cout << "SQL error:" << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    while (ch != '3') {
        cout << "Menu:\n1. print routes\n2. print list of destinations\n3. exit\n";
        cin.clear();
        cin.sync();
        cin >> ch;
        switch (ch) {
            case '1': table_route(db); ps(); break;
            case '2': table_destination(db); ps(); break;
            case '3': cout << "goodbye!" << endl; break;
            default: cout << "wrong choise, please, try again!" << endl; ps(); break;
        }
    }
    sqlite3_close(db);
    delete zErrMsg;
    return 0;
}