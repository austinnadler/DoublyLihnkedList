// File: main.cpp
#include "LinkedList.h"
#include <iomanip>
using namespace std;
int main() {
    LinkedList<int> list;
    cout << "Empty? "; if(list.isEmpty()) { cout << "Yes" << endl; } else { cout << "No" << endl;}
    list.pushFront(1);
    cout << setw(20) << left << "pushFront(1):" << setw(50) << right << list << endl;
    list.pushBack(4);
    cout << setw(20) << left << "pushBack(4):"<< setw(50) << right << list << endl;
    list.push(2);
    cout << setw(20) << left << "push(2):"<< setw(50) << right <<  list << endl;
    list.push(0);
    cout << setw(20) << left << "push(0):" << setw(50) << right<< list << endl;
    list.push(12);
    cout << setw(20) << left << "push(12):"<< setw(50) << right << list << endl;
    list.push(24);
    cout << setw(20) << left << "push(24):"<< setw(50) << right << list << endl;
    list.push(4);
    cout << setw(20) << left << "push(4):"<< setw(50) << right << list << endl;
    list.push(59);
    cout << setw(20) << left << "push(59):"<< setw(50) << right << list << endl;
    list.push(90);
    cout<< setw(20) << left  << "push(90):" << setw(50) << right<< list << endl;
    list.pushBack(14);
    cout << setw(20) << left << "pushBack(14):" << setw(50) << right<< list << endl;
    int popped = list.popAt(0);
    cout << setw(20) << left << "popAt(0):" << setw(50) << right << list << " popped: " << popped << endl;
    popped = list.popAt(6);
    cout << setw(20) << left << "popAt(6):" << setw(50) << right << list << " popped: " << popped << endl;
    popped = list.popAt(1);
    cout << setw(20) << left << "popAt(1):" << setw(50) << right << list << " popped: " << popped << endl;
    list.pushAt(0,123);
    cout << setw(20) << left << "pushAt(0,123):" << setw(50) << right << list << endl;
    list.pushAt(4, -1234);
    cout << setw(20) << left << "pushAt(4,-1234):" << setw(50) << right << list << endl;
    list.deleteAt(3);
    cout << setw(20) << left << "deleteAt(3):" << setw(50) << right<< list << endl;
    list.deleteE(123);
    cout << setw(20) << left << "deleteE(123):" << setw(50) << right<< list << endl;
    list.deleteFront();
    cout << setw(20) << left << "deleteFront():" << setw(50) << right<< list << endl;
    list.deleteBack();
    cout << setw(20) << left << "deleteBack():" << setw(50) << right<< list << endl;
    list.setAt(2,34);
    cout << setw(20) << left << "setAt(2,34):" << setw(50) << right<< list << endl;
    list.setFront(2);
    cout << setw(20) << left << "setFront(2):" << setw(50) << right<< list << endl;
    list.setBack(-2323);
    cout << setw(20) << left << "setBack(-2323):" << setw(50) << right<< list << endl;
    list.pushBack(93);
    cout << setw(20) << left << "pushBack(93):" << setw(50) << right<< list << endl;
    cout << endl;
    cout << "Empty?: "; if(list.isEmpty()) { cout << "Yes" << endl; } else { cout << "No" << endl;}
    cout << "getAt(0): " << list.getAt(0) << endl;
    cout << "getBack(): " << list.getBack() << endl;
    cout << "getAt(2): " << list.getAt(2) << endl;
    cout << "getFront(): " << list.getFront() << endl;
    cout << "contains(12) "; if(list.contains(12)) { cout << "Yes" << endl; } else { cout << "No" << endl; }
    cout << "contains(2) ";  if(list.contains(2))  { cout << "Yes" << endl; } else { cout << "No" << endl; }
    cout << list.size() << " items in list" << endl;
    cout << "Try setAt(10, 15): ";
    try {
        list.setAt(23, 34);  
    } catch (logic_error& e) {
        cout << e.what() << endl;
    }
    cout << "Try deleteAt(-3): ";
    try {
        list.deleteAt(-3);  
    } catch (logic_error& e) {
        cout << e.what() << endl;
    }
    return 0;
}// end main()
