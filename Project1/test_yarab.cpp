#include <cassert>
#include "yarab.cpp" // Include your main application file

int test_main() {
    // Test the HashTable constructor
    {
        HashTable ht;
        assert(ht.getSize() == 0); // The initial size should be 0
    }

    // Test the insert function
    {
        HashTable ht;
        ht.insert("key1", "value1");
        assert(ht.getSize() == 1); // The size should be 1 after one insertion
        assert(ht.getValue("key1") == "value1"); // The value for "key1" should be "value1"
    }

    // Test the remove function
    {
        HashTable ht;
        ht.insert("key1", "value1");
        ht.remove("key1");
        assert(ht.getSize() == 0); // The size should be 0 after removing the only element
        try {
            ht.getValue("key1");
            assert(false); // This should not be reached, as an exception should be thrown
        }
        catch (const std::runtime_error&) {
            // Expected exception
        }
    }

    // Test the getValue function
    {
        HashTable ht;
        ht.insert("key1", "value1");
        assert(ht.getValue("key1") == "value1"); // The value for "key1" should be "value1"
        try {
            ht.getValue("key2");
            assert(false); // This should not be reached, as an exception should be thrown
        }
        catch (const std::runtime_error&) {
            // Expected exception
        }
    }

    // Test the copy constructor
    {
        HashTable ht1;
        ht1.insert("key1", "value1");
        HashTable ht2(ht1); // Create a copy of ht1
        assert(ht2.getSize() == ht1.getSize()); // The size of ht2 should be the same as the size of ht1
        assert(ht2.getValue("key1") == ht1.getValue("key1")); // The value for "key1" in ht2 should be the same as in ht1
    }

    // Test the assignment operator
    {
        HashTable ht1;
        ht1.insert("key1", "value1");
        HashTable ht2;
        ht2 = ht1; // Assign ht1 to ht2
        assert(ht2.getSize() == ht1.getSize()); // The size of ht2 should be the same as the size of ht1
        assert(ht2.getValue("key1") == ht1.getValue("key1")); // The value for "key1" in ht2 should be the same as in ht1
    }

    return 0;
}
