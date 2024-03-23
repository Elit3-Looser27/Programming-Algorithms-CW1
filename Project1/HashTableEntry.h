#include <iostream>
#include <fstream>
#include <string>

struct HashTableEntry {
    std::string key;
    std::string value;
    bool occupied = false;

    HashTableEntry() : key(""), value(""), occupied(false) {}
    HashTableEntry(const std::string& key, const std::string& value) : key(key), value(value), occupied(true) {}
};

class HashTable {
private:
    HashTableEntry* table;
    unsigned int size;
    unsigned int capacity;
    const unsigned int shift = 5; // Shift by 5 for cyclic shifting

    void resizeAndRehash() {
        HashTableEntry* oldTable = table;
        unsigned int oldCapacity = capacity;
        capacity *= 2;
        table = new HashTableEntry[capacity];
        size = 0;

        for (unsigned int i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].occupied) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        delete[] oldTable;
    }

    unsigned int hashFunction(const std::string& key) const {
        unsigned int hashValue = 0;
        for (char c : key) {
            hashValue = (hashValue * 131 + c) % capacity;
        }
        return hashValue;
    }

public:
    HashTable(unsigned int initCapacity = 16) : size(0), capacity(initCapacity) {
        table = new HashTableEntry[capacity];
    }

    ~HashTable() {
        delete[] table;
        table = nullptr;
    }

    void insert(const std::string& key, const std::string& value) {
        if (size >= capacity * 0.75) {
            resizeAndRehash();
        }

        unsigned int index = hashFunction(key);
        while (table[index].occupied && table[index].key != key) {
            index = (index + shift) % capacity; // Cyclic shifting
        }

        if (!table[index].occupied) {
            size++;
        }
        table[index] = HashTableEntry(key, value);
    }

    std::string getValue(const std::string& key) {
        unsigned int index = hashFunction(key);
        while (table[index].occupied && table[index].key != key) {
            index = (index + shift) % capacity; // Cyclic shifting
        }

        if (table[index].occupied) {
            return table[index].value;
        } else {
            return ""; // Key not found
        }
    }
};

std::string caesarCipher(std::string text, int shift, bool encrypt = true) {
    const int alphabetSize = 26;
    const char firstLowercase = 'a';
    const char firstUppercase = 'A';
    int direction = encrypt ? 1 : -1;

    for (char& c : text) {
        if (isalpha(c)) {
            char firstLetter = islower(c) ? firstLowercase : firstUppercase;
            c = firstLetter + (c - firstLetter + direction * shift + alphabetSize) % alphabetSize;
        }
    }

    return text;
}

bool saveUserCredentials(const std::string& username, const std::string& password) {
    std::ofstream file(username + ".txt");
    if (file.is_open()) {
        file << username << std::endl;
        file << caesarCipher(password, 5) << std::endl; // Encrypting password
        file.close();
        return true;
    }
    return false;
}

bool checkUserCredentials(const std::string& username, const std::string& password) {
    std::ifstream file(username + ".txt");
    std::string storedUsername, storedPassword;

    if (file.is_open()) {
        getline(file, storedUsername);
        getline(file, storedPassword);
        file.close();

        // Decrypt stored password and compare
        return storedUsername == username && caesarCipher(storedPassword, 5, false) == password;
    }
    return false;
}

void signUp() {
    std::string username, password;
    std::cout << "Sign up\nUsername: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (saveUserCredentials(username, password)) {
        std::cout << "User registered successfully!\n";
    }
    else {
        std::cout << "Error registering user.\n";
    }
}

void logIn() {
    std::string username, password;
    std::cout << "Log in\nUsername: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (checkUserCredentials(username, password)) {
        std::cout << "Login successful!\n";
        loginUserSession(username); // Initialize the user session
    }
    else {
        std::cout << "Invalid username or password.\n";
    }
}


bool addPlatformPassword(const std::string& username, const std::string& platform, const std::string& password) {
    std::ofstream file(username + ".txt", std::ios::app); // Open in append mode
    if (file.is_open()) {
        // Encrypting both platform and password
        file << caesarCipher(platform, 5) << std::endl;
        file << caesarCipher(password, 5) << std::endl;
        file.close();
        return true;
    }
    return false;
}

