#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

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

string careTypes[NUM_CARE_TYPES] = {
    "Emergency", "Inpatient", "Outpatient",
    "Rehabilitation", "Routine Checkup", "Vaccination"
};

int ageGroupMin[NUM_AGE_GROUPS] = {0, 18, 26, 46, 61};
int ageGroupMax[NUM_AGE_GROUPS] = {17, 25, 45, 60, 100};
string ageGroupLabels[NUM_AGE_GROUPS] = {
    "Pediatrics & Adolescents",
    "Young Adults / University Students",
    "Working Adults (Early Career)",
    "Working Adults (Late Career)",
    "Senior Citizens / Geriatric Care"
};

const int PATIENT_TABLE_WIDTH = 103;

const int FIELD_AGE = 1;
const int FIELD_STAY = 2;
const int FIELD_COST = 3;
const int FIELD_CARE = 4;

struct Patient {
    string id;
    int age;
    string careType;
    int careNumber;        // index in careTypes (0-5)
    int lengthOfStay;      // hours
    double costPerHour;    // MYR
    int visitsPerYear;
    double totalCost;
};

struct SearchQuery {
    int field;
    double low;
    double high;
    string description;
};

// Returns age group index (0-4), or -1 if invalid
int getAgeGroup(int age) {
    for (int i = 0; i < NUM_AGE_GROUPS; i++) {
        if (age >= ageGroupMin[i] && age <= ageGroupMax[i]) {
            return i;
        }
    }
    return -1;
}

string getAgeRange(int group) {
    return to_string(ageGroupMin[group]) + "-" + to_string(ageGroupMax[group]);
}

string getAgeGroupName(int group) {
    return getAgeRange(group) + " " + ageGroupLabels[group];
}

int getCareNumber(string careType) {
    for (int i = 0; i < NUM_CARE_TYPES; i++) {
        if (careTypes[i] == careType) {
            return i;
        }
    }
    return -1;
}

double getValue(const Patient &p, int field) {
    if (field == FIELD_AGE) return p.age;
    if (field == FIELD_STAY) return p.lengthOfStay;
    if (field == FIELD_COST) return p.totalCost;
    return p.careNumber;
}

string getFieldName(int field) {
    if (field == FIELD_AGE) return "Age";
    if (field == FIELD_STAY) return "Visit duration";
    if (field == FIELD_COST) return "Total cost";
    return "Care type";
}

void removeLineEnding(string &line) {
    if (line.length() > 0 && line[line.length() - 1] == '\r') {
        line.erase(line.length() - 1);
    }
}

// Parse one line from CSV
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
        return false;
    }

    p.age = stoi(age);
    p.careType = care;
    p.careNumber = getCareNumber(care);
    p.lengthOfStay = stoi(stay);
    p.costPerHour = stod(rate);
    p.visitsPerYear = stoi(visits);
    p.totalCost = p.lengthOfStay * p.costPerHour * p.visitsPerYear;

    if (p.careNumber == -1 || getAgeGroup(p.age) == -1) {
        return false;
    }
    return true;
}

// Format double to currency string with 2 decimals and commas
string formatMoney(double amount) {
    stringstream ss;
    ss << fixed << setprecision(2) << amount;
    string text = ss.str();
    int dotPosition = text.find('.');
    for (int i = dotPosition - 3; i > 0; i = i - 3) {
        text.insert(i, ",");
    }
    return text;
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

void printPatient(const Patient &p) {
    cout << left << setw(10) << p.id << setw(6) << p.age << setw(11) << getAgeRange(getAgeGroup(p.age))
         << setw(18) << p.careType << right << setw(12) << p.lengthOfStay
         << setw(14) << formatMoney(p.costPerHour)
         << setw(12) << p.visitsPerYear
         << setw(20) << formatMoney(p.totalCost) << endl;
}

#endif
