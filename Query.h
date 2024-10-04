#ifndef QUERY_H
#define QUERY_H

#include "AVLTree.h"
#include "document.h"
#include "Utility/Utility.h"  // Include the utility functions for manual string manipulation
#include <string>
#include <vector>
#include <set>

// Query class that handles search queries using the index built from AVL trees
class Query {
public:
    Query(); // Constructor

    // Function to handle user input, process the query, and return results
    std::vector<std::pair<std::string, int>> processQuery(const std::string& query,
                                                          AvlTree<std::string, std::map<std::string, int>>& wordIndex,
                                                          AvlTree<std::string, std::set<std::string>>& personIndex,
                                                          AvlTree<std::string, std::set<std::string>>& organizationIndex,
                                                          AvlTree<std::string, std::string>& stopWordTree);

private:
    // Helper functions to handle different parts of the query
    void processTerms(const std::string& term,
                      AvlTree<std::string, std::map<std::string, int>>& wordIndex,
                      std::map<std::string, int>& documentScores);

    void processPeople(const std::string& term,
                       AvlTree<std::string, std::set<std::string>>& personIndex,
                       std::map<std::string, int>& documentScores);

    void processOrganizations(const std::string& term,
                              AvlTree<std::string, std::set<std::string>>& organizationIndex,
                              std::map<std::string, int>& documentScores);

    void applyExclusions(const std::set<std::string>& excludedTerms,
                         std::map<std::string, int>& documentScores);

    // Method for ranking the results based on relevance
    std::vector<std::pair<std::string, int>> rankResults(std::map<std::string, int>& documentScores);
};

#endif // QUERY_H
