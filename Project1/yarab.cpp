// Include necessary libraries
#include <iostream> // For input/output
#include <fstream> // For file handling
#include <string> // For string handling
#include <functional> // For std::function
#include <algorithm> // For algorithmic operations
#include <memory> // For smart pointers
#include <utility> // For utility functions
#include <sstream> // String stream utilities

// Struct representing an entry in the hash table
struct HashTableEntry {
    std::string key; // The key of the entry
    std::string value; // The value associated with the key
    bool occupied = false; // Flag to indicate if the entry is in use
    bool deleted = false; // Flag to indicate if the entry has been logically deleted

    // Constructors
    HashTableEntry() : key(""), value(""), occupied(false), deleted(false) {}
    HashTableEntry(const std::string& key, const std::string& value) : key(key), value(value), occupied(true), deleted(false) {}
};

// The HashTable class manages a dynamic array of HashTableEntries
class HashTable {
private:
    HashTableEntry* table; // Dynamic array of hash table entries
    unsigned int size; // Number of elements currently in the table
    unsigned int capacity; // Current capacity of the table
    const unsigned int shift = 5; // Used for some internal calculations

    // Functions for internal mechanics like resizing and hashing
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
    // Constructors, destructor, and assignment operators
    HashTable(const HashTable& other) : size(other.size), capacity(other.capacity) {
        table = new HashTableEntry[capacity];
        for (unsigned int i = 0; i < capacity; ++i) {
            if (other.table[i].occupied) {
                table[i] = HashTableEntry(other.table[i].key, other.table[i].value);
                table[i].occupied = other.table[i].occupied;
            }
        }
    }
    HashTable& operator=(const HashTable& other) {
        if (this != &other) { 
            delete[] table;

            
            size = other.size;
            capacity = other.capacity;
            table = new HashTableEntry[capacity];
            for (unsigned int i = 0; i < capacity; ++i) {
                if (other.table[i].occupied) {
                    table[i] = HashTableEntry(other.table[i].key, other.table[i].value);
                    table[i].occupied = other.table[i].occupied;
                }
            }
        }
        return *this;
    }

    // Constructors, destructor, and assignment operators
    HashTable(unsigned int initCapacity = 16) : size(0), capacity(initCapacity) {
        table = new HashTableEntry[capacity];
    }

    // Public interface for interacting with the hash table
    void forEachEntry(std::function<void(const std::string&, const std::string&)> func, std::function<std::string(std::string, int, bool)> cipher) const {
        for (unsigned int i = 0; i < capacity; ++i) {
            if (table[i].occupied) {
                func(table[i].key, cipher(table[i].value, 5, false)); 
            }
        }
    }
    ~HashTable() {
        delete[] table;
    }
    // Public interface for interacting with the hash table
    void insert(const std::string& key, const std::string& value, bool updateIfExists = true) {
        if (size >= capacity * 0.75) {
            resizeAndRehash();
        }

        unsigned int index = hashFunction(key);
        unsigned int probe = 0;
        bool foundExistingKey = false;

     
        while (table[index].occupied && probe < capacity) {
            if (table[index].key == key) {
                foundExistingKey = true;
                break; 
            }
            probe++;
            index = (index + probe * probe) % capacity; 
        }

        
        if (foundExistingKey && !updateIfExists) {
            return;
        }

        
        if (!table[index].occupied) {
            size++; 
        }
        table[index] = HashTableEntry(key, value);
    }

    // Public interface for interacting with the hash table
    bool remove(const std::string& key) {
        unsigned int index = hashFunction(key);
        unsigned int probe = 0;

        while (table[index].occupied) {
            if (table[index].key == key) {
                table[index].occupied = false; 
                size--;
                return true; 
            }
            probe++;
            index = (index + probe * probe) % capacity; 

            
            if (probe >= capacity) {
                break;
            }
        }
        return false; 
    }

    
    std::string getValue(const std::string& key) {
        unsigned int index = hashFunction(key);
        unsigned int i = 0; 

        while (table[index].occupied && table[index].key != key) {
            i++;
            index = (index + i * i) % capacity; 

           
            if (i > capacity) {
                return ""; 
            }
        }

        if (table[index].occupied) {
            return table[index].value;
        }
        else {
            throw std::runtime_error("Key not found");
        }
    }

};

