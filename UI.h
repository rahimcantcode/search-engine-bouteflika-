#ifndef UI_H
#define UI_H

#include "IndexHandler.h"
#include <string>
#include <vector>

class UI {
private:
    IndexHandler& indexHandler;  // Reference to IndexHandler to manage indexing operations

public:
    // Constructor that accepts a reference to an IndexHandler
    UI(IndexHandler& handler);

    // Display the main menu and handle user interactions
    void mainMenu();

    // Load index from a file
    void loadIndexFromPersist(); //! this function need to call processAllFiles() from documentParser class instea 

    void performSearch(string query);  

    //read the index
    void readIndexFromArticleFiles(const std::string& filePath);  // Add this line

    // Display search results
    void displayResults(const std::vector<pair<document, int>>& results);
};

#endif // UI_H
