/* 
Name: Robin Purvis
Email: purvisr@oregonstate.edu
ONID: 932918937

Name: Nicholas Wisler
Email: wislerni@oregonstate.edu
ONID: 933832326
*/

#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;


int main(int argc, char* const argv[]) {
    if (argc > 2) {
        cerr << "Usage: " << argv[0] << " <ID>" << endl;
        return EXIT_FAILURE;
    }

    StorageBufferManager manager("EmployeeRelation");
    manager.createFromFile("Employee.csv");

    int idToFind=0;
    string idString;
    Record foundRecord;
    cout << "Enter IDs to search for. \nWhen you wish to exit the loop, type '-1 or any other negative number'\n";
    while(1){
        cout << "Input: ";
        scanf("%d",&idToFind);
        if (idToFind < 0)
        {
            break;
        }
        foundRecord = manager.findRecordById(idToFind);
        cout << "Record found:\n";
        foundRecord.print();

    }
    

    return 0;
}