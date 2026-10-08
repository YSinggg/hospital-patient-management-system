#ifndef REPORT_HPP
#define REPORT_HPP

#include "patient.hpp"
#include "menu.hpp"

// Summary totals for each facility
struct FacilityStats {
    int patients;
    double totalCost;
    int totalStay;
    int longStays;

    int ageCount[NUM_AGE_GROUPS];
    double ageCost[NUM_AGE_GROUPS];
    int ageStay[NUM_AGE_GROUPS];

    int careCount[NUM_CARE_TYPES];
    double careCost[NUM_CARE_TYPES];
    int careStay[NUM_CARE_TYPES];

    int careByAge[NUM_AGE_GROUPS][NUM_CARE_TYPES];
    double careCostByAge[NUM_AGE_GROUPS][NUM_CARE_TYPES];
};

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

void printAverageMoney(double total, int count, int width) {
    if (count > 0) {
        cout << setw(width) << formatMoney(total / count);
    } else {
        cout << setw(width) << "N/A";
    }
}

void printAverageHours(double total, int count, int width) {
    if (count > 0) {
        cout << setw(width) << fixed << setprecision(2) << total / count;
    } else {
        cout << setw(width) << "N/A";
    }
}

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

void printCompareHeader(string firstColumn) {
    printLine(110);
    cout << left << setw(42) << firstColumn << right << setw(17) << "Facility A"
         << setw(17) << "Facility B" << setw(17) << "Facility C" << setw(17) << "All Facilities" << endl;
    printLine(110);
}

// Display table for a specific age group
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

    bool printed[NUM_CARE_TYPES] = {false};
    for (int row = 0; row < NUM_CARE_TYPES; row++) {
        int best = -1;
        for (int care = 0; care < NUM_CARE_TYPES; care++) {
            if (!printed[care] && stats.careByAge[group][care] > 0) {
                if (best == -1 || stats.careByAge[group][care] > stats.careByAge[group][best]) {
                    best = care;
                }
            }
        }
        if (best == -1) {
            break;
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

// Patient demographic and billing categorization
void printSection4(const FacilityStats &stats, string title) {
    cout << "\n=====================================================================\n";
    cout << " PATIENT DEMOGRAPHIC & BILLING CATEGORIZATION\n";
    cout << " " << title << endl;
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

// Healthcare expenditure and service analysis
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
    cout << " HEALTHCARE EXPENDITURE & SERVICE ANALYSIS\n";
    cout << "=====================================================================\n";

    cout << "\nTotal medical billing per facility\n";
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

    cout << "\nTotal medical cost (MYR) by care type\n";
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

    cout << "\nTotal medical cost (MYR) by age group\n";
    printCompareHeader("Age Group");
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            cout << setw(17) << formatMoney(stats[facility].ageCost[group]);
        }
        cout << setw(17) << formatMoney(all.ageCost[group]) << endl;
    }

    cout << "\nAverage cost per patient (MYR) by age group\n";
    printCompareHeader("Age Group");
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageMoney(stats[facility].ageCost[group], stats[facility].ageCount[group], 17);
        }
        printAverageMoney(all.ageCost[group], all.ageCount[group], 17);
        cout << endl;
    }

    cout << "\nAverage visit duration (hours) by age group\n";
    printCompareHeader("Age Group");
    for (int group = 0; group < NUM_AGE_GROUPS; group++) {
        cout << left << setw(42) << getAgeGroupName(group) << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageHours(stats[facility].ageStay[group], stats[facility].ageCount[group], 17);
        }
        printAverageHours(all.ageStay[group], all.ageCount[group], 17);
        cout << endl;
    }

    cout << "\nAverage visit duration (hours) by care type\n";
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

// Clinical insights and recommendations
void printSection9(FacilityStats stats[], const FacilityStats &all) {
    cout << "\n=====================================================================\n";
    cout << " CLINICAL INSIGHTS AND RECOMMENDATIONS\n";
    cout << "=====================================================================\n";

    cout << "\nService preference: patient count by care type\n";
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

    cout << "\nTreatment cost: average cost per patient (MYR) by care type\n";
    printCompareHeader("Care Type");
    for (int care = 0; care < NUM_CARE_TYPES; care++) {
        cout << left << setw(42) << careTypes[care] << right;
        for (int facility = 0; facility < NUM_FACILITIES; facility++) {
            printAverageMoney(stats[facility].careCost[care], stats[facility].careCount[care], 17);
        }
        printAverageMoney(all.careCost[care], all.careCount[care], 17);
        cout << endl;
    }

    cout << "\nMost requested care type by age group\n";
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

    cout << "\nHighest billing demographic and patient traffic summary\n";
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

    cout << "\nRecommendations for hospital resource planning\n";
    cout << "1. " << careTypes[busiestCare] << " recorded highest patient volume. Consider allocating\n"
         << "   more counters and dedicated staff to reduce waiting time.\n";
    cout << "2. " << getAgeGroupName(costliestAge) << " incurs the highest total billing.\n"
         << "   Budget more specialized equipment and bed capacity for this demographic.\n";
    cout << "3. " << facilityNames[mostLongStays] << " has the highest number of extended stays (> 24 hours).\n"
         << "   Review admission criteria and bed turnover management to optimize capacity.\n";
    cout << "4. Facility-specific staffing should align with respective peak care types,\n"
         << "   as patient demand varies across each location.\n";
}

void reportMenu(FacilityStats stats[]) {
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
