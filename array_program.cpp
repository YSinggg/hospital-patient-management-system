#include "patient.hpp"
#include "timer.hpp"
#include "menu.hpp"
#include "report.hpp"
#include "array.hpp"

Patient patients[NUM_FACILITIES][MAX_PATIENTS];
int patientCount[NUM_FACILITIES];

// 1. View patients
void viewPatients() {
    int facility = chooseFacility();
    int limit = readNumber("How many patients to show? (0 = all): ", 0, MAX_PATIENTS);
    cout << "\n" << facilityNames[facility] << endl;
    displayPatients(patients[facility], patientCount[facility], limit);
}

// 2. Billing and demographic report
void showReport() {
    FacilityStats stats[NUM_FACILITIES];

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        resetStats(stats[facility]);
        for (int i = 0; i < patientCount[facility]; i++) {
            addPatientToStats(stats[facility], patients[facility][i]);
        }
    }
    reportMenu(stats);
}

// 3. Sort patients
void sortPatients() {
    int facility = chooseFacility();
    int field = chooseField();
    int limit = readNumber("How many patients to show? (0 = all): ", 0, MAX_PATIENTS);
    int count = patientCount[facility];

    // Sort a copy to preserve original order
    Patient sorted[MAX_PATIENTS];
    copyArray(patients[facility], sorted, count);

    int comparisons = 0, shifts = 0;
    double start = getTimeNow();
    insertionSort(sorted, count, field, comparisons, shifts);
    double timeTaken = getTimeNow() - start;

    cout << "\n" << facilityNames[facility] << " - sorted by " << getFieldName(field)
         << " using Insertion Sort (ascending)\n";
    displayPatients(sorted, count, limit);
    cout << "Time taken  : " << fixed << setprecision(3) << timeTaken << " microseconds\n";
    cout << "Comparisons : " << comparisons << endl;
    cout << "Shifts      : " << shifts << endl;
    cout << "Memory      : " << count * sizeof(Patient) << " bytes of patient data + "
         << sizeof(Patient) << " bytes extra (one temporary Patient)\n";
}

// 4. Search patients
void searchPatients() {
    int facility = chooseFacility();
    SearchQuery query = chooseSearch();
    int count = patientCount[facility];

    cout << "\n1. Linear search on unsorted data\n2. Insertion sort first, then binary search\n";
    int method = readNumber("Choose method: ", 1, 2);

    int checks = 0, unused = 0, matches = 0;
    double searchTime = 0;

    if (method == 1) {
        // Measure search time without I/O display overhead
        double start = getTimeNow();
        matches = linearSearch(patients[facility], count, query, false, checks);
        searchTime = getTimeNow() - start;

        cout << "\n" << query.description << " in " << facilityNames[facility] << " (Linear Search)\n";
        printPatientHeader();
        linearSearch(patients[facility], count, query, true, unused);
    } else {
        // Binary search requires sorted data
        Patient sorted[MAX_PATIENTS];
        copyArray(patients[facility], sorted, count);

        int comparisons = 0, shifts = 0;
        double sortStart = getTimeNow();
        insertionSort(sorted, count, query.field, comparisons, shifts);
        double sortTime = getTimeNow() - sortStart;

        double start = getTimeNow();
        matches = binarySearch(sorted, count, query, false, checks);
        searchTime = getTimeNow() - start;

        cout << "\n" << query.description << " in " << facilityNames[facility] << " (Binary Search)\n";
        cout << "Insertion sort time before searching: " << fixed << setprecision(3)
             << sortTime << " microseconds\n";
        printPatientHeader();
        binarySearch(sorted, count, query, true, unused);
    }

    printLine(PATIENT_TABLE_WIDTH);
    cout << "Matches found   : " << matches << endl;
    cout << "Records checked : " << checks << endl;
    cout << "Search time     : " << fixed << setprecision(3) << searchTime << " microseconds\n";
    cout << "Memory          : " << count * sizeof(Patient)
         << " bytes of patient data, no extra memory needed by the search\n";
}

// 5. Performance test
void printPerformanceRow(int facility, string operation, string test, double averageTime,
                         int comparisons, string shifts, string matches, int extraMemory) {
    cout << left << setw(10) << facilityLetters[facility] << setw(28) << operation
         << setw(20) << test << right << setw(14) << fixed << setprecision(3) << averageTime
         << setw(14) << comparisons << setw(8) << shifts << setw(8) << matches
         << setw(14) << extraMemory << endl;
}

void testInsertionSort(int facility, int field) {
    Patient temp[MAX_PATIENTS];
    int count = patientCount[facility];
    double totalTime = 0;
    int comparisons = 0, shifts = 0;

    for (int run = 0; run < RUNS; run++) {
        copyArray(patients[facility], temp, count);
        comparisons = 0;
        shifts = 0;
        double start = getTimeNow();
        insertionSort(temp, count, field, comparisons, shifts);
        totalTime += getTimeNow() - start;
    }

    printPerformanceRow(facility, "Insertion sort", getFieldName(field), totalTime / RUNS,
                        comparisons, to_string(shifts), "-", sizeof(Patient));
}

