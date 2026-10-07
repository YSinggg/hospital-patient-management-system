# Hospital Patient Management System

This project is a C++ study comparing two data structures—Fixed-size Array and Singly Linked List—for managing patient records across three hospital facilities.

The system implements patient record viewing, billing calculations, sorting, searching, and performance timing to compare how arrays and linked lists perform in practice.

---

## Project Structure

- `patient.hpp`: Patient structure, search query definitions, CSV reading, and display helpers.
- `timer.hpp`: Timer helper using std::chrono to measure execution time in microseconds.
- `menu.hpp`: Console input validation and menu navigation.
- `report.hpp`: Demographic breakdown, expenditure calculations, and recommendations.
- `array.hpp`: Array implementation of Insertion Sort, Linear Search, and Binary Search.
- `array_program.cpp`: Main driver program for the array version.
- `linked_list.hpp`: Singly linked list implementation of Bubble Sort and Linear Search.
- `linked_list_program.cpp`: Main driver program for the linked list version.
- `data/`: CSV datasets for Facility A, Facility B, and Facility C (200 records each).

---

## Features

1. **View Patient Records**: Displays formatted patient records (ID, age, age group, care type, stay duration, cost per hour, visits per year, total cost).
2. **Billing and Demographic Reports**:
   - Patient breakdown by age group and care type.
   - Facility expenditure summary and comparison across facilities.
   - Healthcare resource allocation recommendations based on data.
3. **Sorting**:
   - Array: Insertion Sort.
   - Linked List: Bubble Sort.
   - Sortable fields: Age, Visit Duration, Total Cost, Care Type.
   - Always sorts on a copy to preserve original dataset order.
4. **Searching**:
   - Linear search on unsorted data.
   - Binary search on sorted array / Early-stop linear search on sorted linked list.
   - Search filters: Age Group, Care Type, Visit Duration (> X hours).
5. **Performance Testing**:
   - Measures average execution time over 100 runs.
   - Tracks comparison counts, swaps/shifts, and memory usage.

---

## How to Compile and Run

Make sure you are in the project folder containing the `data/` directory.

### Compile using GCC / MinGW:

```cmd
g++ -std=c++17 array_program.cpp -o array_program.exe
g++ -std=c++17 linked_list_program.cpp -o linked_list_program.exe
```

### Run the programs:

```cmd
.\array_program.exe
.\linked_list_program.exe
```

### VS Code:
You can also use VS Code's Terminal > Run Task:
- "Run Array program"
- "Run Singly linked-list program"

---

## Algorithmic Summary

- **Array**:
  - Insertion Sort: Best O(n), Worst O(n^2), Space O(1)
  - Linear Search: O(n)
  - Binary Search: O(log n) + O(k) matches (requires sorted data)
- **Singly Linked List**:
  - Bubble Sort: Best O(n), Worst O(n^2), Space O(1)
  - Linear Search: O(n)
  - Sorted Linear Search: O(n), with early-stop optimization
  - (Binary Search is not practical on singly linked lists due to lack of random access)
