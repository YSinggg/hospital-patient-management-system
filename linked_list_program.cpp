// Hospital Patient Management System - Singly Linked List Version

#include "patient.hpp"
#include "timer.hpp"
#include "menu.hpp"
#include "report.hpp"
#include "linked_list.hpp"

// One linked list for each facility
LinkedList lists[NUM_FACILITIES];

// 1. View patients
void viewPatients() {
    int facility = chooseFacility();
    int limit = readNumber("How many patients to show? (0 = all): ", 0, 100000);
    cout << "\n" << facilityNames[facility] << endl;
    displayList(lists[facility], limit);
}

// 2. Billing and demographic report
void showReport() {
    FacilityStats stats[NUM_FACILITIES];

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        resetStats(stats[facility]);
        Node* current = lists[facility].head;
        while (current != NULL) {
            addPatientToStats(stats[facility], current->data);
            current = current->next;
        }
    }
    reportMenu(stats);
}

// 3. Sort patients
void sortPatients() {
    int facility = chooseFacility();
    int field = chooseField();
    int limit = readNumber("How many patients to show? (0 = all): ", 0, 100000);
    int count = lists[facility].count;

    // Sort a copy to preserve original order
    LinkedList sorted;
    copyList(lists[facility], sorted);

    int comparisons = 0, swaps = 0;
    double start = getTimeNow();
    bubbleSort(sorted, field, comparisons, swaps);
    double timeTaken = getTimeNow() - start;

    cout << "\n" << facilityNames[facility] << " - sorted by " << getFieldName(field)
         << " using Bubble Sort (ascending)\n";
    displayList(sorted, limit);
    cout << "Time taken  : " << fixed << setprecision(3) << timeTaken << " microseconds\n";
    cout << "Comparisons : " << comparisons << endl;
    cout << "Swaps       : " << swaps << endl;
    cout << "Memory      : " << count * sizeof(Node) << " bytes of nodes ("
         << count << " x " << sizeof(Node) << ") + " << sizeof(Patient)
         << " bytes extra (one temporary Patient)\n";

    deleteList(sorted);
}

// 4. Search patients
void searchPatients() {
    int facility = chooseFacility();
    SearchQuery query = chooseSearch();
    int count = lists[facility].count;

    cout << "\n1. Linear search on unsorted list\n2. Bubble sort first, then linear search with early stop\n";
    int method = readNumber("Choose method: ", 1, 2);

    int checks = 0, unused = 0, matches = 0;
    double searchTime = 0;

    if (method == 1) {
        // Measure search time without I/O display overhead
        double start = getTimeNow();
        matches = linearSearch(lists[facility], query, false, false, checks);
        searchTime = getTimeNow() - start;

        cout << "\n" << query.description << " in " << facilityNames[facility] << " (Linear Search, unsorted)\n";
        printPatientHeader();
        linearSearch(lists[facility], query, false, true, unused);
    } else {
        // Sort a copy first for early stop search
        LinkedList sorted;
        copyList(lists[facility], sorted);

        int comparisons = 0, swaps = 0;
        double sortStart = getTimeNow();
        bubbleSort(sorted, query.field, comparisons, swaps);
        double sortTime = getTimeNow() - sortStart;

        double start = getTimeNow();
        matches = linearSearch(sorted, query, true, false, checks);
        searchTime = getTimeNow() - start;

        cout << "\n" << query.description << " in " << facilityNames[facility] << " (Linear Search, sorted)\n";
        cout << "Bubble sort time before searching: " << fixed << setprecision(3)
             << sortTime << " microseconds\n";
        printPatientHeader();
        linearSearch(sorted, query, true, true, unused);

        deleteList(sorted);
    }

    printLine(PATIENT_TABLE_WIDTH);
    cout << "Matches found   : " << matches << endl;
    cout << "Records checked : " << checks << endl;
    cout << "Search time     : " << fixed << setprecision(3) << searchTime << " microseconds\n";
    cout << "Memory          : " << count * sizeof(Node) << " bytes of nodes ("
         << count << " x " << sizeof(Node) << "), no extra memory needed by the search\n";
}

// 5. Performance test
void printPerformanceRow(int facility, string operation, string test, double averageTime,
                         int comparisons, string swaps, string matches, int extraMemory) {
    cout << left << setw(10) << facilityLetters[facility] << setw(28) << operation
         << setw(20) << test << right << setw(14) << fixed << setprecision(3) << averageTime
         << setw(14) << comparisons << setw(8) << swaps << setw(8) << matches
         << setw(14) << extraMemory << endl;
}

void testBubbleSort(int facility, int field) {
    double totalTime = 0;
    int comparisons = 0, swaps = 0;

    for (int run = 0; run < RUNS; run++) {
        LinkedList temp;
        copyList(lists[facility], temp);
        comparisons = 0;
        swaps = 0;
        double start = getTimeNow();
        bubbleSort(temp, field, comparisons, swaps);
        totalTime += getTimeNow() - start;
        deleteList(temp);
    }

    printPerformanceRow(facility, "Bubble sort", getFieldName(field), totalTime / RUNS,
                        comparisons, to_string(swaps), "-", sizeof(Patient));
}

