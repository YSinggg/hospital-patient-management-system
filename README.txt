HOSPITAL PATIENT MANAGEMENT SYSTEM - ARRAY vs SINGLY LINKED LIST
================================================================

FILES
  Shared by both programs:
    patient.hpp              Constants, Patient and SearchQuery structs,
                             helper functions, reading one CSV line,
                             printing a patient
    timer.hpp                getTimeNow() for measuring time in microseconds
    menu.hpp                 Reading the user's choices (facility, field, search)
    report.hpp               Billing and demographic report

  Array program:
    array.hpp                Load, display, Insertion Sort, Linear Search,
                             Binary Search
    array_program.cpp        main() and the menu screens

  Singly linked list program:
    linked_list.hpp          Node and LinkedList structs, insert/copy/delete,
                             load, display, Bubble Sort, Linear Search
    linked_list_program.cpp  main() and the menu screens

  data/                      The three facility CSV files (200 patients each)

  array_program.cpp includes:        patient.hpp, timer.hpp, menu.hpp, report.hpp, array.hpp
  linked_list_program.cpp includes:  patient.hpp, timer.hpp, menu.hpp, report.hpp, linked_list.hpp

  The functions are written inside the .hpp files, so you only need to
  compile ONE .cpp file for each program. Never compile the two .cpp files
  together, because each one has its own main().

HOW TO RUN (VS CODE)
  1. Open the hospital_project folder in VS Code (File > Open Folder).
  2. Terminal > Run Task > "Run Array program"
     or Terminal > Run Task > "Run Singly linked-list program"

HOW TO RUN (TERMINAL)
  Open a terminal inside the hospital_project folder, then:
    g++ array_program.cpp -o array_program
    g++ linked_list_program.cpp -o linked_list_program
    ./array_program          (Windows: .\array_program.exe)
    ./linked_list_program    (Windows: .\linked_list_program.exe)

  The program must be run from the hospital_project folder so it can
  find the data folder.

MENU (same in both programs)
  1. View patient records (the Age Group column shows each patient's
     age group, brief 4a)
  2. Billing and demographic report
       1. Patient demographic & billing categorization    (brief section 4)
          - choose Facility A, B, C or all facilities combined
          - summary table + one table per age group in the same layout
            as the sample in the brief
       2. Healthcare expenditure & service analysis        (brief section 5)
       3. Clinical insights and recommendations            (brief section 9)
       4. Full report (everything)
  3. Sort patients by age, visit duration, total cost or care type
     (brief 6a, 6b)
  4. Search patients by age group, care type, or visit duration more than X hours,
     on unsorted or sorted data (brief 7a-d)
  5. Performance test: time, comparisons, memory and time complexity for
     all facilities (brief 6c, 6d, 7c, 8a)
  0. Exit

FORMULA
  Total cost = LengthOfStay x BaseCostPerHour x DaysVisitsPerYear

ALGORITHMS
  Array:
    Insertion sort  - best O(n), worst O(n^2), extra space O(1)
    Linear search   - O(n), works on unsorted data
    Binary search   - O(log n) to find the first match + O(k) to list
                      the k matches, data must be sorted first

  Singly linked list:
    Bubble sort     - best O(n), worst O(n^2), extra space O(1)
                      (swaps the patient data inside neighbouring nodes)
    Linear search   - O(n), works on unsorted data
    Linear search on a sorted list can stop early once the value is too
    big, but it is still O(n) in the worst case. Binary search is not
    used because a linked list cannot jump straight to the middle node.

NOTES
  - Sorting is always done on a copy, so the original order is kept for
    the unsorted searches.
  - The performance test repeats every test 100 times and shows the
    average time, because one run of 200 records is too fast to measure
    accurately.
  - Times will be different on every computer. Run option 5 on your own
    computer for the report.
