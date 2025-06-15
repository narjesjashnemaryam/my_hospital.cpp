#include "hospital.h"

// Person class implementation
Person::Person(const string &n, int a, const string &g)
    : name(n), age(a), gender(g) {}

void Person::displayInfo() const {
    cout << "Name: " << name << "\nAge: " << age << "\nGender: " << gender << endl;
}

istream &operator>>(istream &in, Person &p) {
    cout << "Enter name: ";
    getline(in, p.name);
    cout << "Enter age: ";
    in >> p.age;
    cout << "Enter gender: ";
    in >> p.gender;
    return in;
}

ostream &operator<<(ostream &out, const Person &p) {
    out << "Name: " << p.name << "\nAge: " << p.age << "\nGender: " << p.gender;
    return out;
}

// Patient class implementation
Patient::Patient(const string &n, int a, const string &g,
                long id, const string &sym, int sev)
    : Person(n, a, g), patientId(id), symptoms(sym), severity(sev),
      isAdmitted(false), bedNumber(-1), medicationCount(0) {}

bool Patient::admitPatient(int bed) {
    if (!isAdmitted) {
        isAdmitted = true;
        bedNumber = bed;
        return true;
    }
    return false;
}

bool Patient::dischargePatient() {
    if (isAdmitted) {
        isAdmitted = false;
        bedNumber = -1;
        return true;
    }
    return false;
}

bool Patient::addMedication(const string &med, const string &time) {
    if (medicationCount < MAX_MEDICATIONS) {
        medications[medicationCount] = {med, time};
        medicationCount++;
        return true;
    }
    return false;
}

void Patient::displayInfo() const {
    Person::displayInfo();
    cout << "Patient ID: " << patientId << "\nSymptoms: " << symptoms
         << "\nSeverity: " << severity << "\nDiagnosis: " << diagnosis
         << "\nAdmitted: " << (isAdmitted ? "Yes" : "No");
    if (isAdmitted) cout << "\nBed Number: " << bedNumber;
    cout << endl;
}

void Patient::showMedications() const {
    cout << "Medications for " << name << ":\n";
    for (int i = 0; i < medicationCount; ++i) {
        cout << "- " << medications[i].name << " at " << medications[i].time << endl;
    }
}

istream &operator>>(istream &in, Patient &p) {
    in >> static_cast<Person&>(p);
    cout << "Enter patient ID: ";
    in >> p.patientId;
    cout << "Enter symptoms: ";
    in.ignore();
    getline(in, p.symptoms);
    cout << "Enter severity (1-5): ";
    in >> p.severity;
    return in;
}

ostream &operator<<(ostream &out, const Patient &p) {
    out << static_cast<const Person&>(p)
       << "\nPatient ID: " << p.patientId
       << "\nSymptoms: " << p.symptoms
       << "\nSeverity: " << p.severity
       << "\nDiagnosis: " << p.diagnosis
       << "\nAdmitted: " << (p.isAdmitted ? "Yes" : "No");
    if (p.isAdmitted) out << "\nBed Number: " << p.bedNumber;
    return out;
}

// HospitalSystem implementation
HospitalSystem::HospitalSystem() : patientCount(0), diseaseCount(0) {
    fill(beds, beds + MAX_BEDS, false);
    initializeDiseases();
}

void HospitalSystem::sortPatientsBySeverity() {
    sort(patients, patients + patientCount, [](const Patient& a, const Patient& b) {
        return a.getSeverity() > b.getSeverity();
    });
}

void HospitalSystem::initializeDiseases() {
    diseases[diseaseCount++] = {"Flu", {"fever", "cough", "headache"}, 3};
    diseases[diseaseCount++] = {"Common Cold", {"runny nose", "sneezing", "sore throat"}, 3};
    diseases[diseaseCount++] = {"COVID-19", {"fever", "cough", "shortness of breath"}, 3};
}

bool HospitalSystem::addPatient(const Patient &p) {
    if (patientCount < MAX_PATIENTS) {
        patients[patientCount++] = p;
        sortPatientsBySeverity();
        saveToFile();
        return true;
    }
    return false;
}