void testSearch(int facility, const SearchQuery &query) { // Linear search on original unsorted array
    int count = patientCount[facility];
    int checks = 0, matches = 0;

    double totalTime = 0;
    for (int run = 0; run < RUNS; run++) {
        checks = 0;
        double start = getTimeNow();
        matches = linearSearch(patients[facility], count, query, false, checks);
        totalTime += getTimeNow() - start;
    }
    printPerformanceRow(facility, "Linear search (unsorted)", query.description, totalTime / RUNS,
                        checks, "-", to_string(matches), 0);

    // Binary search on sorted copy
    Patient sorted[MAX_PATIENTS];
    int comparisons = 0, shifts = 0;
    copyArray(patients[facility], sorted, count);
    insertionSort(sorted, count, query.field, comparisons, shifts);

    totalTime = 0;
    for (int run = 0; run < RUNS; run++) {
        checks = 0;
        double start = getTimeNow();
        matches = binarySearch(sorted, count, query, false, checks);
        totalTime += getTimeNow() - start;
    }
    printPerformanceRow(facility, "Binary search (sorted)", query.description, totalTime / RUNS,
                        checks, "-", to_string(matches), 0);
}

void printMemoryUsage() {
    cout << "\nMEMORY USAGE\n";
    cout << "Size of one Patient            : " << sizeof(Patient) << " bytes\n";
    cout << "Array size per facility        : " << MAX_PATIENTS << " places x " << sizeof(Patient)
         << " = " << MAX_PATIENTS * sizeof(Patient) << " bytes (fixed allocation)\n";
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        cout << "Facility " << facilityLetters[facility] << " actually uses       : "
             << patientCount[facility] << " x " << sizeof(Patient) << " = "
             << patientCount[facility] * sizeof(Patient) << " bytes\n";
    }
    cout << "Note: sizes do not include dynamic string buffer memory.\n";
}

void printComplexityTable() {
    cout << "\nTIME AND SPACE COMPLEXITY (n = number of patients, k = number of matches)\n";
    printLine(100);
    cout << left << setw(32) << "Operation" << setw(20) << "Best case" << setw(26) << "Worst case"
         << "Extra memory" << endl;
    printLine(100);
    cout << left << setw(32) << "Insertion sort" << setw(20) << "O(n)" << setw(26) << "O(n^2)" << "O(1)" << endl;
    cout << left << setw(32) << "Linear search (unsorted)" << setw(20) << "O(n)" << setw(26) << "O(n)" << "O(1)" << endl;
    cout << left << setw(32) << "Binary search (sorted)" << setw(20) << "O(log n)" << setw(26) << "O(log n + k)" << "O(1)" << endl;
    cout << left << setw(32) << "Access patient number i" << setw(20) << "O(1)" << setw(26) << "O(1)" << "-" << endl;
    printLine(100);
}

void performanceTest() {
    SearchQuery tests[3] = {
        {FIELD_AGE, 61, 100, "Age 61-100"},
        {FIELD_CARE, 0, 0, "Care: Emergency"},
        {FIELD_STAY, 25, 1000000, "Duration > 24 hours"}
    };

    cout << "\n========== PERFORMANCE TEST (ARRAY) ==========\n";
    cout << "Each test is repeated " << RUNS << " times. Time is the average in microseconds.\n";
    cout << "Binary search time does not include insertion sort.\n";
    cout << "Extra Mem = auxiliary memory used by algorithm.\n";
    printLine(116);
    cout << left << setw(10) << "Facility" << setw(28) << "Operation" << setw(20) << "Field / Search"
         << right << setw(14) << "Avg Time (us)" << setw(14) << "Comparisons"
         << setw(8) << "Shifts" << setw(8) << "Matches" << setw(14) << "Extra Mem (B)" << endl;
    printLine(116);

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        testInsertionSort(facility, FIELD_AGE);
        testInsertionSort(facility, FIELD_STAY);
        testInsertionSort(facility, FIELD_COST);
        for (int i = 0; i < 3; i++) {
            testSearch(facility, tests[i]);
        }
        printLine(116);
    }

    printMemoryUsage();
    printComplexityTable();
}

int main() {
    cout << "=====================================================\n";
    cout << "  HOSPITAL PATIENT MANAGEMENT SYSTEM (ARRAY VERSION)\n";
    cout << "=====================================================\n";

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        if (!loadFile(fileNames[facility], patients[facility], patientCount[facility])) {
            cout << "Make sure the data folder is in the same folder you run the program from.\n";
            return 1;
        }
        cout << "Loaded " << patientCount[facility] << " patients for " << facilityNames[facility] << endl;
    }

    int choice;
    do {
        cout << "\n===== MAIN MENU (ARRAY) =====\n";
        cout << "1. View patient records\n";
        cout << "2. Billing and demographic report\n";
        cout << "3. Sort patients (Insertion Sort)\n";
        cout << "4. Search patients (Linear / Binary Search)\n";
        cout << "5. Performance test\n";
        cout << "0. Exit\n";
        choice = readNumber("Enter your choice: ", 0, 5);

        switch (choice) {
            case 1: viewPatients(); break;
            case 2: showReport(); break;
            case 3: sortPatients(); break;
            case 4: searchPatients(); break;
            case 5: performanceTest(); break;
            case 0: cout << "Goodbye!\n"; break;
        }
    } while (choice != 0);

    return 0;
}
