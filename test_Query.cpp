#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"
#include "Query.h"
#include "AVLTree.h"

// Assuming the use of a simplified data structure for documents and setting up some global AVL trees
AvlTree<std::string, std::map<std::string, int>> wordIndex;
AvlTree<std::string, std::set<std::string>> personIndex;
AvlTree<std::string, std::set<std::string>> organizationIndex;
AvlTree<std::string, std::string> stopWordTree;

// Helper function to setup indices (might need more complexity based on actual data handling)
void setupIndices() {
    wordIndex.insert("brexit", {{"e17252151a2d36344b7104d14805414a55042abe", 5}});
    personIndex.insert("eric schweitzer", {"e17252151a2d36344b7104d14805414a55042abe"});
    organizationIndex.insert("reuters", {"e17252151a2d36344b7104d14805414a55042abe"});
    stopWordTree.insert("the", "stopword");
}

TEST_CASE("Query Processing", "[Query]") {
    setupIndices();  // Prepare the indices with some sample data
    Query queryProcessor;

    SECTION("Single Term Query") {
        std::vector<std::pair<std::string, int>> results = queryProcessor.processQuery("brexit", wordIndex, personIndex, organizationIndex, stopWordTree);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].first == "e17252151a2d36344b7104d14805414a55042abe");
        REQUIRE(results[0].second == 5);
    }

    SECTION("Person Query") {
        std::vector<std::pair<std::string, int>> results = queryProcessor.processQuery("person:eric schweitzer", wordIndex, personIndex, organizationIndex, stopWordTree);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].first == "e17252151a2d36344b7104d14805414a55042abe");
        REQUIRE(results[0].second == 1);  // Assuming score increment for person match
    }

    SECTION("Organization Query") {
        std::vector<std::pair<std::string, int>> results = queryProcessor.processQuery("org:reuters", wordIndex, personIndex, organizationIndex, stopWordTree);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].first == "e17252151a2d36344b7104d14805414a55042abe");
        REQUIRE(results[0].second == 1);  // Assuming score increment for organization match
    }

    SECTION("Exclusion Query") {
        std::vector<std::pair<std::string, int>> results = queryProcessor.processQuery("-the", wordIndex, personIndex, organizationIndex, stopWordTree);
        REQUIRE(results.empty());  // Assuming exclusion removes all matches
    }
}