bool getPasswordForPlatform(const std::string& username, const std::string& platform, std::string& passwordOut) {
    std::ifstream file(username + ".txt");
    std::string storedPlatform, storedPassword;

    if (file.is_open()) {
        while (getline(file, storedPlatform) && getline(file, storedPassword)) {
            // Decrypting for comparison
            storedPlatform = caesarCipher(storedPlatform, 5, false);
            if (storedPlatform == platform) {
                passwordOut = caesarCipher(storedPassword, 5, false); // Decrypting password
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}

HashTable* currentUserHashTable = nullptr; // Pointer to the current user's hash table
std::string currentUser; // Tracks the current logged-in user

void loginUserSession(const std::string& username) {
    currentUser = username;
    // For demonstration, we're directly allocating a new HashTable for each login
    // In a complete implementation, you'd check if one already exists and load it
    if (currentUserHashTable != nullptr) {
        delete currentUserHashTable; // Clean up the previous session
    }
    currentUserHashTable = new HashTable(16); // Initialize a new hash table for the session
    // Load or initialize the user's hash table here
    // Load the user's passwords
    loadPasswordsFromFile(currentUser, *currentUserHashTable);
}

void logoutUserSession() {
    if (!currentUser.empty() && currentUserHashTable != nullptr) {
        // Save the current user's passwords before logging out
        savePasswordsToFile(currentUser, *currentUserHashTable);

        delete currentUserHashTable; // Clean up
        currentUserHashTable = nullptr;
    }
    currentUser.clear();
}


void addPassword(const std::string& platform, const std::string& password) {
    std::string encryptedPassword = caesarCipher(password, 5, true); // Encrypt
    currentUserHashTable->insert(platform, encryptedPassword);
}

std::string getPassword(const std::string& platform) {
    if (currentUserHashTable == nullptr) {
        std::cerr << "Error: No user is currently logged in." << std::endl;
        return ""; // Return empty string if no user is logged in
    }
    std::string encryptedPassword = currentUserHashTable->getValue(platform);
    return encryptedPassword.empty() ? "" : caesarCipher(encryptedPassword, 5, false); // Decrypt
}


void savePasswordsToFile(const std::string& username, HashTable& passwordStorage) {
    std::ofstream file(username + "_passwords.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving passwords.\n";
        return;
    }

    for (unsigned int i = 0; i < passwordStorage.capacity; ++i) {
        if (passwordStorage.table[i].occupied) {
            // Assuming you have a method to encrypt the password here
            std::string encryptedPassword = caesarCipher(passwordStorage.table[i].value, 5);
            file << passwordStorage.table[i].key << " " << encryptedPassword << "\n";
        }
    }

    file.close();
}

void loadPasswordsFromFile(const std::string& username, HashTable& passwordStorage) {
    std::ifstream file(username + "_passwords.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading passwords.\n";
        return;
    }

    std::string platform, encryptedPassword;
    while (file >> platform >> encryptedPassword) {
        // Assuming you have a method to decrypt the password here
        std::string decryptedPassword = caesarCipher(encryptedPassword, 5, false);
        passwordStorage.insert(platform, decryptedPassword);
    }

    file.close();
}

int main() {
    HashTable passwordStorage(16); // Initialize the hash table for password storage
    char choice;
    bool running = true;

    while (running) {
        std::cout << "Password Manager\n";
        std::cout << "1. Sign Up\n";
        std::cout << "2. Log In\n";
        std::cout << "3. Add a Password\n";
        std::cout << "4. Retrieve a Password\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice: ";

        std::cin >> choice;

        // Clear the input buffer to handle extra input or incorrect types
        std::cin.clear(); // Clear any error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the input

        switch (choice) {
        case '1':
            signUp();
            break;
        case '2':
            logIn();
            break;
        case '3': { // Option for adding a new password
            if (currentUser.empty() || currentUserHashTable == nullptr) {
                std::cerr << "Error: No user is currently logged in.\n";
            }
            else {
                std::string platform, password;
                std::cout << "Enter platform name: ";
                std::cin >> platform;
                std::cout << "Enter password: ";
                std::cin >> password;

                addPassword(platform, password); // Adds the password to currentUserHashTable

                // Save the updated hash table to file
                savePasswordsToFile(currentUser, *currentUserHashTable);

                std::cout << "Password added successfully for " << platform << ".\n";
            }
            break;
        }

        case '4':
            if (currentUser.empty() || currentUserHashTable == nullptr) {
                std::cerr << "Error: No user is currently logged in.\n";
            }
            else {
                std::string platform;
                std::cout << "Enter platform name to retrieve your password: ";
                std::cin >> platform;
                std::string password = getPassword(platform); // Uses currentUserHashTable
                if (!password.empty()) {
                    std::cout << "Your password for " << platform << " is: " << password << std::endl;
                }
                else {
                    std::cout << "No password found for " << platform << std::endl;
                }
            }
            break;
        case '5':
            logoutUserSession();
            running = false;
            break;

        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}
