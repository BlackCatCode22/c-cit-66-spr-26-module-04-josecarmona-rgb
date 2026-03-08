// zooPractice.cpp
// jc 03/02/26
// Project for CIT-66

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

// HELPER FUNCTIONS
vector<string> splitBySpace(const string & str) {
    // Split String Into Words
    vector<string> result;
    stringstream ss(str);
    string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

string getBirthDate(int age, const string & season) {
    // Calculate Birth Year
    int birthYear = 2022 - age;
    // Default Birth Date
    string date =  "Jan 1";

    // Assign Seasonal Date
    if (season == "spring") date = "Mar 21";
    else if (season == "summer") date = "Jun 21";
    else if (season == "fall") date = "Sept 21";
    else if (season == "winter") date = "Dec 21";
    return date + ", " + to_string(birthYear);
}

// CLASS DESIGN
class Animal {
    protected:
    string id, name, species, gender, color, weight, origin, birthDate;
    int age;

public:
    Animal(string id, string name, int age, string species, string gender, string color, string weight, string origin, string birthDate) {
        // Initialize Base Properties
        this->id = id;
        this->name = name;
        this->age = age;
        this->species = species;
        this->gender = gender;
        this->color = color;
        this->weight = weight;
        this->origin = origin;
        this->birthDate = birthDate;
    }

    virtual ~Animal() {
        // Virtual Destructor
    }

    string getSpecies() const {
        // Return Species String
        return species;
    }

    virtual string getHabitat() const = 0;

    string getAnimalData() const {
        // Format Animal Data String
        return id + "; " + name + "; " + to_string(age) + " years old; birth date " + birthDate + "; " + gender + "; " + color + "; " + weight + "; " + origin + "; arrived Sept 27, 2022";
    }
};

// Subclasses Inheriting From Animal
class Hyena : public Animal {
    public:
    Hyena(string id, string n, int a, string g, string c, string w, string o, string b): Animal(id, n, a, "hyena", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Hyena Habitat";
    }
};

class Lion : public Animal {
public:
    Lion(string id, string n, int a, string g, string c, string w, string o, string b) : Animal(id, n, a, "lion", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Lion Habitat";
    }
};

class Tiger : public Animal {
public:
    Tiger(string id, string n, int a, string g, string c, string w, string o, string b) : Animal(id, n, a, "tiger", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Tiger Habitat";
    }
};

class Bear : public Animal {
public:
    Bear(string id, string n, int a, string g, string c, string w, string o, string b) : Animal(id, n, a, "bear", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Bear Habitat";
    }
};

// MAIN PROGRAM
int main() {
    // Store Polymorphic Animal Pointers
    vector<Animal* > zooAnimals;

    // Track Species Counts
    map<string, int> speciesCount;

    // Map Species To Names
    map<string, vector<string>> availableNames;

    // File Stream Object
    fstream myFile;
    string line;

    // Read Animal Names
    myFile.open("animalNames.txt", ios::in);
    if (myFile.is_open()) {
        string currentSpecies = "";
        while (getline(myFile, line)) {
            // Skip Empty Lines
            if (line.empty()) continue;

            // Check Current Species
            if (line.find("Hyena") != string::npos) currentSpecies = "Hyena";
            else if (line.find("Lion") != string::npos) currentSpecies = "Lion";
            else if (line.find("Tiger") != string::npos) currentSpecies = "Tiger";
            else if (line.find("Bear") != string::npos) currentSpecies = "Bear";
            else {
                // Extract & Store Names
                stringstream ss(line);
                string name;
                while (getline(ss, name, ',')) {
                    // Trim Spaces
                    name.erase(0, name.find_first_not_of(" \t"));
                    name.erase(name.find_last_not_of(" \t") + 1);
                    availableNames[currentSpecies].push_back(name);
                }
            }
        }
        myFile.close();
    } else {
        // Print Error Message
        cout << "Unable to open animalNames.txt \n";
    }

    // Read Arriving Animals
    myFile.open("arrivingAnimals.txt", ios::in);
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            // Skip Empty Lines
            if (line.empty()) continue;

            vector<string> parts;
            stringstream ss(line);
            string item;

            // Split By Commas
            while (getline(ss, item, ',')) {
                // Trim Spaces
                item.erase(0, item.find_first_not_of(" \t"));
                item.erase(item.find_last_not_of(" \t") + 1);
                parts.push_back(item);
            }

            // Skip Bad Lines
            if (parts.size() < 6) continue;

            // Extract Age, Gender, Species
            vector<string> part0Words = splitBySpace(parts[0]);
            int age = stoi(part0Words[0]);
            string gender = part0Words[3];
            string species = part0Words[4];

            // Calculate Birth Date
            vector<string> part1Words = splitBySpace(parts[1]);
            string season = part1Words[2];
            string birthDate = getBirthDate(age, season);

            // Get Color & Weight
            string color = parts[2];
            string weight = parts[3];

            // Recreate Origin String
            string origin = parts[4] + ", " + parts[5];

            // Increment Count
            speciesCount[species]++;
            int count = speciesCount[species];
            string idPrefix = species.substr(0, 2);

            // Capitalize Prefix
            idPrefix[0] = toupper(idPrefix[0]);
            string id = idPrefix + (count < 10 ? "0" : "") + to_string(count);

            // Assign Animal Name
            string assignedName = "unknown";
            if (count <= availableNames[species].size()) {
                assignedName = availableNames[species][count - 1];
            }

            // Create Polymorphic Object
            Animal* newAnimal = nullptr;
            if (species == "hyena") newAnimal = new Hyena(id, assignedName, age, gender, color, weight, origin, birthDate);
            else if (species == "lion") newAnimal = new Lion(id, assignedName, age, gender, color, weight, origin, birthDate);
            else if (species == "tiger") newAnimal = new Tiger(id, assignedName, age, gender, color, weight, origin, birthDate);
            else if (species == "bear") newAnimal = new Bear(id, assignedName, age, gender, color, weight, origin, birthDate);

            // Add To Vector
            if (newAnimal) zooAnimals.push_back(newAnimal);
        }
        myFile.close();
    } else {
        // Print Error Message
        cout << "Unable to open arrivingAnimals.txt \n";
    }

    // Write Final Report
    myFile.open("newAnimals.txt", ios::out);
    if (myFile.is_open()) {
        // Print In Specific Order
        string printOrder[] = {"hyena", "lion", "tiger", "bear"};

        for (const string & currentSpecies : printOrder) {
            bool habitatPrinted = false;

            // Print Animals By Species
            for (Animal* animal : zooAnimals) {
                if (animal->getSpecies() == currentSpecies) {

                    // Print Habitat Title
                    if (!habitatPrinted) {
                        myFile << animal->getHabitat() << ": \n\n";
                        habitatPrinted = true;
                    }
                    // Write Animal Data
                    myFile << animal->getAnimalData() << " \n";
                }
            }
            // Add Blank Line
            if (habitatPrinted) myFile << " \n";
        }
        myFile.close();
    } else {
        // Print Error Message
        cout << "Unable to open newAnimals.txt \n";
    }

    // Append Summary Footer
    myFile.open("newAnimals.txt", ios::app);
    if (myFile.is_open()) {
        myFile << "------------------------------------------------------- \n";
        myFile << "End of Zoo Population Report. Total Species Processed: " << speciesCount.size() << " \n";
        myFile.close();
    }

    // Print Success Message
    cout << "Report successfully generated and appended in newAnimals.txt!" << endl;

    // Free Dynamic Memory
    for (Animal* animal : zooAnimals) {
        delete animal;
    }

    // Pause Console
    system("pause>0");

    // Return 0
    return 0;
}