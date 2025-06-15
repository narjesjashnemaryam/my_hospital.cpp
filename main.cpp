#include "hospital.h"
#include <iostream>

using namespace std;

void displayMenu() {
    cout << "\nHospital Management System\n";
    cout << "1. Book Appointment\n";
    cout << "2. Admit Patient\n";
    cout << "3. Discharge Patient\n";
    cout << "4. Add Medication\n";
    cout << "5. Show Medication Reminders\n";
    cout << "6. Display All Patients\n";
    cout << "7. Find Available Bed\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    HospitalSystem hospital;
    hospital.loadFromFile();

    int choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string name, gender, symptoms;
                int age;

                cout << "Patient name: ";
                getline(cin, name);
                cout << "Age: ";
                cin >> age;
                cout << "Gender: ";
                cin >> gender;
                cout << "Symptoms: ";
                cin.ignore();
                getline(cin, symptoms);

                hospital.bookAppointment(name, age, gender, symptoms);
                break;
            }
            case 2: {
                long id;
                cout << "Patient ID to admit: ";
                cin >> id;
                if (hospital.admitPatient(id)) {
                    cout << "Patient admitted successfully.\n";
                } else {
                    cout << "Admission failed.\n";
                }
                break;
            }
            case 3: {
                long id;
                cout << "Patient ID to discharge: ";
                cin >> id;
                if (hospital.dischargePatient(id)) {
                    cout << "Patient discharged successfully.\n";
                } else {
                    cout << "Discharge failed.\n";
                }
                break;
            }
            case 4: {
                long id;
                string med, time;
                cout << "Patient ID: ";
                cin >> id;
                cout << "Medication name: ";
                cin.ignore();
                getline(cin, med);
                cout << "Time: ";
                getline(cin, time);

                if (hospital.addMedication(id, med, time)) {
                    cout << "Medication added successfully.\n";
                } else {
                    cout << "Failed to add medication.\n";
                }
                break;
            }
            case 5: {
                long id;
                cout << "Patient ID: ";
                cin >> id;
                hospital.showMedicationReminders(id);
                break;
            }
            case 6: {
                hospital.displayAllPatients();
                break;
            }
            case 7: {
                int bed = hospital.findAvailableBed();
                if (bed != -1) {
                    cout << "Available bed found. Bed number: " << bed << endl;
                } else {
                    cout << "No available beds.\n";
                }
                break;
            }
            case 8:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please enter 1-8.\n";
        }
    } while (choice != 8);

    return 0;
}