string HospitalSystem::diagnoseDisease(const string &symptoms) {
    int maxMatches = 0;
    string likelyDisease = "Unknown";

    for (int i = 0; i < diseaseCount; ++i) {
        int matches = 0;
        for (int j = 0; j < diseases[i].symptomCount; ++j) {
            if (symptoms.find(diseases[i].symptoms[j]) != string::npos) {
                matches++;
            }
        }
        if (matches > maxMatches) {
            maxMatches = matches;
            likelyDisease = diseases[i].name;
        }
    }

    return maxMatches > 0 ? "Possible diagnosis: " + likelyDisease : "No specific diagnosis";
}

bool HospitalSystem::admitPatient(long patientId) {
    for (int i = 0; i < patientCount; ++i) {
        if (patients[i].getId() == patientId && !patients[i].getAdmissionStatus()) {
            int bed = findAvailableBed();
            if (bed != -1) {
                patients[i].admitPatient(bed);
                beds[bed] = true;
                saveToFile();
                return true;
            }
        }
    }
    return false;
}

bool HospitalSystem::dischargePatient(long patientId) {
    for (int i = 0; i < patientCount; ++i) {
        if (patients[i].getId() == patientId && patients[i].getAdmissionStatus()) {
            beds[patients[i].getBedNumber()] = false;
            patients[i].dischargePatient();
            saveToFile();
            return true;
        }
    }
    return false;
}

int HospitalSystem::findAvailableBed() const {
    for (int i = 0; i < MAX_BEDS; ++i) {
        if (!beds[i]) return i;
    }
    return -1;
}

void HospitalSystem::bookAppointment(const string &name, int age, const string &gender,
                                   const string &symptoms) {
    if (patientCount < MAX_PATIENTS) {
        long newId = patientCount == 0 ? 1 : patients[patientCount-1].getId() + 1;
        Patient newPatient(name, age, gender, newId, symptoms, 1);
        newPatient.setDiagnosis(diagnoseDisease(symptoms));
        addPatient(newPatient);
        cout << "Appointment booked. Patient ID: " << newId << endl;
    } else {
        cout << "Hospital at full capacity." << endl;
    }
}

bool HospitalSystem::addMedication(long patientId, const string &med, const string &time) {
    for (int i = 0; i < patientCount; ++i) {
        if (patients[i].getId() == patientId) {
            bool success = patients[i].addMedication(med, time);
            if (success) saveToFile();
            return success;
        }
    }
    return false;
}

void HospitalSystem::showMedicationReminders(long patientId) const {
    for (int i = 0; i < patientCount; ++i) {
        if (patients[i].getId() == patientId) {
            patients[i].showMedications();
            return;
        }
    }
    cout << "Patient not found." << endl;
}

void HospitalSystem::saveToFile() const {
    ofstream outFile("hospital_data.txt");
    if (outFile) {
        for (int i = 0; i < patientCount; ++i) {
            outFile << patients[i] << "\n---\n";
        }
    }
}

void HospitalSystem::loadFromFile() {
    ifstream inFile("hospital_data.txt");
    if (inFile) {
        patientCount = 0;
        string line;

        while (getline(inFile, line)) {
            if (line.find("Name: ") == 0) {
                Patient p;
                p.setName(line.substr(6));

                getline(inFile, line); p.setAge(stoi(line.substr(5)));
                getline(inFile, line); p.setGender(line.substr(8));
                getline(inFile, line); p.setPatientId(stol(line.substr(12)));
                getline(inFile, line); p.setSymptoms(line.substr(11));
                getline(inFile, line); p.setSeverity(stoi(line.substr(10)));
                getline(inFile, line); p.setDiagnosis(line.substr(11));
                getline(inFile, line); p.setIsAdmitted(line.substr(10) == "Yes");
                if (p.getAdmissionStatus()) {
                    getline(inFile, line); p.setBedNumber(stoi(line.substr(12)));
                }

                patients[patientCount++] = p;
            }
        }
    }
}

void HospitalSystem::displayAllPatients() const {
    for (int i = 0; i < patientCount; ++i) {
        patients[i].displayInfo();
        cout << "-----------------\n";
    }
}
