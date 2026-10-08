#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "patient.hpp"
#include <fstream>

const int MAX_PATIENTS = 500;

bool loadFile(string fileName, Patient arr[], int &count) {
    ifstream file(fileName);
    if (!file) {
        cout << "Error: cannot open " << fileName << endl;
        return false;
    }

    string line;
    getline(file, line); // header line
    count = 0;

    while (getline(file, line)) {
        removeLineEnding(line);
        if (line == "") {
            continue;
        }
        if (count >= MAX_PATIENTS) {
            cout << "Warning: " << fileName << " exceeds limit of " << MAX_PATIENTS << " records.\n";
            break;
        }

        Patient p;
        if (!readPatientLine(line, p)) {
            continue;
        }

        arr[count] = p;
        count++;
    }

    file.close();
    return true;
}

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

void copyArray(Patient source[], Patient destination[], int count) {
    for (int i = 0; i < count; i++) {
        destination[i] = source[i];
    }
}

// Insertion Sort
void insertionSort(Patient arr[], int count, int field, int &comparisons, int &shifts) {
    for (int i = 1; i < count; i++) {
        Patient current = arr[i];
        int j = i - 1;

        while (j >= 0) {
            comparisons++;
            if (getValue(arr[j], field) > getValue(current, field)) {
                arr[j + 1] = arr[j];
                shifts++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = current;
    }
}

// Linear Search
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

// Binary Search
int binarySearch(Patient arr[], int count, const SearchQuery &query, bool show, int &checks) {
    int left = 0;
    int right = count - 1;
    int found = -1;

    // Tried any possible match
    while (left <= right) {
        int mid = (left + right) / 2;
        double value = getValue(arr[mid], query.field);
        checks++;
        if (value < query.low) {
            left = mid + 1;
        } else if (value > query.high) {
            right = mid - 1;
        } else {
            found = mid;
            break;
        }
    }

    if (found == -1) {
        return 0;
    }

    // Start with first element
    int first = found;
    while (first > 0) {
        checks++;
        if (getValue(arr[first - 1], query.field) >= query.low) {
            first--;
        } else {
            break;
        }
    }

    // Fetch all elements
    int matches = 0;
    for (int i = first; i < count; i++) {
        checks++;
        if (getValue(arr[i], query.field) > query.high) {
            break;
        }
        matches++;
        if (show) printPatient(arr[i]);
    }
    return matches;
}

#endif
