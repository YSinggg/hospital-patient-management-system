// ============================================================
//  menu.hpp
//  Used by BOTH programs.
//  Functions that read the user's choices from the keyboard.
// ============================================================
#ifndef MENU_HPP
#define MENU_HPP

#include "patient.hpp"
#include <cstdlib>

// Keeps asking until the user types a whole number between min and max
int readNumber(string message, int min, int max) {
    int value;
    while (true) {
        cout << message;
        cin >> value;
        if (cin.eof()) {
            exit(0);   // input was closed (e.g. Ctrl+Z), end the program
        }
        if (cin.fail() || value < min || value > max) {
            cin.clear();               // clear the error
            cin.ignore(1000, '\n');    // throw away the wrong input
            cout << "Invalid input. Please enter a number from " << min << " to " << max << ".\n";
        } else {
            cin.ignore(1000, '\n');
            return value;
        }
    }
}

// Returns 0, 1 or 2 for Facility A, B or C
int chooseFacility() {
    cout << "\n1. " << facilityNames[0] << "\n2. " << facilityNames[1]
         << "\n3. " << facilityNames[2] << endl;
    return readNumber("Choose facility: ", 1, 3) - 1;
}

// Returns FIELD_AGE, FIELD_STAY, FIELD_COST or FIELD_CARE
int chooseField() {
    cout << "\nSort by:\n1. Age\n2. Visit duration (length of stay)\n3. Total cost\n4. Care type\n";
    return readNumber("Choose field: ", 1, 4);
}

// Asks the user what to search for and returns it as a SearchQuery
SearchQuery chooseSearch() {
    SearchQuery query;

    cout << "\nSearch by:\n1. Age group\n2. Care type\n3. Visit duration more than X hours\n";
    int type = readNumber("Choose search: ", 1, 3);

    if (type == 1) {
        for (int i = 0; i < NUM_AGE_GROUPS; i++) {
            cout << i + 1 << ". " << getAgeGroupName(i) << endl;
        }
        int group = readNumber("Choose age group: ", 1, NUM_AGE_GROUPS) - 1;
        query.field = FIELD_AGE;
        query.low = ageGroupMin[group];
        query.high = ageGroupMax[group];
        query.description = "Age group " + getAgeGroupName(group);
    } else if (type == 2) {
        for (int i = 0; i < NUM_CARE_TYPES; i++) {
            cout << i + 1 << ". " << careTypes[i] << endl;
        }
        int care = readNumber("Choose care type: ", 1, NUM_CARE_TYPES) - 1;
        query.field = FIELD_CARE;
        query.low = care;
        query.high = care;
        query.description = "Care type " + careTypes[care];
    } else {
        int hours = readNumber("Show patients who stayed more than how many hours? ", 0, 100000);
        query.field = FIELD_STAY;
        // Stay is stored in whole hours, so "more than X" is the same as "X + 1 or more"
        query.low = hours + 1;
        query.high = 1000000;
        query.description = "Visit duration more than " + to_string(hours) + " hours";
    }
    return query;
}

#endif
