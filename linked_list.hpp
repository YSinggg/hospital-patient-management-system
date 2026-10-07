#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "patient.hpp"
#include <fstream>

struct Node {
    Patient data;
    Node* next;
};

struct LinkedList {
    Node* head;
    Node* tail;
    int count;
};

void createList(LinkedList &list) {
    list.head = NULL;
    list.tail = NULL;
    list.count = 0;
}

void insertAtEnd(LinkedList &list, Patient p) {
    Node* newNode = new Node;
    newNode->data = p;
    newNode->next = NULL;

    if (list.head == NULL) {
        list.head = newNode;
    } else {
        list.tail->next = newNode;
    }
    list.tail = newNode;
    list.count++;
}

void copyList(const LinkedList &source, LinkedList &copy) {
    createList(copy);
    Node* current = source.head;
    while (current != NULL) {
        insertAtEnd(copy, current->data);
        current = current->next;
    }
}

void deleteList(LinkedList &list) {
    Node* current = list.head;
    while (current != NULL) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    createList(list);
}

bool loadFile(string fileName, LinkedList &list) {
    ifstream file(fileName);
    if (!file) {
        cout << "Error: cannot open " << fileName << endl;
        return false;
    }

    string line;
    getline(file, line);
    createList(list);

    while (getline(file, line)) {
        removeLineEnding(line);
        if (line == "") {
            continue;
        }

        Patient p;
        if (!readPatientLine(line, p)) {
            continue;
        }

        insertAtEnd(list, p);
    }

    file.close();
    return true;
}

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

// Bubble Sort on linked list (ascending)
void bubbleSort(LinkedList &list, int field, int &comparisons, int &swaps) {
    if (list.head == NULL) {
        return;
    }

    bool swapped;
    Node* lastSorted = NULL;

    do {
        swapped = false;
        Node* current = list.head;

        while (current->next != lastSorted) {
            comparisons++;
            if (getValue(current->data, field) > getValue(current->next->data, field)) {
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

// Linear Search with optional early stop if list is sorted
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
            break;
        }
        current = current->next;
    }
    return matches;
}

#endif
