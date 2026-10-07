#ifndef REPORT_HPP
#define REPORT_HPP

#include "patient.hpp"
#include "menu.hpp"

// Totals for ONE facility (or for all facilities added together)
struct FacilityStats {
    int patients;
    double totalCost;
    int totalStay;                                        // total hours of all patients
    int longStays;                                        // stays more than 24 hours

    int ageCount[NUM_AGE_GROUPS];                         // patients in each age group
    double ageCost[NUM_AGE_GROUPS];                       // total cost of each age group
    int ageStay[NUM_AGE_GROUPS];                          // total hours of each age group

    int careCount[NUM_CARE_TYPES];                        // patients in each care type
    double careCost[NUM_CARE_TYPES];                      // total cost of each care type
    int careStay[NUM_CARE_TYPES];                         // total hours of each care type

    int careByAge[NUM_AGE_GROUPS][NUM_CARE_TYPES];        // patients per care type in each age group
    double careCostByAge[NUM_AGE_GROUPS][NUM_CARE_TYPES]; // cost per care type in each age group
};

// Filling in the totals

// Set every total back to 0
void resetStats(FacilityStats &stats) {
    stats.patients = 0;
    stats.totalCost = 0;
    stats.totalStay = 0;
    stats.longStays = 0;
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        stats.ageCount[group] = 0;
        stats.ageCost[group] = 0;
        stats.ageStay[group] = 0;
        for (int care = 0; care < NUM_CARE_TYPES; care++) {
            stats.careByAge[group][care] = 0;
            stats.careCostByAge[group][care] = 0;
        }
    }
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        stats.careCount[care] = 0;
        stats.careCost[care] = 0;
        stats.careStay[care] = 0;
    }
}

// Add one patient to the totals
void addPatientToStats(FacilityStats &stats, const Patient &p) {
    int group = getAgeGroup(p.age);
    int care = p.careNumber;

    stats.patients++;
    stats.totalCost += p.totalCost;
    stats.totalStay += p.lengthOfStay;
    if (p.lengthOfStay > 24) stats.longStays++;

    stats.ageCount[group]++;
    stats.ageCost[group] += p.totalCost;
    stats.ageStay[group] += p.lengthOfStay;

    stats.careCount[care]++;
    stats.careCost[care] += p.totalCost;
    stats.careStay[care] += p.lengthOfStay;

    stats.careByAge[group][care]++;
    stats.careCostByAge[group][care] += p.totalCost;
}

// Add all the totals of one facility into "total" (used to get ALL FACILITIES)
void addStats(FacilityStats &total, const FacilityStats &stats) {
    total.patients += stats.patients;
    total.totalCost += stats.totalCost;
    total.totalStay += stats.totalStay;
    total.longStays += stats.longStays;
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        total.ageCount[group] += stats.ageCount[group];
        total.ageCost[group] += stats.ageCost[group];
        total.ageStay[group] += stats.ageStay[group];
        for (int care = 0; care < NUM_CARE_TYPES; care++) {
            total.careByAge[group][care] += stats.careByAge[group][care];
            total.careCostByAge[group][care] += stats.careCostByAge[group][care];
        }
    }
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        total.careCount[care] += stats.careCount[care];
        total.careCost[care] += stats.careCost[care];
        total.careStay[care] += stats.careStay[care];
    }
}

// Small printing helpers

// Prints the average money amount (total / count), or N/A if count is 0
void printAverageMoney(double total, int count, int width) {
    if (count > 0) {
        cout << setw(width) << formatMoney(total / count);
    } else {
        cout << setw(width) << "N/A";
    }
}

// Prints the average number of hours (total / count), or N/A if count is 0
void printAverageHours(double total, int count, int width) {
    if (count > 0) {
        cout << setw(width) << fixed << setprecision(2) << total / count;
    } else {
        cout << setw(width) << "N/A";
    }
}

// Returns the care type with the most patients, e.g. "Outpatient (36)".
// unit is added after the number, e.g. " patients" gives "Outpatient (36 patients)".
// If two care types have the same number, both are shown.
string mostRequestedCare(const int counts[], string unit) {
    int most = 0;
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        if (counts[care] > most) most = counts[care];
    }
    if (most == 0) {
        return "N/A";
    }
    string result = "";
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        if (counts[care] == most) {
            if (result != "") result += " / ";
            result += careTypes[care];
        }
    }
    return result + " (" + to_string(most) + unit + ")";
}

// Header for the tables that compare Facility A, B and C side by side
void printCompareHeader(string firstColumn) {
    printLine(110);
    cout << left << setw(42) << firstColumn << right << setw(17) << "Facility A"
         << setw(17) << "Facility B" << setw(17) << "Facility C" << setw(17) << "All Facilities" << endl;
    printLine(110);
}

