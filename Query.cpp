#include "Query.h"
#include "Utility/Utility.h"  // Include the utility functions for manual string manipulation

using namespace Utility;

// Constructor definition
Query::Query() {
    // Constructor initialization if necessary
}

/// Main function to handle user input, process the query, and return results
std::vector<std::pair<std::string, int>> Query::processQuery(const std::string& query,
                                                             AvlTree<std::string, std::map<std::string, int>>& wordIndex,
                                                             AvlTree<std::string, std::set<std::string>>& personIndex,
                                                             AvlTree<std::string, std::set<std::string>>& organizationIndex,
                                                             AvlTree<std::string, std::string>& stopWordTree) {
    std::map<std::string, int> documentScores;
    std::set<std::string> excludedTerms;
    std::vector<std::string> terms = manualSplit(query, ' ');

    for (size_t i = 0; i < terms.size(); ++i) {
        std::string processedTerm = manualToLower(terms[i]);
        if (manualStartsWith(processedTerm, "org:")) {
            std::string fullOrgName = processedTerm.substr(4);
            i++;
            // Accumulate the full organization name if it contains spaces
            while (i < terms.size() && !manualStartsWith(terms[i], "org:") && !manualStartsWith(terms[i], "person:") && terms[i][0] != '-') {
                fullOrgName += " " + terms[i];
                i++;
            }
            i--; // Adjust because for loop will increment
            processOrganizations(fullOrgName, organizationIndex, documentScores);
        } else if (manualStartsWith(processedTerm, "person:")) {
            std::string fullPersonName = processedTerm.substr(7);
            i++;
            // Accumulate the full person name if it contains spaces
            while (i < terms.size() && !manualStartsWith(terms[i], "org:") && !manualStartsWith(terms[i], "person:") && terms[i][0] != '-') {
                fullPersonName += " " + terms[i];
                i++;
            }
            i--; // Adjust because for loop will increment
            processPeople(fullPersonName, personIndex, documentScores);
        } else if (!processedTerm.empty() && processedTerm[0] == '-') {
            excludedTerms.insert(manualStripPrefix(processedTerm, "-"));
        } else {
            processTerms(processedTerm, wordIndex, documentScores);
        }
    }

    applyExclusions(excludedTerms, documentScores);
    return rankResults(documentScores);
}



// Function to process generic terms from the query and update document scores
void Query::processTerms(const std::string& term,
                         AvlTree<std::string, std::map<std::string, int>>& wordIndex,
                         std::map<std::string, int>& documentScores) {
    // Check if the term is in the word index
    if (wordIndex.contains(term)) {
        const auto& docsMap = wordIndex.at(term); // Retrieve documents associated with the term
        std::cout << "Term found, documents count: " << docsMap.size() << std::endl;
        for (const auto& docPair : docsMap) {
            std::cout << "Doc: " << docPair.first << ", Score: " << docPair.second << std::endl;
            documentScores[docPair.first] += docPair.second; // Update document scores by adding the term frequency
        }
    } else {
        std::cout << "Term not found: " << term << std::endl;
    }
}



// Function to handle processing of people-related terms
void Query::processPeople(const std::string& term,
                          AvlTree<std::string, std::set<std::string>>& personIndex,
                          std::map<std::string, int>& documentScores) {
    if (personIndex.contains(term)) {
        const std::set<std::string>& docsSet = personIndex.at(term); // Retrieve documents associated with the person
        for (const std::string& docID : docsSet) {
            documentScores[docID] += 1; // Increment score for each document; adjust logic as needed for scoring
        }
    }
}


// Function to handle processing of organization-related terms
void Query::processOrganizations(const std::string& term,
                                 AvlTree<std::string, std::set<std::string>>& organizationIndex,
                                 std::map<std::string, int>& documentScores) {
    if (organizationIndex.contains(term)) {
        const std::set<std::string>& docsSet = organizationIndex.at(term); // Retrieve documents associated with the organization
        for (const std::string& docID : docsSet) {
            documentScores[docID] += 1; // Increment score for each document; this could be adjusted based on specific logic
        }
    }
}


// Function to apply exclusions to filter out unwanted terms
void Query::applyExclusions(const std::set<std::string>& excludedTerms,
                            std::map<std::string, int>& documentScores) {
    // Iterate through each term that needs to be excluded
    for (const std::string& term : excludedTerms) {
        // Check if the term is actually in the document scores map
        if (documentScores.find(term) != documentScores.end()) {
            documentScores.erase(term);  // Completely removes the score for this document
        }
    }
}


// Function to rank the results by their scores
std::vector<std::pair<std::string, int>> Query::rankResults(std::map<std::string, int>& documentScores) {
    // Convert map to a vector of pairs for sorting
    std::vector<std::pair<std::string, int>> sortedResults(documentScores.begin(), documentScores.end());

    // Sort the vector by the scores in descending order
    std::sort(sortedResults.begin(), sortedResults.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second;
              });

    return sortedResults;
}

