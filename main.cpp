/* 
Name: Robin Purvis
Email: purvisr@oregonstate.edu
ONID: 932918937

Name:
Email:
ONID:
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
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <ID>" << endl;
        return EXIT_FAILURE;
    }

    StorageBufferManager manager("EmployeeRelation");
    manager.createFromFile("Employee.csv");

    int idToFind = stoi(argv[1]);

    Record foundRecord = manager.findRecordById(idToFind);

    cout << "Record found:\n";
    foundRecord.print();

    return 0;
}