// SECTION 4: PATIENT DEMOGRAPHIC & BILLING CATEGORIZATION

// One age group, in the same layout as the sample table in the brief.
// Care types are listed from the most patients to the fewest.
void printAgeGroupTable(const FacilityStats &stats, int group) {
    cout << "\nAge Group " << getAgeRange(group) << ": " << ageGroupLabels[group] << endl;
    printLine(92);
    cout << left << setw(20) << "Care Type" << right << setw(15) << "Patient Count"
         << setw(22) << "Total Cost (MYR)" << setw(35) << "Average Cost per Patient (MYR)" << endl;
    printLine(92);

    if (stats.ageCount[group] == 0) {
        cout << "No patients in this age group.\n";
        printLine(92);
        return;
    }

    bool printed[NUM_CARE_TYPES] = {false};   // which care types are already printed
    for (int row = 0; row < NUM_CARE_TYPES; row++) {
        // find the care type with the most patients that is not printed yet
        int best = -1;
        for (int care = 0; care < NUM_CARE_TYPES; care++) {
            if (!printed[care] && stats.careByAge[group][care] > 0) {
                if (best == -1 || stats.careByAge[group][care] > stats.careByAge[group][best]) {
                    best = care;
                }
            }
        }
        if (best == -1) {
            break;   // every care type with patients has been printed
        }
        printed[best] = true;

        int count = stats.careByAge[group][best];
        double cost = stats.careCostByAge[group][best];
        cout << left << setw(20) << careTypes[best] << right << setw(15) << count
             << setw(22) << formatMoney(cost) << setw(35) << formatMoney(cost / count) << endl;
    }
    printLine(92);
    cout << "Total Billing for Age Group : MYR " << formatMoney(stats.ageCost[group]) << endl;
    cout << "Number of Patients          : " << stats.ageCount[group] << endl;
    cout << "Average Cost per Patient    : MYR " << formatMoney(stats.ageCost[group] / stats.ageCount[group])
         << "   (" << formatMoney(stats.ageCost[group]) << " / " << stats.ageCount[group] << ")\n";
    cout << "Most Requested Care Type    : " << mostRequestedCare(stats.careByAge[group], " patients") << endl;
}

// Section 4 for one facility (or for all facilities together)
void printSection4(const FacilityStats &stats, string title) {
    cout << "\n=====================================================================\n";
    cout << " 4. PATIENT DEMOGRAPHIC & BILLING CATEGORIZATION\n";
    cout << "    " << title << endl;
    cout << "=====================================================================\n";
    cout << "Cost = Length of Stay x Base Cost Per Hour x Days Visits Per Year\n";
    cout << "Average Cost = Total Medical Cost in Age Group / Number of Patients in Age Group\n";

    cout << "\nSummary by age group\n";
    printLine(130);
    cout << left << setw(42) << "Age Group" << right << setw(10) << "Patients"
         << setw(20) << "Total Cost (MYR)" << setw(28) << "Avg Cost per Patient (MYR)"
         << "   " << left << "Most Requested Care Type (patients)" << endl;
    printLine(130);
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right << setw(10) << stats.ageCount[group]
             << setw(20) << formatMoney(stats.ageCost[group]);
        printAverageMoney(stats.ageCost[group], stats.ageCount[group], 28);
        cout << "   " << mostRequestedCare(stats.careByAge[group], "") << endl;
    }
    printLine(130);
    cout << left << setw(42) << "TOTAL" << right << setw(10) << stats.patients
         << setw(20) << formatMoney(stats.totalCost);
    printAverageMoney(stats.totalCost, stats.patients, 28);
    cout << endl;

    cout << "\nDetails for each age group\n";
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        printAgeGroupTable(stats, group);
    }
}

// SECTION 5: HEALTHCARE EXPENDITURE & SERVICE ANALYSIS

// One row of the 5a table
void printBillingRow(string name, const FacilityStats &stats) {
    cout << left << setw(42) << name << right << setw(10) << stats.patients
         << setw(20) << formatMoney(stats.totalCost);
    printAverageMoney(stats.totalCost, stats.patients, 20);
    cout << setw(12) << stats.totalStay;
    printAverageHours(stats.totalStay, stats.patients, 12);
    cout << setw(12) << stats.longStays << endl;
}

