#include "UI.h"
#include <iostream>
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

UI::UI(IndexHandler& handler) : indexHandler(handler) {}

void UI::mainMenu() {
    bool running = true;
    while (running) {
        std::cout << "\nMenu Options:\n"
                  << "1. Load Index from persistence file\n"
                  << "2. Load Index from article files\n"
                  << "3. Search\n"
                  << "4. Write Index to persistence file\n"
                  << "5. Exit\n"
                  << "Enter your choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();  // Clear the buffer to avoid issues with getline

        switch (choice) {
            case 1:
                {
                    loadIndexFromPersist();
                    break;
                }
            case 2:
                {
                    std::cout << "Enter filepath to read index from: ";
                    string input;
                    std::getline(std::cin, input);
                    readIndexFromArticleFiles(input);
                    break;
                }
            case 3:
                {
                    std::cout << "Enter search query: ";
                    string query;
                    std::getline(std::cin, query);
                    performSearch(query);
                    break;
                }
            case 4:
                {
                    std::cout << "Writing index to persistence file" << endl;
                    indexHandler.serializeIndex();
                    std::cout << "Successfully wrote index to persistence file" << endl;
                    break;
                }
            case 5:
                std::cout << "Exiting the program.\n";
                running = false;
                break;
            default:
                std::cout << "Invalid option, please try again.\n";
                break;
        }
    }
}

void UI::loadIndexFromPersist() {
    // todo: make this call the doc parser code
    std::cout << "Loading index from persistence file" << endl;
    indexHandler.deserializeIndex();
    std::cout << "Index loaded successfully." << endl;
}

void UI::performSearch(string query) {
    std::vector<pair<document, int>> results = indexHandler.queryIndex(query);
    displayResults(results);
}

void UI::readIndexFromArticleFiles(const std::string& filePath) {
    cout << "Loading index from: " << filePath << endl;
    indexHandler.processAllFiles(filePath);
    cout << "Successfully loaded the index from: " << filePath << endl;
}

void UI::displayResults(const std::vector<pair<document, int>>& results) {
    if (results.empty()) {
        std::cout << "No results found.\n";
        return;
    } 
    
    // todo later
    int i = 0;
    for (auto& result: results) {
        if (++i == 16) {
            break;
        }

        cout << i << ") " << result.first.docTitle << endl;
    }

    while (true) {
        cout << "Select one of the above articles or enter \'e\' to exit this menu: " << endl;
        string input;
        getline(cin, input);

        if (input.empty() || (isalpha(input[0]) && input[0] != 'e') || (isdigit(input[0]) && input[0] == '0')) {
            cout << "incorrect input" << endl;
            continue;
        }

        if (input[0] == 'e') {
            break;
        }

        int intInput = stoi(input) - 1;
        if (intInput >= results.size()) {
            cout << "incorrect input" << endl;
            continue;
        }

        // display article
        document doc = results.at(intInput).first;
        cout << doc.docTitle << endl;
        cout << doc.docAuthor << endl;
        cout << doc.docIdentifier << endl;
        cout << doc.docPath << endl;
        
        ifstream inputFile(doc.docPath);
        if (!inputFile.is_open()) {
            cerr << "Cannot open file: " << doc.docPath << "\n";
            return;
        }

        // Wrap the file inputFile stream with a RapidJSON stream wrapper
        //  and Parse the JSON data into a document object
        rapidjson::IStreamWrapper isw(inputFile);
        rapidjson::Document jsonDoc;
        jsonDoc.ParseStream(isw);

    // Extracting document details from the JSON object
        cout << jsonDoc["text"].GetString() << endl << endl << endl;
    }
}