void testSearch(int facility, const SearchQuery &query) {
    int checks = 0, matches = 0;

    // Linear search on unsorted list
    double totalTime = 0;
    for (int run = 0; run < RUNS; run++) {
        checks = 0;
        double start = getTimeNow();
        matches = linearSearch(lists[facility], query, false, false, checks);
        totalTime += getTimeNow() - start;
    }
    printPerformanceRow(facility, "Linear search (unsorted)", query.description, totalTime / RUNS,
                        checks, "-", to_string(matches), 0);

    // Linear search with early stop on sorted list
    LinkedList sorted;
    int comparisons = 0, swaps = 0;
    copyList(lists[facility], sorted);
    bubbleSort(sorted, query.field, comparisons, swaps);

    totalTime = 0;
    for (int run = 0; run < RUNS; run++) {
        checks = 0;
        double start = getTimeNow();
        matches = linearSearch(sorted, query, true, false, checks);
        totalTime += getTimeNow() - start;
    }
    printPerformanceRow(facility, "Linear search (sorted)", query.description, totalTime / RUNS,
                        checks, "-", to_string(matches), 0);

    deleteList(sorted);
}

void printMemoryUsage() {
    cout << "\nMEMORY USAGE\n";
    cout << "Size of one Patient            : " << sizeof(Patient) << " bytes\n";
    cout << "Size of one Node               : " << sizeof(Node) << " bytes (Patient + next pointer)\n";
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        cout << "Facility " << facilityLetters[facility] << " list uses           : "
             << lists[facility].count << " nodes x " << sizeof(Node) << " = "
             << lists[facility].count * sizeof(Node) << " bytes\n";
    }
    cout << "Each node needs " << sizeof(Node) - sizeof(Patient)
         << " more bytes than an array element because of the next pointer.\n";
    cout << "Note: sizes do not include dynamic string buffer memory.\n";
}

void printComplexityTable() {
    cout << "\nTIME AND SPACE COMPLEXITY (n = number of patients)\n";
    printLine(100);
    cout << left << setw(32) << "Operation" << setw(20) << "Best case" << setw(26) << "Worst case"
         << "Extra memory" << endl;
    printLine(100);
    cout << left << setw(32) << "Bubble sort" << setw(20) << "O(n)" << setw(26) << "O(n^2)" << "O(1)" << endl;
    cout << left << setw(32) << "Linear search (unsorted)" << setw(20) << "O(n)" << setw(26) << "O(n)" << "O(1)" << endl;
    cout << left << setw(32) << "Linear search (sorted)" << setw(20) << "O(1) early stop" << setw(26) << "O(n)" << "O(1)" << endl;
    cout << left << setw(32) << "Access patient number i" << setw(20) << "O(1) (first)" << setw(26) << "O(n) (traverse)" << "-" << endl;
    cout << left << setw(32) << "Add patient at end" << setw(20) << "O(1) with tail" << setw(26) << "O(1) with tail" << "1 new node" << endl;
    printLine(100);
}

void performanceTest() {
    // Test search queries
    SearchQuery tests[3] = {
        {FIELD_AGE, 61, 100, "Age 61-100"},
        {FIELD_CARE, 0, 0, "Care: Emergency"},
        {FIELD_STAY, 25, 1000000, "Duration > 24 hours"}
    };

    cout << "\n========== PERFORMANCE TEST (SINGLY LINKED LIST) ==========\n";
    cout << "Each test is repeated " << RUNS << " times. Time is the average in microseconds.\n";
    cout << "Sorted search time does not include bubble sort.\n";
    cout << "Extra Mem = auxiliary memory used by algorithm.\n";
    printLine(116);
    cout << left << setw(10) << "Facility" << setw(28) << "Operation" << setw(20) << "Field / Search"
         << right << setw(14) << "Avg Time (us)" << setw(14) << "Comparisons"
         << setw(8) << "Swaps" << setw(8) << "Matches" << setw(14) << "Extra Mem (B)" << endl;
    printLine(116);

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        testBubbleSort(facility, FIELD_AGE);
        testBubbleSort(facility, FIELD_STAY);
        testBubbleSort(facility, FIELD_COST);
        for (int i = 0; i < 3; i++) {
            testSearch(facility, tests[i]);
        }
        printLine(116);
    }

    printMemoryUsage();
    printComplexityTable();
}

int main() {
    cout << "===================================================================\n";
    cout << "  HOSPITAL PATIENT MANAGEMENT SYSTEM (SINGLY LINKED LIST VERSION)\n";
    cout << "===================================================================\n";

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        if (!loadFile(fileNames[facility], lists[facility])) {
            cout << "Make sure the data folder is in the same folder you run the program from.\n";
            return 1;
        }
        cout << "Loaded " << lists[facility].count << " patients for " << facilityNames[facility] << endl;
    }

    int choice;
    do {
        cout << "\n===== MAIN MENU (SINGLY LINKED LIST) =====\n";
        cout << "1. View patient records\n";
        cout << "2. Billing and demographic report\n";
        cout << "3. Sort patients (Bubble Sort)\n";
        cout << "4. Search patients (Linear Search)\n";
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

    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        deleteList(lists[facility]);
    }
    return 0;
}
