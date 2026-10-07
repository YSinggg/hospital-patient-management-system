// ============================================================
//  patient.hpp
//  Used by BOTH programs.
//  Holds the constants, the Patient and SearchQuery structs and
//  small helper functions that do not depend on array or linked list.
//
//  Note about "const Patient &p" in the function parameters:
//    &      = pass by reference, so the Patient is NOT copied (faster)
//    const  = the function only reads the Patient and cannot change it
// ============================================================
#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// ------------------------------------------------------------
// Constants
// ------------------------------------------------------------
const int NUM_FACILITIES = 3;
const int NUM_AGE_GROUPS = 5;
const int NUM_CARE_TYPES = 6;

string facilityNames[NUM_FACILITIES] = {
    "Facility A - General Hospital",
    "Facility B - University Medical Center",
    "Facility C - Community Health Clinic"
};
string facilityLetters[NUM_FACILITIES] = {"A", "B", "C"};
string fileNames[NUM_FACILITIES] = {
    "data/dataset1_facility_a.csv",
    "data/dataset2_facility_b.csv",
    "data/dataset3_facility_c.csv"
};

// Care types are in alphabetical order, so sorting by care type
// gives alphabetical order.
string careTypes[NUM_CARE_TYPES] = {
    "Emergency", "Inpatient", "Outpatient",
    "Rehabilitation", "Routine Checkup", "Vaccination"
};

// Age groups (same as the assignment brief)
int ageGroupMin[NUM_AGE_GROUPS] = {0, 18, 26, 46, 61};
int ageGroupMax[NUM_AGE_GROUPS] = {17, 25, 45, 60, 100};
string ageGroupLabels[NUM_AGE_GROUPS] = {
    "Pediatrics & Adolescents",
    "Young Adults / University Students",
    "Working Adults (Early Career)",
    "Working Adults (Late Career)",
    "Senior Citizens / Geriatric Care"
};

// Width of the patient table (used to draw the lines)
const int PATIENT_TABLE_WIDTH = 103;

// Field numbers used for sorting and searching
const int FIELD_AGE = 1;
const int FIELD_STAY = 2;   // visit duration (length of stay)
const int FIELD_COST = 3;
const int FIELD_CARE = 4;

// ------------------------------------------------------------
// Structs
// ------------------------------------------------------------

// One patient record (one line of the CSV file)
struct Patient {
    string id;
    int age;
    string careType;
    int careNumber;        // position of careType in the careTypes list (0-5)
    int lengthOfStay;      // visit duration in hours
    double costPerHour;    // MYR
    int visitsPerYear;
    double totalCost;      // lengthOfStay x costPerHour x visitsPerYear
};

// What we are searching for:
// a patient matches when  low <= value of field <= high
struct SearchQuery {
    int field;              // FIELD_AGE, FIELD_STAY or FIELD_CARE
    double low;
    double high;
    string description;     // text shown on screen, e.g. "Age 61-100"
};

// ------------------------------------------------------------
// Helper functions
// ------------------------------------------------------------

// Returns the age group (0-4) for an age, or -1 if not in any group
int getAgeGroup(int age) {
    for (int i = 0; i < NUM_AGE_GROUPS; i++) {
        if (age >= ageGroupMin[i] && age <= ageGroupMax[i]) {
            return i;
        }
    }
    return -1;
}

// Returns the age range of a group, e.g. "18-25"
string getAgeRange(int group) {
    return to_string(ageGroupMin[group]) + "-" + to_string(ageGroupMax[group]);
}

// Returns the full name of a group, e.g. "18-25 Young Adults / University Students"
string getAgeGroupName(int group) {
    return getAgeRange(group) + " " + ageGroupLabels[group];
}

// Returns the position of a care type in the careTypes list, or -1 if not found
int getCareNumber(string careType) {
    for (int i = 0; i < NUM_CARE_TYPES; i++) {
        if (careTypes[i] == careType) {
            return i;
        }
    }
    return -1;
}

// Returns the value of the field we want to sort or search by
double getValue(const Patient &p, int field) {
    if (field == FIELD_AGE) return p.age;
    if (field == FIELD_STAY) return p.lengthOfStay;
    if (field == FIELD_COST) return p.totalCost;
    return p.careNumber;   // FIELD_CARE
}

string getFieldName(int field) {
    if (field == FIELD_AGE) return "Age";
    if (field == FIELD_STAY) return "Visit duration";
    if (field == FIELD_COST) return "Total cost";
    return "Care type";
}

// ------------------------------------------------------------
// Reading one line of the CSV file
// ------------------------------------------------------------

// Removes the Windows line ending (\r) at the end of a line, if there is one
void removeLineEnding(string &line) {
    if (line.length() > 0 && line[line.length() - 1] == '\r') {
        line.erase(line.length() - 1);
    }
}

// Turns one CSV line into a Patient.
// Example line: PT1001,42,Emergency,12,150.0,2
// Returns false if a value is missing or the care type / age is not valid.
bool readPatientLine(string line, Patient &p) {
    stringstream ss(line);
    string id, age, care, stay, rate, visits;
    getline(ss, id, ',');
    getline(ss, age, ',');
    getline(ss, care, ',');
    getline(ss, stay, ',');
    getline(ss, rate, ',');
    getline(ss, visits, ',');

    p.id = id;
    if (age == "" || stay == "" || rate == "" || visits == "") {
        return false;   // a value is missing, so we cannot use this line
    }

    p.age = stoi(age);              // stoi = string to int
    p.careType = care;
    p.careNumber = getCareNumber(care);
    p.lengthOfStay = stoi(stay);
    p.costPerHour = stod(rate);     // stod = string to double
    p.visitsPerYear = stoi(visits);
    p.totalCost = p.lengthOfStay * p.costPerHour * p.visitsPerYear;

    if (p.careNumber == -1 || getAgeGroup(p.age) == -1) {
        return false;
    }
    return true;
}

// ------------------------------------------------------------
// Printing
// ------------------------------------------------------------

// Turns an amount into money text with 2 decimals and commas,
// e.g. 2500 becomes "2,500.00". (Amounts in this system are never negative.)
string formatMoney(double amount) {
    stringstream ss;
    ss << fixed << setprecision(2) << amount;
    string text = ss.str();               // e.g. "2322210.00"
    int dotPosition = text.find('.');
    // put a comma every 3 digits, going left from the decimal point
    for (int i = dotPosition - 3; i > 0; i = i - 3) {
        text.insert(i, ",");
    }
    return text;                          // e.g. "2,322,210.00"
}

void printLine(int length) {
    for (int i = 0; i < length; i++) {
        cout << "-";
    }
    cout << endl;
}

void printPatientHeader() {
    printLine(PATIENT_TABLE_WIDTH);
    cout << left << setw(10) << "ID" << setw(6) << "Age" << setw(11) << "Age Group"
         << setw(18) << "Care Type" << right << setw(12) << "Stay (hrs)" << setw(14) << "Cost/Hour"
         << setw(12) << "Visits/Yr" << setw(20) << "Total Cost (MYR)" << endl;
    printLine(PATIENT_TABLE_WIDTH);
}

// The Age Group column shows which age group the patient was put into (brief 4a)
void printPatient(const Patient &p) {
    cout << left << setw(10) << p.id << setw(6) << p.age << setw(11) << getAgeRange(getAgeGroup(p.age))
         << setw(18) << p.careType << right << setw(12) << p.lengthOfStay
         << setw(14) << formatMoney(p.costPerHour)
         << setw(12) << p.visitsPerYear
         << setw(20) << formatMoney(p.totalCost) << endl;
}

#endif