void printSection5(FacilityStats stats[], const FacilityStats &all) {
    cout << "\n=====================================================================\n";
    cout << " 5. HEALTHCARE EXPENDITURE & SERVICE ANALYSIS\n";
    cout << "=====================================================================\n";

    // ---- 5a: total billing per dataset ----
    cout << "\n5a. Total medical billing per dataset\n";
    printLine(128);
    cout << left << setw(42) << "Facility" << right << setw(10) << "Patients"
         << setw(20) << "Total Cost (MYR)" << setw(20) << "Avg Cost (MYR)"
         << setw(12) << "Total Hours" << setw(12) << "Avg Hours" << setw(12) << "Stays > 24h" << endl;
    printLine(128);
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        printBillingRow(facilityNames[facility], stats[facility]);
    }
    printLine(128);
    printBillingRow("ALL FACILITIES", all);

    // ---- 5b: total cost grouped by care type ----
    cout << "\n5b. Total medical cost (MYR) by care type\n";
    printCompareHeader("Care Type");
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        cout << left << setw(42) << careTypes[care] << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            cout << setw(17) << formatMoney(stats[facility].careCost[care]);
        }
        cout << setw(17) << formatMoney(all.careCost[care]) << endl;
    }
    printLine(110);
    cout << left << setw(42) << "TOTAL" << right;
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        cout << setw(17) << formatMoney(stats[facility].totalCost);
    }
    cout << setw(17) << formatMoney(all.totalCost) << endl;

    // ---- 5c: compare expenditure and visit durations across datasets and age groups ----
    cout << "\n5c. Total medical cost (MYR) by age group\n";
    printCompareHeader("Age Group");
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            cout << setw(17) << formatMoney(stats[facility].ageCost[group]);
        }
        cout << setw(17) << formatMoney(all.ageCost[group]) << endl;
    }

    cout << "\n5c. Average cost per patient (MYR) by age group\n";
    printCompareHeader("Age Group");
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageMoney(stats[facility].ageCost[group], stats[facility].ageCount[group], 17);
        }
        printAverageMoney(all.ageCost[group], all.ageCount[group], 17);
        cout << endl;
    }

    cout << "\n5c. Average visit duration (hours) by age group\n";
    printCompareHeader("Age Group");
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageHours(stats[facility].ageStay[group], stats[facility].ageCount[group], 17);
        }
        printAverageHours(all.ageStay[group], all.ageCount[group], 17);
        cout << endl;
    }

    cout << "\n5c. Average visit duration (hours) by care type\n";
    printCompareHeader("Care Type");
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        cout << left << setw(42) << careTypes[care] << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageHours(stats[facility].careStay[care], stats[facility].careCount[care], 17);
        }
        printAverageHours(all.careStay[care], all.careCount[care], 17);
        cout << endl;
    }
    cout << "N/A = the facility has no patients in that group.\n";
}

