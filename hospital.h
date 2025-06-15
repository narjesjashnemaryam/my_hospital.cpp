#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>

using namespace std;

const int MAX_PATIENTS = 100;          //تعداد بیماران
const int MAX_BEDS = 50;               //تعداد تخت ها
const int MAX_MEDICATIONS = 20;        //تعداد  داروها
const int MAX_SYMPTOMS = 5;           // تعداد علائم بیماری
const int MAX_DISEASES = 10;          //بیماری

// Base Person class
class Person {
protected:
    string name;
    int age;
    string gender;

public:
    Person(const string &n = "", int a = 0, const string &g = "");    // سازنده پارامتری با مقادیر پیش فرض

    // Setter
    void setName(const string &n) {
         name = n;
         }
    void setAge(int a) {
         age = a;
         }
    void setGender(const string &g) {
         gender = g;
         }

    // Getters
    string getName() const {
         return name;
         }
    int getAge() const {
         return age;
         }
    string getGender() const {
         return gender;
         }

    virtual void displayInfo() const;
    friend istream &operator>>(istream &in, Person &p);
    friend ostream &operator<<(ostream &out, const Person &p);
    virtual ~Person() {}
};

// Patient class derived from Person
class Patient : public Person {
private:
    long patientId;    // شناسه بیمار
    string symptoms;   // علایم بیماری
    int severity;      //شدت بیماری
    string diagnosis;  // تشخیص پزشک
    bool isAdmitted;   //وضعیت بستری
    int bedNumber;     //شماره تخت

// ساختار دارو

    struct Medication {
        string name;
        string time;
    };

    Medication medications[MAX_MEDICATIONS];
    int medicationCount;

public:
    Patient(const string &n = "", int a = 0, const string &g = "",
            long id = 0, const string &sym = "", int sev = 1);

    // Setters
    void setPatientId(long id) {
         patientId = id;
         }
    void setSymptoms(const string &sym) {
         symptoms = sym;
         }
    void setSeverity(int sev) {
         severity = sev;
         }
    void setDiagnosis(const string &diag) {
         diagnosis = diag;
         }
    void setIsAdmitted(bool admitted) {
         isAdmitted = admitted;
         }
    void setBedNumber(int bed) {
         bedNumber = bed;
         }

    // Getters
    long getId() const {
         return patientId;
         }
    string getSymptoms() const {
         return symptoms;
         }
    int getSeverity() const {
         return severity;
         }
    string getDiagnosis() const {
         return diagnosis;
         }
    bool getAdmissionStatus() const {
         return isAdmitted;
         }
    int getBedNumber() const {
         return bedNumber;
         }
    int getMedicationCount() const {
         return medicationCount;
         }
    Medication getMedication(int index) const {
         return medications[index];
         }

    bool admitPatient(int bed);
    bool dischargePatient();
    bool addMedication(const string &med, const string &time);
    void displayInfo() const override;
    void showMedications() const;

    friend istream &operator>>(istream &in, Patient &p);
    friend ostream &operator<<(ostream &out, const Patient &p);
};

// Hospital System class
class HospitalSystem {
private:
    Patient patients[MAX_PATIENTS];
    int patientCount;
    bool beds[MAX_BEDS];

    struct Disease {
        string name;
        string symptoms[MAX_SYMPTOMS];
        int symptomCount;
    };

    Disease diseases[MAX_DISEASES];
    int diseaseCount;

    void sortPatientsBySeverity();
    void initializeDiseases();

public:
    HospitalSystem();

    // Patient management
    bool addPatient(const Patient &p);
    int getPatientCount() const { return patientCount; }
    Patient getPatient(int index) const { return patients[index]; }

    // Diagnosis
    string diagnoseDisease(const string &symptoms);

    // Admission/discharge
    bool admitPatient(long patientId);
    bool dischargePatient(long patientId);
    int findAvailableBed() const;
    bool isBedAvailable(int bed) const { return !beds[bed]; }

    // Appointment booking
    void bookAppointment(const string &name, int age, const string &gender,
                        const string &symptoms);

    // Medication management
    bool addMedication(long patientId, const string &med, const string &time);
    void showMedicationReminders(long patientId) const;

    // Data persistence
    void saveToFile() const;
    void loadFromFile();

    // Display
    void displayAllPatients() const;
};

#endif
