/* 
Name: Robin Purvis
Email: purvisr@oregonstate.edu
ONID: 932918937

Name: Nicholas Wisler
Email: wislerni@oregonstate.edu
ONID: 933832326
*/

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

class Record {
public:
    int id, manager_id;
    std::string bio, name;
    Record() : id(0), manager_id(0), bio(""), name("") {}

    Record(vector<std::string> fields) {
        id = stoi(fields[0]);
        name = fields[1];
        bio = fields[2];
        manager_id = stoi(fields[3]);
    }

    void print() {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }
};

class StorageBufferManager {
private:
    const int BLOCK_SIZE = 4096;
    const string fileName;
    vector<Record> buffer;
    vector<pair<int, int>> slotDirectory; // Offset and length of each record in the block
    int numRecords;
    int currentBlockSize;
    int blocksInMemory;

    void insertRecord(Record record) {
        int recordSize = sizeof(int) * 2 + record.name.size() + record.bio.size();

        // If a block is full, write block to file, clear buffer, increment blocksInMemory counter
        if (currentBlockSize + recordSize > BLOCK_SIZE) { 
            writeBufferToFile();
            currentBlockSize = 0;
            buffer.clear();
            slotDirectory.clear();
            blocksInMemory++;
        }

        // If more than three blocks are in memory, write to file and reset counters
        if (blocksInMemory >= 3) {
            writeBufferToFile();
            currentBlockSize = 0;
            buffer.clear();
            slotDirectory.clear();
            blocksInMemory = 0;
        }

        // Store the offset and length of the record in the slot directory
        slotDirectory.push_back({currentBlockSize, recordSize});

        buffer.push_back(record);
        currentBlockSize += recordSize;
        
    }

    void writeBufferToFile() {
        ofstream file(fileName, ios::app | ios::binary);
        if (!file.is_open()) {
            cerr << "Error opening file: " << fileName << endl;
            exit(EXIT_FAILURE);
        }

        
    // Write records to file
       for (size_t i = 0; i < buffer.size(); ++i) {
            file.write(reinterpret_cast<const char*>(&buffer[i].id), sizeof(int));
            file.write(buffer[i].name.c_str(), buffer[i].name.size() + 1);
            file.write(buffer[i].bio.c_str(), buffer[i].bio.size() + 1);
            file.write(reinterpret_cast<const char*>(&buffer[i].manager_id), sizeof(int));
        }
        
        // Update slot directory with the correct offset after writing to file
        for (size_t i = 0; i < buffer.size(); ++i) {
            slotDirectory[i].first += sizeof(int) + buffer[i].name.size() + buffer[i].bio.size() + sizeof(int);
        }
        file.close();
    }

public:
    StorageBufferManager(string NewFileName) : fileName(NewFileName) {
        numRecords = 0;
        currentBlockSize = 0;
        blocksInMemory = 0;

        // Attempt to open the file in append mode.
        ofstream file(fileName, ios::app);
        if (!file.is_open()) {
            cerr << "Error creating file: " << fileName << endl;
            exit(EXIT_FAILURE);
        }
        file.close(); // Close the file after creation.
    }

    // Reads records from a CSV file and inserts them into the storage buffer.
    void createFromFile(string csvFName) {
        ifstream file(csvFName);
        if (!file.is_open()) {
            cerr << "Error opening file: " << csvFName << endl;
            exit(EXIT_FAILURE);
        }

        string line;
        // Read each line from the CSV file.
        while (getline(file, line)) {
            vector<string> fields;
            stringstream ss(line);

            // Split each line into fields using ',' as the delimiter.
            string field;
            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }

            // Create a Record from the fields and insert it.
            Record record(fields);
            insertRecord(record);
        }

        // Write the records in the buffer to the file.
        writeBufferToFile();

        file.close(); // Close the CSV file after processing.
    }

    // Finds a record by its ID in the storage buffer file.
    Record findRecordById(int id) {
        // Open the storage buffer file in binary mode.
        ifstream file(fileName, ios::binary);

        if (!file.is_open()) {
            cerr << "Error opening file: " << fileName << endl;
            exit(EXIT_FAILURE);
        }

        Record record;
        file.seekg(0, ios::beg); // Move the file pointer to the beginning.

        // Read records from the file until the end is reached.
        while (file.read(reinterpret_cast<char*>(&record.id), sizeof(int))) {
            getline(file, record.name, '\0');
            getline(file, record.bio, '\0');
            file.read(reinterpret_cast<char*>(&record.manager_id), sizeof(int));

            // If the ID matches, close the file and return the found record.
            if (record.id == id) {
                file.close();
                return record;
            }
        }

        file.close();

        // If the record is not found, return a default record with placeholder values.
        return Record({ "0", "", "", "0" });
    }
};