// Function to encode or decode a string using the Caesar cipher technique
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
// Functions for managing user credentials, including saving to file and checking
bool saveUserCredentials(const std::string& username, const std::string& password) {
    std::ofstream file(username + ".txt");
    if (file.is_open()) {
        file << username << std::endl;
        file << caesarCipher(password, 5) << std::endl; 
        file.close();
        return true;
        bool saveUserCredentials(const std::string& username, const std::string& password);{
            std::ofstream file(username + ".txt");
            if (!file) {
                std::cerr << "Error opening file for writing.\n";
                return false;
            }
            file << username << std::endl;
            file << caesarCipher(password, 5) << std::endl; 
            if (!file) {
                std::cerr << "Error writing to file.\n";
                return false;
            }
            file.close();
            return true;
        }
    }
    return false;
}
// Functions for managing user credentials, including saving to file and checking
bool checkUserCredentials(const std::string& username, const std::string& password) {
    std::ifstream file(username + ".txt");
    std::string storedUsername, storedPassword;

    if (file.is_open()) {
        getline(file, storedUsername);
        getline(file, storedPassword);
        file.close();

        
        return storedUsername == username && caesarCipher(storedPassword, 5, false) == password;
    }
    return false;
}
// Functions for managing user credentials, including saving to file and checking
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
// Functions for managing user credentials, including saving to file and checking
void logIn() {
    std::string username, password;
    std::cout << "Log in\nUsername: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (checkUserCredentials(username, password)) {
        std::cout << "Login successful!\n";
    }
    else {
        std::cout << "Invalid username or password.\n";
    }
}
// Functions to add a new platform password and retrieve an existing one
bool addPlatformPassword(const std::string& username, const std::string& platform, const std::string& password) {
    std::ofstream file(username + ".txt", std::ios::app); 
    if (file.is_open()) {
        
        file << caesarCipher(platform, 5) << std::endl;
        file << caesarCipher(password, 5) << std::endl;
        file.close();
        return true;
    }
    return false;
}
// Functions to add a new platform password and retrieve an existing one
bool getPasswordForPlatform(const std::string& username, const std::string& platform, std::string& passwordOut) {
    std::ifstream file(username + ".txt");
    std::string storedPlatform, storedPassword;

    if (file.is_open()) {
        while (getline(file, storedPlatform) && getline(file, storedPassword)) {
            // Decrypting for comparison
            storedPlatform = caesarCipher(storedPlatform, 5, false);
            if (storedPlatform == platform) {
                passwordOut = caesarCipher(storedPassword, 5, false); 
                file.close();
                return true;
            }
        }
        file.close();
    }
    return false;
}
// Global variables and functions for managing the current user session
HashTable* currentUserHashTable = nullptr; 
std::string currentUser; 
// Functions to add a new password to the current session and retrieve an existing one
void loadPasswordsFromFile(const std::string & username, HashTable & passwordStorage) {
        std::ifstream file(username + "_passwords.txt");
        if (!file.is_open()) {
            std::cerr << "Failed to open file for loading passwords.\n";
            return;
        }

        std::string platform, encryptedPassword;
        while (file >> platform >> encryptedPassword) {
            
            std::string decryptedPassword = caesarCipher(encryptedPassword, 5, false);
            passwordStorage.insert(platform, decryptedPassword);
        }

        file.close();
    }


// Global variables and functions for managing the current user session
    void loginUserSession(const std::string & username) {
        currentUser = username;
        if (currentUserHashTable != nullptr) {
            delete currentUserHashTable; 
        }
        currentUserHashTable = new HashTable(16); 
        loadPasswordsFromFile(currentUser, *currentUserHashTable);
    }

    
    // Global variables and functions for managing the current user session
void logoutUserSession() {
    if (currentUserHashTable != nullptr) {
        delete currentUserHashTable; 
        currentUserHashTable = nullptr; 
    }
    currentUser.clear();
}

// Functions to add a new password to the current session and retrieve an existing one
void addPassword(const std::string& platform, const std::string& password) {
    if (currentUserHashTable == nullptr) {
        std::cerr << "Error: No user is currently logged in." << std::endl;
        return; 
    }
    if (platform.empty() || password.empty()) {
        std::cerr << "Platform and password cannot be empty.\n";
        return;
    }
    std::string encryptedPassword = caesarCipher(password, 5, true); 
    currentUserHashTable->insert(platform, encryptedPassword);
};

// Functions to add a new password to the current session and retrieve an existing one
void savePasswordsToFile(const std::string& username, HashTable& passwordStorage) {
    std::ofstream file(username + "_passwords.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving passwords.\n";
        return;
    }

    passwordStorage.forEachEntry([&file](const std::string& key, const std::string& value) {
        file << key << " " << value << "\n";
        }, caesarCipher);

    file.close();
}



// The entry point of the program, presenting a menu for the password manager
int main() {
    HashTable passwordStorage(16);
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

        switch (choice) {
        case '1': {
            signUp();
            break;
        }
        case '2': {
            logIn();
            
            break;
        }
        case '3': {
            
            std::string platform, password;
            std::cout << "Enter platform name: ";
            std::cin >> platform;
            std::cout << "Enter password: ";
            std::cin >> password;
            
            passwordStorage.insert(platform, password); 
            break;
        }
        case '4': {
            
            std::string platform;
            std::cout << "Enter platform name to retrieve your password: ";
            std::cin >> platform;
            std::string password = passwordStorage.getValue(platform);
            
            if (!password.empty()) {
                std::cout << "Your password for " << platform << " is: " << password << std::endl;
            }
            else {
                std::cout << "No password found for " << platform << std::endl;
            }
            break;
        }
        case '5': {
            running = false;
            break;
        }
        default: {
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
        }
    }

    return 0;
}