// ============================================================
// SECTION 9: CLINICAL INSIGHTS AND RECOMMENDATIONS
// ============================================================
void printSection9(FacilityStats stats[], const FacilityStats &all) {
    cout << "\n=====================================================================\n";
    cout << " 9. CLINICAL INSIGHTS AND RECOMMENDATIONS\n";
    cout << "=====================================================================\n";

    // ---- 9a: treatment costs and service preferences across datasets and age groups ----
    cout << "\n9a. Service preference: number of patients by care type\n";
    printCompareHeader("Care Type");
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        cout << left << setw(42) << careTypes[care] << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            cout << setw(17) << stats[facility].careCount[care];
        }
        cout << setw(17) << all.careCount[care] << endl;
    }
    printLine(110);
    cout << left << setw(42) << "TOTAL" << right;
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        cout << setw(17) << stats[facility].patients;
    }
    cout << setw(17) << all.patients << endl;

    cout << "\n9a. Treatment cost: average cost per patient (MYR) by care type\n";
    printCompareHeader("Care Type");
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        cout << left << setw(42) << careTypes[care] << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageMoney(stats[facility].careCost[care], stats[facility].careCount[care], 17);
        }
        printAverageMoney(all.careCost[care], all.careCount[care], 17);
        cout << endl;
    }

    cout << "\n9a. Service preference: most requested care type by age group (number of patients)\n";
    printLine(130);
    cout << left << setw(42) << "Age Group" << setw(22) << "Facility A" << setw(22) << "Facility B"
         << setw(22) << "Facility C" << setw(22) << "All Facilities" << endl;
    printLine(130);
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group);
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            cout << setw(22) << mostRequestedCare(stats[facility].careByAge[group], "");
        }
        cout << setw(22) << mostRequestedCare(all.careByAge[group], "") << endl;
    }
    printLine(130);

    // ---- 9b: highest billing demographic and highest patient traffic ----
    int costliestAge = 0;
    int highestAvgAge = 0;
    double highestAvg = 0;
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        if (all.ageCost[group] > all.ageCost[costliestAge]) costliestAge = group;
        if (all.ageCount[group] > 0 && all.ageCost[group] / all.ageCount[group] > highestAvg) {
            highestAvg = all.ageCost[group] / all.ageCount[group];
            highestAvgAge = group;
        }
    }
    int busiestCare = 0;
    int costliestCare = 0;
    for (int care = 1; care < NUM_CARE_TYPES; care++) {
        if (all.careCount[care] > all.careCount[busiestCare]) busiestCare = care;
        if (all.careCost[care] > all.careCost[costliestCare]) costliestCare = care;
    }
    int costliestFacility = 0;
    int mostLongStays = 0;
    for (int facility = 1; facility < NUM_FACILITIES; facility++) {
        if (stats[facility].totalCost > stats[costliestFacility].totalCost) costliestFacility = facility;
        if (stats[facility].longStays > stats[mostLongStays].longStays) mostLongStays = facility;
    }

    cout << "\n9b. Highest billing and highest patient traffic (all facilities)\n";
    printLine(110);
    cout << "Age group with highest total billing  : " << getAgeGroupName(costliestAge)
         << " (MYR " << formatMoney(all.ageCost[costliestAge]) << ")\n";
    cout << "Age group with highest average cost   : " << getAgeGroupName(highestAvgAge)
         << " (MYR " << formatMoney(highestAvg) << " per patient)\n";
    cout << "Care type with highest billing        : " << careTypes[costliestCare]
         << " (MYR " << formatMoney(all.careCost[costliestCare]) << ")\n";
    cout << "Care type with highest patient traffic: " << careTypes[busiestCare]
         << " (" << all.careCount[busiestCare] << " patients)\n";
    cout << "Facility with highest billing         : " << facilityNames[costliestFacility]
         << " (MYR " << formatMoney(stats[costliestFacility].totalCost) << ")\n";
    cout << "Facility with most stays over 24 hours: " << facilityNames[mostLongStays]
         << " (" << stats[mostLongStays].longStays << " patients)\n";
    cout << "Busiest care type in each facility    :\n";
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        cout << "   " << left << setw(40) << facilityNames[facility]
             << mostRequestedCare(stats[facility].careCount, " patients") << endl;
    }
    printLine(110);

    // ---- 9c: recommendations ----
    cout << "\n9c. Recommendations for hospital administrators\n";
    cout << "1. " << careTypes[busiestCare] << " has the most patients. Add more registration counters,\n"
         << "   staff and appointment slots for this care type to shorten queues.\n";
    cout << "2. The " << getAgeGroupName(costliestAge) << " group produces the highest bill.\n"
         << "   Plan more beds, specialist staff and follow-up care for this age group.\n";
    cout << "3. " << facilityNames[mostLongStays] << " has the most stays over 24 hours.\n";
    cout << "4. Each facility should put more staff on its own busiest care type (see above),\n"
         << "   because each facility serves a different age group and care mix.\n";
}

// Report menu
// stats has one FacilityStats for each facility (already filled in)
void reportMenu(FacilityStats stats[]) {
    // Add the three facilities together to get ALL FACILITIES
    FacilityStats all;
    resetStats(all);
    for (int facility = 0; facility < NUM_FACILITIES; facility++) {
        addStats(all, stats[facility]);
    }

    cout << "\nBILLING AND DEMOGRAPHIC REPORT\n";
    cout << "1. Patient demographic & billing categorization\n";
    cout << "2. Healthcare expenditure & service analysis\n";
    cout << "3. Clinical insights and recommendations\n";
    cout << "4. Full report (everything)\n";
    int part = readNumber("Choose report: ", 1, 4);

    if (part == 1) {
        cout << "\n1. " << facilityNames[0] << "\n2. " << facilityNames[1]
             << "\n3. " << facilityNames[2] << "\n4. All facilities combined\n";
        int choice = readNumber("Choose facility: ", 1, 4);
        if (choice == 4) {
            printSection4(all, "ALL FACILITIES COMBINED");
        } else {
            printSection4(stats[choice - 1], facilityNames[choice - 1]);
        }
    } else if (part == 2) {
        printSection5(stats, all);
    } else if (part == 3) {
        printSection9(stats, all);
    } else {
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printSection4(stats[facility], facilityNames[facility]);
        }
        printSection4(all, "ALL FACILITIES COMBINED");
        printSection5(stats, all);
        printSection9(stats, all);
    }
}

#endif
