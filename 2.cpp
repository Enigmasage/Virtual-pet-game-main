

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <map>
#include <conio.h>
#include <windows.h>
#include <iomanip>
using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void getHiddenPassword(string &password) {
    char ch;
    password = "";

    while ((ch = _getch()) != '\r') {   // stop when Enter is pressed
        if (ch == '\b') {               // backspace pressed
            if (!password.empty()) {
                password = password.substr(0, password.size() - 1);
                cout << "\b \b";        // erase * from screen
            }
        } 
        else {                          // normal character typed
            password += ch;             // store character
            cout << '*';                // show * instead of character
        }
    }

    cout << endl;
}

void registerUser() {
    string username, password;
    setColor(12);
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter password: ";
    getHiddenPassword(password);

    ofstream userFile("users.txt", ios::app);
    if (userFile.is_open()) {
        userFile << username << " " << password << endl;
        userFile.close();
    }

    setColor(10);
    cout << "Registration successful!" << endl;
    setColor(7);
}

bool loginUser(string &username) {
    string inputUser, inputPass, fileUser, filePass;
    setColor(11);
    cout << "Enter username: ";
    cin >> inputUser;
    cout << "Enter password: ";
    getHiddenPassword(inputPass);

    ifstream userFile("users.txt");
    if (userFile.is_open()) {
        while (userFile >> fileUser >> filePass) {
            if (fileUser == inputUser && filePass == inputPass) {
                username = inputUser;
                setColor(10);
                cout << "Login successful!" << endl;
                setColor(7);
                return true;
            }
        }
    }

    setColor(12);
    cout << "Invalid username or password!" << endl;
    setColor(7);
    return false;
}

bool adminLogin() {
    string password;
    setColor(11);
    cout << "\nEnter Admin Password: ";
    getHiddenPassword(password);
    setColor(7);
    return (password == "admin123");
}

void viewRegisteredUsers() {
    ifstream userFile("users.txt");
    string user, pass;
    setColor(14);
    cout << "\nRegistered Users:\n";
    setColor(7);
    while (userFile >> user >> pass) {
        cout << "- " << user << endl;
    }
}

void modifyFoodPreferences(const string &username) {
    string filename = username + "_savegame.txt";
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "No save file found for user." << endl;
        return;
    }

    string name, type, fav_food, allergic_food;
    int hunger, energy, happiness, points;
    file >> name >> type >> fav_food >> allergic_food >> hunger >> energy >> happiness >> points;
    file.close();

    setColor(14);
    cout << "\nCurrent Favorite Food: " << fav_food << "\nCurrent Allergic Food: " << allergic_food << endl;
    setColor(7);

    cout << "Enter new favorite food: ";
    cin >> fav_food;
    cout << "Enter new allergic food: ";
    cin >> allergic_food;

    ofstream outFile(filename.c_str());
    outFile << name << " " << type << " " << fav_food << " " << allergic_food << " "
            << hunger << " " << energy << " " << happiness << " " << points << endl;
    outFile.close();

    setColor(10);
    cout << "Food preferences updated for user: " << username << endl;
    setColor(7);
}

