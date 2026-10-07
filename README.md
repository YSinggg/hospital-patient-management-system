# 🏥 Hospital Patient Management System: Array vs. Singly Linked List

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Compiler](https://img.shields.io/badge/Compiler-GCC%20%2F%20MinGW-orange.svg)](https://gcc.gnu.org/)
[![IDE](https://img.shields.io/badge/IDE-VS%20Code-007ACC.svg)](https://code.visualstudio.com/)

A comprehensive C++ comparative data structures study implementing a **Hospital Patient Management and Billing System**. The system evaluates performance, memory overhead, and algorithmic behavior across two distinct core data structures: **Fixed-size Arrays** vs. **Singly Linked Lists**.

---

## 📋 Overview

Modern healthcare facilities handle diverse patient demographics, clinical records, and billing transactions. This project implements identical administrative, analytical, and clinical features on both:
1. **Array-based Implementation (`array_program.cpp`)**
2. **Singly Linked List-based Implementation (`linked_list_program.cpp`)**

It enables empirical performance testing to measure and compare real-world execution times ($\mu s$), comparison counts, dynamic memory overhead, and theoretical Big-O time complexities.

---

## 📂 Project Structure

```text
hospital_project/
├── data/
│   ├── dataset1_facility_a.csv     # 200 patient records (Facility A)
│   ├── dataset2_facility_b.csv     # 200 patient records (Facility B)
│   └── dataset3_facility_c.csv     # 200 patient records (Facility C)
├── .vscode/
│   ├── launch.json                 # GDB debug configurations
│   ├── tasks.json                  # Automated build & run tasks
│   ├── settings.json               # C++17 standard settings
│   └── extensions.json             # Recommended extensions
├── array.hpp                       # Array algorithms (Insertion Sort, Linear & Binary Search)
├── array_program.cpp               # Array application driver & CLI menu
├── linked_list.hpp                 # Linked List algorithms (Bubble Sort, Linear Search, Node ops)
├── linked_list_program.cpp         # Linked List application driver & CLI menu
├── patient.hpp                     # Patient, SearchQuery structs, CSV parsing, print helpers
├── report.hpp                      # Demographic & expenditure reporting logic
├── menu.hpp                        # Interactive console UI and input validators
├── timer.hpp                       # High-resolution microsecond benchmarking timer
├── .gitignore                      # Git ignore rules for binaries and object files
├── README.md                       # Project documentation
└── README.txt                      # Original project brief notes
```

---

## ⚡ Algorithms & Complexity Analysis

| Operation | Array Implementation | Linked List Implementation | Notes |
| :--- | :--- | :--- | :--- |
| **Sorting** | **Insertion Sort**<br>Best: $O(n)$<br>Avg/Worst: $O(n^2)$ | **Bubble Sort**<br>Best: $O(n)$<br>Avg/Worst: $O(n^2)$ | Swaps patient data in adjacent nodes; both sorts operate in $O(1)$ auxiliary space. |
| **Unsorted Search** | **Linear Search**<br>$O(n)$ | **Linear Search**<br>$O(n)$ | Sequential traversal across datasets. |
| **Sorted Search** | **Binary Search**<br>$O(\log n) + O(k)$ matches | **Early-Exit Linear Search**<br>$O(n)$ | Linked lists cannot perform $O(\log n)$ random indexing. |
| **Random Access** | $O(1)$ via index | $O(n)$ pointer traversal | Array supports direct indexing. |
| **Memory Overhead** | $O(1)$ overhead | $O(n)$ pointer overhead | Linked list requires pointer per node (`Node* next`). |

---

## 💡 Core Features

### 1. Patient Record Viewing & Categorization
- Tabular display of patient ID, demographics, admission duration, facility, and age group (Children, Youth, Adult, Senior).

### 2. Billing & Demographic Reports
- **Patient Demographic & Billing Categorization**: Summary tables broken down per age group across Facility A, B, C, or combined facilities.
- **Healthcare Expenditure & Service Analysis**: Inpatient vs. Outpatient cost metrics and service breakdown.
- **Clinical Insights & Recommendations**: Tailored healthcare resource allocation insights.
- **Full Consolidated Report**: Single-command comprehensive report generation.

$$\text{Total Cost} = \text{LengthOfStay} \times \text{BaseCostPerHour} \times \text{DaysVisitsPerYear}$$

### 3. Multi-Field Sorting
- Sort records dynamically by:
  - **Age**
  - **Visit Duration (Length of Stay)**
  - **Total Cost**
  - **Care Type**
- Sorting operates safely on data copies, preserving the original order.

### 4. Search & Filtering
- Search records on unsorted or sorted data:
  - By Age Group
  - By Care Type
  - By Visit Duration exceeding $X$ hours

### 5. Automated Performance Benchmarking
- Runs multi-iteration benchmarks (100 runs per test) to measure:
  - Microsecond execution time ($\mu s$)
  - Total algorithmic comparison counts
  - Heap and stack memory utilization
  - Big-O analysis comparison

---

## 🚀 Getting Started

### Prerequisites
- **GCC / G++ Compiler** (supporting C++17 or higher, e.g., MinGW-w64 on Windows, GCC on Linux/macOS)
- **VS Code** (Optional, recommended for integrated build tasks)

### Compiling via Terminal

Navigate to the project root directory:

```bash
# Compile Array Program
g++ -std=c++17 -O2 array_program.cpp -o array_program

# Compile Linked List Program
g++ -std=c++17 -O2 linked_list_program.cpp -o linked_list_program
```

### Running the Programs

> [!NOTE]
> Ensure programs are executed from the project root folder so the relative path to `data/` is resolved.

**Windows:**
```powershell
.\array_program.exe
# or
.\linked_list_program.exe
```

**Linux / macOS:**
```bash
./array_program
# or
./linked_list_program
```

### Running via VS Code
1. Open the project folder in VS Code (`File > Open Folder`).
2. Press `Ctrl+Shift+B` or go to **Terminal > Run Task...**:
   - Select **`Run Array program`**
   - Or select **`Run Singly linked-list program`**
3. Alternatively, press `F5` to run with the GDB debugger attached.

---

## 📊 Datasets

Located in the `data/` directory:
- `dataset1_facility_a.csv`: 200 patient records from Facility A
- `dataset2_facility_b.csv`: 200 patient records from Facility B
- `dataset3_facility_c.csv`: 200 patient records from Facility C

Data fields include `PatientID`, `Name`, `Age`, `Gender`, `CareType`, `LengthOfStay`, `BaseCostPerHour`, and `DaysVisitsPerYear`.
