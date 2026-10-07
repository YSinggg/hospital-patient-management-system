#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "patient.hpp"
#include <fstream>

// One node of the singly linked list
struct Node {
    Patient data;   // the patient stored in this node
    Node* next;     // pointer to the next node (NULL if this is the last node)
};

// The linked list for one facility
struct LinkedList {
    Node* head;     // first node
    Node* tail;     // last node (so adding at the end is fast)
    int count;      // number of nodes in the list
};

// Basic linked list functions

// Make the list empty (must be called before using a new list)
void createList(LinkedList &list) {
    list.head = NULL;
    list.tail = NULL;
    list.count = 0;
}

// Add a patient to the end of the list
void insertAtEnd(LinkedList &list, Patient p) {
    Node* newNode = new Node;
    newNode->data = p;
    newNode->next = NULL;

    if (list.head == NULL) {
        list.head = newNode;        // list was empty, so the new node is the first node
    } else {
        list.tail->next = newNode;  // link the old last node to the new node
    }
    list.tail = newNode;            // the new node is now the last node
    list.count++;
}

// Copy every patient from source into a new list called copy
// (used so we can sort a copy and keep the original order)
void copyList(const LinkedList &source, LinkedList &copy) {
    createList(copy);
    Node* current = source.head;
    while (current != NULL) {
        insertAtEnd(copy, current->data);
        current = current->next;
    }
}

// Delete every node to free the memory
void deleteList(LinkedList &list) {
    Node* current = list.head;
    while (current != NULL) {
        Node* nextNode = current->next;   // remember the next node before deleting
        delete current;
        current = nextNode;
    }
    createList(list);   // the list is empty again
}

// Load patients from a CSV file into a linked list
bool loadFile(string fileName, LinkedList &list) {
    ifstream file(fileName);
    if (!file) {
        cout << "Error: cannot open " << fileName << endl;
        return false;
    }

    string line;
    getline(file, line);   // skip the header line
    createList(list);

    while (getline(file, line)) {
        removeLineEnding(line);
        if (line == "") {
            continue;   // skip empty lines
        }

        Patient p;
        if (!readPatientLine(line, p)) {
            cout << "Warning: skipped patient " << p.id << " (missing or invalid value).\n";
            continue;
        }

        insertAtEnd(list, p);   // add a new node at the end
    }

    file.close();
    return true;
}

// Display patients (limit 0 = show all)
void displayList(const LinkedList &list, int limit) {
    if (limit == 0 || limit > list.count) {
        limit = list.count;
    }
    printPatientHeader();
    Node* current = list.head;
    int shown = 0;
    while (current != NULL && shown < limit) {
        printPatient(current->data);
        shown++;
        current = current->next;
    }
    printLine(PATIENT_TABLE_WIDTH);
    cout << "Showing " << shown << " of " << list.count << " patients.\n";
}

// BUBBLE SORT (ascending)
// Compares each pair of neighbouring nodes and swaps their
// patient data if they are in the wrong order. After each pass
// the biggest value is at the end, so the next pass can stop
// one node earlier. Stops early if a pass makes no swaps.
void bubbleSort(LinkedList &list, int field, int &comparisons, int &swaps) {
    if (list.head == NULL) {
        return;   // nothing to sort
    }

    bool swapped;
    Node* lastSorted = NULL;   // nodes from here to the end are already sorted

    do {
        swapped = false;
        Node* current = list.head;

        while (current->next != lastSorted) {
            comparisons++;
            if (getValue(current->data, field) > getValue(current->next->data, field)) {
                // swap the patient data of the two nodes
                Patient temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swaps++;
                swapped = true;
            }
            current = current->next;
        }
        lastSorted = current;
    } while (swapped);
}

// LINEAR SEARCH
// Goes through the list node by node.
// If the list is sorted by the same field, we can stop early
// once the value is bigger than what we are looking for.
// Returns the number of matches. Prints them if show is true.
// checks counts how many patients were looked at.
int linearSearch(const LinkedList &list, const SearchQuery &query, bool isSorted,
                 bool show, int &checks) {
    int matches = 0;
    Node* current = list.head;

    while (current != NULL) {
        checks++;
        double value = getValue(current->data, query.field);

        if (value >= query.low && value <= query.high) {
            matches++;
            if (show) printPatient(current->data);
        } else if (isSorted && value > query.high) {
            break;   // sorted list, so nothing after this can match
        }
        current = current->next;
    }
    return matches;
}

#endif
