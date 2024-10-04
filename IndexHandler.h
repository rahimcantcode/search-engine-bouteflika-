#ifndef INDEX_HANDLER_H
#define INDEX_HANDLER_H

#include "AVLTree.h"
#include "document.h"
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <iostream>

class IndexHandler {
public:
    IndexHandler();

    // Methods to manage indexing addPersonToIndex 
    void addWordToIndex(const std::string& term, const document& doc);
    void addPersonToIndex(const std::string& term, const document& doc);
    void addOrgToIndex(const std::string& term, const document& doc);

    void removeFromIndex(const std::string& term, const document& doc);
    std::vector<pair<document, int>> queryIndex(const std::string& term) const; //! check this for queries 

    // Process all files within a given directory and index their contents
    void processAllFiles(const string &directoryPath);
    //!call this funciton instead of deserializeIndex in UI.cpp

    // Serialization and Deserialization for persistence
    void serializeIndex();
    void deserializeIndex();

    size_t getNumberOfIndexedTerms() const;  // Add this line

    int getTotalDocumentCount() const {
            return totalDocumentsProcessed;
        };

private:
    // Parse a JSON file and index its content into various AVL trees
    void parseJsonDocument(const string &filePath);

    // Tokenizes the text into individual words
    std::vector<string> splitIntoWords(const string &text) const;

    // Loads stop words into an AVL tree from a given source
    void loadExclusionWords();
    
    // Helper methods for serialization
    static void serializeTree(std::ofstream& out, const AvlTree<std::string, std::map<document, int>>& tree);
    static void deserializeTree(std::ifstream& in, AvlTree<std::string, std::map<document, int>>& tree);


    //! instead of a vector of strings, you should use a set of strings
    AvlTree<std::string, std::map<document, int>> mainIndex;
    AvlTree<std::string, std::map<document, int>> personIndex;
    AvlTree<std::string, std::map<document, int>> organizationIndex;
    AvlTree<string, string> stopWordTree;
    int totalDocumentsProcessed = 0;  // Counter for the number of documents processed


};

#endif // INDEX_HANDLER_H