void adminPanel() {
    if (!adminLogin()) {
        setColor(12);
        cout << "\nAccess Denied. Incorrect Password." << endl;
        setColor(7);
        return;
    }

    int option;
    string username;
    do {
        setColor(13);
        cout << "\n--- ADMIN PANEL ---" << endl;
        cout << "1. View Registered Users" << endl;
        cout << "2. Modify Food Preferences" << endl;
        cout << "3. Exit Admin Panel" << endl;
        setColor(7);
        cout << "Enter choice: ";
        cin >> option;

        switch (option) {
            case 1:
                viewRegisteredUsers();
                break;
            case 2:
                cout << "Enter username to modify: ";
                cin >> username;
                modifyFoodPreferences(username);
                break;
            case 3:
                cout << "Exiting admin panel...\n";
                
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (option != 3);
}


    
class Pet {
protected:
    string name, fav_food, allergic_food;
    int hunger, energy, happiness, points;

public:
    Pet(string n = "", string ff = "", string af = "")
        : name(n), fav_food(ff), allergic_food(af), hunger(100), energy(100), happiness(100), points(0) {}

    void show() {
        setColor(14);
        cout << "\nPet Name: " << name;
        cout << "\nFavorite Food: " << fav_food;
        cout << "\nAllergic To: " << allergic_food;
        cout << "\nHunger: " << hunger;
        cout << "\nEnergy: " << energy;
        cout << "\nHappiness: " << happiness;
        cout << "\nPoints: " << points << endl;
        setColor(7);
    }

    void play() {
        if (energy >= 10) {
            setColor(10);
            cout << "\nYou played with " << name << "! Happiness increased!";
            happiness += 10;
            energy -= 10;
            points += 5;
        } else {
            setColor(12);
            cout << "\n" << name << " is too tired to play.";
        }
        setColor(7);
    }

    void feed(string food) {
        if (food == allergic_food) {
            setColor(12);
            cout << "\nOh no! " << name << " is allergic to " << food << "! Lost happiness.";
            happiness -= 20;
            hunger -= 10;
            energy -= 10;
        } else if (food == fav_food) {
            setColor(10);
            cout << "\n" << name << " loves " << food << "! Gained energy and happiness.";
            happiness += 15;
            hunger -= 10;
            energy += 10;
            points += 10;
        } else {
            setColor(11);
            cout << "\n" << name << " ate " << food << ".";
            hunger -= 10;
            energy += 5;
        }
        setColor(7);
    }

    void interact(const string& weather) {
        setColor(11);
        cout << "\nHi, today the weather seems quite " << weather << ".\n";
        if (weather == "sunny") {
            cout << "How about a walk in the park? Your pet seems super energetic!\n";
            happiness += 20;
        } else if (weather == "rainy") {
            cout << "It's pouring outside! Your pet seems a bit lazy. Maybe some indoor games?\n";
            happiness -= 10;
        } else if (weather == "cold") {
            cout << "Brr... it's chilly! Your pet is curled up and sleepy.\n";
            happiness += 5;
        } else {
            cout << "The weather seems calm. Your pet is feeling neutral today.\n";
            happiness += 10;
        }
        cout << "Current Happiness Level: " << happiness << "\n";
        setColor(7);
    }

    int getPoints() { return points; }
    void setPoints(int p) { points = p; }
    virtual string getType() { return "Pet"; }

    virtual void saveProgress(ofstream &file) {
        file << name << " " << getType() << " " << fav_food << " " << allergic_food << " " << hunger << " " << energy << " " << happiness << " " << points << endl;
    }

    virtual ~Pet() {}
};

class Dog : public Pet {
public:
    Dog(string n = "", string ff= "Bone", string af="Chocolate") : Pet(n, ff, af) {}
    string getType() override { return "Dog"; }
};

class Cat : public Pet {
public:
    Cat(string n = "", string ff="Fish", string af="Milk") : Pet(n, ff, af) {}
    string getType() override { return "Cat"; }
};

class Rabbit : public Pet {
public:
    Rabbit(string n = "", string ff="Carrot", string af="Tomato") : Pet(n, ff, af) {}
    string getType() override { return "Rabbit"; }
};

class Dragon : public Pet {
public:
    Dragon(string n = "", string ff="Gems", string af="Silver") : Pet(n, ff,af) {}
    string getType() override { return "Dragon"; }
};

string getRandomWeather() {
    string weatherOptions[] = {"sunny", "rainy", "cloudy", "cold"};
    return weatherOptions[rand() % 4];
}

void saveExitTime() {
    ofstream timeFile("time.txt");
    time_t now = time(0);
    timeFile << now;
    timeFile.close();
}

time_t getLastExitTime() {
    ifstream timeFile("time.txt");
    time_t lastTime = 0;
    if (timeFile.is_open()) {
        timeFile >> lastTime;
        timeFile.close();
    }
    return lastTime;
}

bool isNewUser(string username) {
    string filename = username + "_savegame.txt";
    ifstream file(filename.c_str());
    return !file.good();
}

bool checkNeglect(string username) {
    if (isNewUser(username)) return false;

    time_t now = time(0);
    time_t lastTime = getLastExitTime();
    time_t t = 90;

    if (lastTime != 0 && difftime(now, lastTime) > t) {
        setColor(12);
        cout << "\nOH NO! You neglected your pet for too long, and it's gone... ??\n";
        setColor(7);
        return true;
    }
    return false;
}

void clearSaveData(string username) {
	string filename = username + "_savegame.txt";
    remove(filename.c_str());
    remove("time.txt");
}

bool recoveryQuiz(string username) {
    string answer, namae;
    string filename = username + "_savegame.txt";
    ifstream file(filename.c_str());
    if (file.is_open()) {
        file >> namae;
        file.close();
    }
    setColor(14);
    cout << "Quick! To bring your pet back, answer this question: \n";
    cout << "What was your pet's name? ";
    cin >> answer;
    setColor(7);
    return (answer == namae);
}

void saveGame(Pet &pet, string username) {
    string filename = username + "_savegame.txt";
    ofstream file(filename.c_str());
    if (file.is_open()) {
        pet.saveProgress(file);
        file.close();
    }
    saveExitTime();
}

Pet* loadGame(string username) {
    string name, fav_food, allergic_food, type;
    int hunger, energy, happiness, points;
    string filename = username + "_savegame.txt";
    ifstream file(filename.c_str());
    if (file.is_open()) {
        file >> name >> type >> fav_food >> allergic_food >> hunger >> energy >> happiness >> points;
        file.close();
        setColor(10);
        cout << "Game loaded successfully!" << endl;
        setColor(7);
        Pet* loadedPet = NULL;
        if (type == "Dog") loadedPet = new Dog(name,fav_food,allergic_food);
        else if (type == "Cat") loadedPet = new Cat(name,fav_food,allergic_food);
        else if (type == "Rabbit") loadedPet = new Rabbit(name,fav_food,allergic_food);
        else if (type == "Dragon") loadedPet = new Dragon(name,fav_food,allergic_food);
        if (loadedPet) loadedPet->setPoints(points);
        return loadedPet;
    }
    return NULL;
}

void viewExistingPlayers() {
    ifstream file("users.txt");
    string line;
    cout << "\n--- Registered Players ---\n";
    if (!file) {
        cout << "No registered users found.\n";
        return;
    }
    while (getline(file, line)) {
        cout << "- " << line << endl;
    }
    file.close();
}


int main() {
    string username;
    int choice;
      
       setColor(11);
        cout << "****************************************************************************************************\n";
        cout << "*                                                                                                  *\n";
        cout << "*                                    ** WELCOME TO FURGET ME NOT **                                *\n";
        cout << "*                                                                                                  *\n";
        cout << "****************************************************************************************************\n";
         
    while (true) {
    	setColor(14);
        cout << "1. Register\n2. Login\n3. AdminPanel\n4. Exit\n";
        setColor(7);
        cin >> choice;

        if (choice == 1) {
            registerUser();
            setColor(13);
            cout << "\n Quick Guide:\n";
            setColor(7);
            cout << "1. Register/Login to start.\n";
            cout << "2. Pick your pet and name it.\n";
            cout << "3. Check stats: favorite food, allergies, traits.\n";
            cout << "4. Play and feed your pet to earn points.\n";
            cout << "5. Avoid neglect,pets need love!\n";
            cout << "6. If it runs away, answer a memory question to get it back.\n";
            cout << "7. Hit 50 points to unlock the Dragon pet!\n";
            cout << "Login to begin your pet adventure!\n\n";
        } 
        else if (choice == 2) {
            if (!loginUser(username)) continue;

            // --- Begin Game Loop ---
            srand((unsigned int)time(0));
            string weather = getRandomWeather();

            string filename = username + "_savegame.txt";
            ifstream file(filename.c_str());
            bool hasSaveFile = file.good();
            file.close();

            Pet* myPet = NULL;

            if (hasSaveFile) {
                if (checkNeglect(username)) {
                    if (recoveryQuiz(username)) {
                        setColor(10);
                        cout << "\nYou saved your pet! They're back and happier than ever!\n";
                        myPet = loadGame(username);
                    } else {
                        setColor(12);
                        cout << "\nUnfortunately, your pet has found a new home...:(\n";
                        clearSaveData(username);
                        continue; // back to main menu
                    }
                } else {
                    setColor(8);
                    cout << "Do you want to:\n1. Load Previous Game\n2. Start a New Game\n";
                    setColor(7);
                    cin >> choice;
                    if (choice == 2) clearSaveData(username);
                    else myPet = loadGame(username);
                }
            } else {
                setColor(14);
                cout << "No saved game found.\n1. Start a new game\n2. Exit\n";
                setColor(7);
                cin >> choice;
                if (choice != 1) continue;
            }

            if (!myPet) {
                setColor(14);
                cout << "\nChoose a pet: 1. Dog  2. Cat  3. Rabbit\n";
                setColor(7);
                cin >> choice;
                setColor(14);
                cout << "Enter your pet's name: ";
                setColor(7);
                string name;
                cin >> name;
                switch (choice) {
                    case 1: myPet = new Dog(name); break;
                    case 2: myPet = new Cat(name); break;
                    case 3: myPet = new Rabbit(name); break;
                    default: cout << "\nInvalid choice."; continue;
                }
            }

            myPet->interact(weather);
            int action;
            do {
                setColor(14);
                cout << "\n\nChoose an action: 1. Show Stats  2. Play  3. Feed  4. Save 5. Logout\n";
                setColor(7);
                cin >> action;
                switch (action) {
                    case 1: myPet->show(); break;
                    case 2: myPet->play(); break;
                    case 3: {
                        string food;
                        setColor(14);
                        cout << "Enter food: ";
                        setColor(5);
                        cin >> food;
                        myPet->feed(food);
                        break;
                    }
                    case 4:
                        saveGame(*myPet, username);
                        cout << "----Progress Saved--------";
                        break;
                    case 5:
                        saveGame(*myPet, username);
                        cout << "----Logging out. See you soon!\n";
                        break;
                    default:
                        setColor(12);
                        cout << "\nInvalid action.";
                        setColor(7);
                }

                if (myPet->getPoints() >= 50 && myPet->getType() != "Dragon") {
                    setColor(9);
                    cout << "\nCongratulations! You've unlocked the Dragon!";
                    setColor(3);
                    cout << "\nEnter a new name for your Dragon: ";
                    setColor(7);
                    string dragonName;
                    cin >> dragonName;
                    delete myPet;
                    myPet = new Dragon(dragonName);
                }

            } while (action != 5);

            saveGame(*myPet, username);
            delete myPet;
            username = "";
            continue;  // After logout, go back to main menu
        } 
        else if (choice == 3) {
            adminPanel();
            continue;  // After admin panel, go back to main menu
        } 
        else {
            return 0; // Exit
        }
    }

    return 0;
}



