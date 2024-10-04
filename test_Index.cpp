// #define CATCH_CONFIG_MAIN  // This tells CATCH to provide a main() - only do this in one cpp file
// #include <sstream>
// #include <filesystem>

// #include "DocumentParser.h"
// #include "IndexHandler.h"
// #include "DocumentParser.h"

// #include "catch2/catch.hpp"
// #include "rapidjson/document.h"
// #include "rapidjson/istreamwrapper.h"


// TEST_CASE("IndexHandler - Basic functionality", "[indexhandler]") {
//     IndexHandler indexHandler;

//     SECTION("Adding and querying documents") {
//         indexHandler.addWordToIndex("apple", "doc1");
//         indexHandler.addWordToIndex("banana", "doc2");
//         indexHandler.addWordToIndex("apple", "doc3");

//         std::vector<std::string> expectedApple = {"doc1", "doc3"};
//         std::vector<std::string> expectedBanana = {"doc2"};

//         REQUIRE(indexHandler.queryIndex("apple") == expectedApple);
//         REQUIRE(indexHandler.queryIndex("banana") == expectedBanana);
//     }

//     SECTION("Removing documents") {
//         indexHandler.addWordToIndex("apple", "doc1");
//         indexHandler.addWordToIndex("banana", "doc2");
//         indexHandler.addWordToIndex("apple", "doc3");

//         indexHandler.removeFromIndex("apple", "doc1");

//         std::vector<std::string> expectedApple = {"doc3"};
//         std::vector<std::string> expectedBanana = {"doc2"};

//         REQUIRE(indexHandler.queryIndex("apple") == expectedApple);
//         REQUIRE(indexHandler.queryIndex("banana") == expectedBanana);
//     }

//     SECTION("Querying non-existent term") {
//         REQUIRE(indexHandler.queryIndex("nonexistent").empty());
//     }
// }

// TEST_CASE("IndexHandler - Serialization and Deserialization", "[indexhandler]") {
//     IndexHandler indexHandler;

//     indexHandler.addWordToIndex("apple", "doc1");
//     indexHandler.addWordToIndex("banana", "doc2");
//     indexHandler.addWordToIndex("apple", "doc3");

//     std::string filePath = "sample_data/coll_1/news_0064567.json";

//     SECTION("Serialization") {
//         REQUIRE_NOTHROW(indexHandler.serializeIndex(filePath));
//     }

//     SECTION("Deserialization") {
//         indexHandler.serializeIndex(filePath);

//         IndexHandler deserializedHandler;
//         REQUIRE_NOTHROW(deserializedHandler.deserializeIndex(filePath));

//         std::vector<std::string> expectedApple = {"doc1", "doc3"};
//         std::vector<std::string> expectedBanana = {"doc2"};

//         REQUIRE(deserializedHandler.queryIndex("apple") == expectedApple);
//         REQUIRE(deserializedHandler.queryIndex("banana") == expectedBanana);
//     }

//     // Clean up the test file
//     std::remove(filePath.c_str());
// }

// TEST_CASE("IndexHandler - Querying multiple terms", "[indexhandler]") {
//     IndexHandler indexHandler;

//     indexHandler.addWordToIndex("apple", "doc1");
//     indexHandler.addWordToIndex("banana", "doc1");
//     indexHandler.addWordToIndex("apple", "doc2");
//     indexHandler.addWordToIndex("orange", "doc3");

//     SECTION("Querying multiple terms") {
//         std::vector<std::string> expectedApple = {"doc1", "doc2"};
//         std::vector<std::string> expectedBanana = {"doc1"};
//         std::vector<std::string> expectedOrange = {"doc3"};

//         REQUIRE(indexHandler.queryIndex("apple") == expectedApple);
//         REQUIRE(indexHandler.queryIndex("banana") == expectedBanana);
//         REQUIRE(indexHandler.queryIndex("orange") == expectedOrange);
//     }
// }

// TEST_CASE("IndexHandler - Updating an existing document", "[indexhandler]") {
//     IndexHandler indexHandler;

//     indexHandler.addWordToIndex("apple", "doc1");
//     indexHandler.addWordToIndex("banana", "doc1");

//     SECTION("Updating an existing document") {
//         indexHandler.addWordToIndex("apple", "doc1");
//         indexHandler.addWordToIndex("orange", "doc1");

//         std::vector<std::string> expectedApple = {"doc1"};
//         std::vector<std::string> expectedBanana = {"doc1"};
//         std::vector<std::string> expectedOrange = {"doc1"};

//         REQUIRE(indexHandler.queryIndex("apple") == expectedApple);
//         REQUIRE(indexHandler.queryIndex("banana") == expectedBanana);
//         REQUIRE(indexHandler.queryIndex("orange") == expectedOrange);
//     }
// }

