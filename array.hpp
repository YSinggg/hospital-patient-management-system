// ============================================================
//  array.hpp
//  Used by the ARRAY program only.
//  Loading, displaying, Insertion Sort, Linear Search and
//  Binary Search for an array of patients.
// ============================================================
#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "patient.hpp"
#include <fstream>

const int MAX_PATIENTS = 500;   // maximum patients one facility can hold

// ------------------------------------------------------------
// Load patients from a CSV file into an array
// ------------------------------------------------------------
bool loadFile(string fileName, Patient arr[], int &count) {
    ifstream file(fileName);
    if (!file) {
        cout << "Error: cannot open " << fileName << endl;
        return false;
    }

    string line;
    getline(file, line);   // skip the header line
    count = 0;

    while (getline(file, line)) {
        removeLineEnding(line);
        if (line == "") {
            continue;   // skip empty lines
        }
        if (count >= MAX_PATIENTS) {
            cout << "Warning: " << fileName << " has more than " << MAX_PATIENTS
                 << " patients. Extra patients were not loaded.\n";
            break;
        }

        Patient p;
        if (!readPatientLine(line, p)) {
            cout << "Warning: skipped patient " << p.id << " (missing or invalid value).\n";
            continue;
        }

        arr[count] = p;   // put the patient in the next free place
        count++;
    }

    file.close();
    return true;
}

// ------------------------------------------------------------
// Display patients (limit 0 = show all)
// ------------------------------------------------------------
void displayPatients(Patient arr[], int count, int limit) {
    if (limit == 0 || limit > count) {
        limit = count;
    }
    printPatientHeader();
    for (int i = 0; i < limit; i++) {
        printPatient(arr[i]);
    }
    printLine(PATIENT_TABLE_WIDTH);
    cout << "Showing " << limit << " of " << count << " patients.\n";
}

// Copy one array into another (so we can sort a copy and keep the original order)
void copyArray(Patient source[], Patient destination[], int count) {
    for (int i = 0; i < count; i++) {
        destination[i] = source[i];
    }
}

// ------------------------------------------------------------
// INSERTION SORT (ascending)
// Takes each patient and shifts the bigger ones to the right
// until the correct place for that patient is found.
// ------------------------------------------------------------
void insertionSort(Patient arr[], int count, int field, int &comparisons, int &shifts) {
    for (int i = 1; i < count; i++) {
        Patient current = arr[i];
        int j = i - 1;

        while (j >= 0) {
            comparisons++;
            if (getValue(arr[j], field) > getValue(current, field)) {
                arr[j + 1] = arr[j];   // shift bigger patient one place right
                shifts++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = current;   // put the patient in its correct place
    }
}

// ------------------------------------------------------------
// LINEAR SEARCH (works on unsorted data)
// Checks every patient one by one.
// Returns the number of matches. Prints them if show is true.
// checks counts how many patients were looked at.
// ------------------------------------------------------------
int linearSearch(Patient arr[], int count, const SearchQuery &query, bool show, int &checks) {
    int matches = 0;
    for (int i = 0; i < count; i++) {
        checks++;
        double value = getValue(arr[i], query.field);
        if (value >= query.low && value <= query.high) {
            matches++;
            if (show) printPatient(arr[i]);
        }
    }
    return matches;
}

// ------------------------------------------------------------
// BINARY SEARCH (array must already be sorted by the same field)
// Step 1: binary search to find any one patient inside the range
// Step 2: move left to the first patient inside the range
// Step 3: move right and collect every patient inside the range
// ------------------------------------------------------------
int binarySearch(Patient arr[], int count, const SearchQuery &query, bool show, int &checks) {
    int left = 0;
    int right = count - 1;
    int found = -1;

    // Step 1
    while (left <= right) {
        int mid = (left + right) / 2;
        double value = getValue(arr[mid], query.field);
        checks++;
        if (value < query.low) {
            left = mid + 1;        // answer is on the right side
        } else if (value > query.high) {
            right = mid - 1;       // answer is on the left side
        } else {
            found = mid;           // this patient is inside the range
            break;
        }
    }

    if (found == -1) {
        return 0;   // no match
    }

    // Step 2
    int first = found;
    while (first > 0) {
        checks++;
        if (getValue(arr[first - 1], query.field) >= query.low) {
            first--;
        } else {
            break;
        }
    }

    // Step 3
    int matches = 0;
    for (int i = first; i < count; i++) {
        checks++;
        if (getValue(arr[i], query.field) > query.high) {
            break;   // sorted, so nothing after this can match
        }
        matches++;
        if (show) printPatient(arr[i]);
    }
    return matches;
}

#endif
