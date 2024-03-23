#include "gtest/gtest.h"
#include "yarab.cpp" // Include your main application file

// Test the HashTable constructor
TEST(HashTableTest, Constructor) {
    HashTable ht;
    EXPECT_EQ(ht.getSize(), 0); // The initial size should be 0
}

// Test the insert function
TEST(HashTableTest, Insert) {
    HashTable ht;
    ht.insert("key1", "value1");
    EXPECT_EQ(ht.getSize(), 1); // The size should be 1 after one insertion
    EXPECT_EQ(ht.getValue("key1"), "value1"); // The value for "key1" should be "value1"
}

// Test the remove function
TEST(HashTableTest, Remove) {
    HashTable ht;
    ht.insert("key1", "value1");
    ht.remove("key1");
    EXPECT_EQ(ht.getSize(), 0); // The size should be 0 after removing the only element
    EXPECT_THROW(ht.getValue("key1"), std::runtime_error); // An exception should be thrown when trying to get the value for "key1"
}

// Test the getValue function
TEST(HashTableTest, GetValue) {
    HashTable ht;
    ht.insert("key1", "value1");
    EXPECT_EQ(ht.getValue("key1"), "value1"); // The value for "key1" should be "value1"
    EXPECT_THROW(ht.getValue("key2"), std::runtime_error); // An exception should be thrown when trying to get the value for a non-existent key
}

// Test the copy constructor
TEST(HashTableTest, CopyConstructor) {
    HashTable ht1;
    ht1.insert("key1", "value1");
    HashTable ht2(ht1); // Create a copy of ht1
    EXPECT_EQ(ht2.getSize(), ht1.getSize()); // The size of ht2 should be the same as the size of ht1
    EXPECT_EQ(ht2.getValue("key1"), ht1.getValue("key1")); // The value for "key1" in ht2 should be the same as in ht1
}

// Test the assignment operator
TEST(HashTableTest, AssignmentOperator) {
    HashTable ht1;
    ht1.insert("key1", "value1");
    HashTable ht2;
    ht2 = ht1; // Assign ht1 to ht2
    EXPECT_EQ(ht2.getSize(), ht1.getSize()); // The size of ht2 should be the same as the size of ht1
    EXPECT_EQ(ht2.getValue("key1"), ht1.getValue("key1")); // The value for "key1" in ht2 should be the same as in ht1
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