// TEST_CASE("Document Parsing and Indexing from JSON", "[DocumentParser]") {
//     DocumentParser parser;
//     AvlTree<string, map<string, int>> indexWordTree;
//     AvlTree<string, set<string>> indexPeopleTree;
//     AvlTree<string, set<string>> indexOrgsTree;
//     AvlTree<string, string> indexStopWordTree;
//     AvlTree<string, document> documentIndex;

//     // This should be the path to your JSON file, adjust as necessary
//     string filePath = "sample_data\\coll_1\\news_0064567.json";


//     parser.parseJsonDocument(filePath, indexWordTree, indexPeopleTree, indexOrgsTree, indexStopWordTree, documentIndex);

//     SECTION("Parsing and Indexing JSON Document") {
//         parser.parseJsonDocument(filePath, indexWordTree, indexPeopleTree, indexOrgsTree, indexStopWordTree, documentIndex);

//         REQUIRE(documentIndex.size() == 1); // Validate that exactly one document is indexed
//         auto& doc = documentIndex.findMin(); // Assuming findMin() retrieves the document based on a relevant sort order, typically by UUID or insertion order

//         // Check that the document title matches the actual title from the JSON
//         REQUIRE(doc.docTitle == "German firms doing business in UK gloomy about Brexit - survey");
//         // Confirm that a specific phrase exists within the document's content
//         REQUIRE(doc.docContent.find("German businesses") != string::npos);
//     }
// }

// TEST_CASE("IndexHandler - Data Integrity Testing", "[indexhandler]") {
//     IndexHandler indexHandler;

//     indexHandler.addWordToIndex("fruit", "doc1");
//     indexHandler.addWordToIndex("fruit", "doc2");
//     indexHandler.removeFromIndex("fruit", "doc1");

//     SECTION("Check Data Consistency After Modifications") {
//         std::vector<std::string> expected = {"doc2"};
//         REQUIRE(indexHandler.queryIndex("fruit") == expected);  // Ensure data consistency after removal
//     }

//     SECTION("Check Empty Query") {
//         REQUIRE(indexHandler.queryIndex("vegetable").empty());  // Ensure no data returns for non-existent term
//     }
// }


// TEST_CASE("Document Parsing and Indexing from Actual JSON File", "[DocumentParser]") {
//     DocumentParser parser;
//     AvlTree<string, map<string, int>> indexWordTree;
//     AvlTree<string, set<string>> indexPeopleTree;
//     AvlTree<string, set<string>> indexOrgsTree;
//     AvlTree<string, string> indexStopWordTree;
//     AvlTree<string, document> documentIndex;

//     // Path adjusted as per your setup
//     string filePath = "sample_data\\coll_1\\news_0064567.json";

//     parser.parseJsonDocument(filePath, indexWordTree, indexPeopleTree, indexOrgsTree, indexStopWordTree, documentIndex);

//     SECTION("Verify that the document is indexed correctly") {
//         REQUIRE(documentIndex.size() == 1);  // Ensure that one document is indexed
//         auto& doc = documentIndex.findMin();  // Retrieve the document assuming the document ID is indexed

//         // Correctly access the title and author using the actual member names from the document class
//         REQUIRE(doc.docTitle == "German firms doing business in UK gloomy about Brexit - survey");
//         REQUIRE(doc.docAuthor == "Reuters Editorial");  // Correct member name for author
//     }
// }

// TEST_CASE("IndexHandler - Serialization and Deserialization with Real Data", "[indexhandler]") {
//     IndexHandler indexHandler;
//     DocumentParser parser;
//     AvlTree<string, map<string, int>> indexWordTree;
//     AvlTree<string, set<string>> indexPeopleTree;
//     AvlTree<string, set<string>> indexOrgsTree;
//     AvlTree<string, string> indexStopWordTree;
//     AvlTree<string, document> documentIndex;

//     string filePath = "sample_data\\coll_1\\news_0064567.json";
//     parser.parseJsonDocument(filePath, indexWordTree, indexPeopleTree, indexOrgsTree, indexStopWordTree, documentIndex);

//     // Serialize the state after parsing
//     std::string serializePath = "index_snapshot.json";
//     indexHandler.serializeIndex(serializePath);

//     SECTION("Deserialization") {
//         IndexHandler deserializedHandler;
//         REQUIRE_NOTHROW(deserializedHandler.deserializeIndex(serializePath));

//         // Assuming you have a method to get the number of terms or documents indexed
//         REQUIRE(deserializedHandler.getNumberOfIndexedTerms() == indexHandler.getNumberOfIndexedTerms());
//     }

//     // Clean up the test file
//     std::remove(serializePath.c_str());
// }
