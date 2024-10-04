#include <iostream>
#include "UI.h"
#include "IndexHandler.h"

using namespace std;

int main(int argc, char **argv) {
    // if (argc < 2) {
    //     cout << "Usage:\n"
    //          << "\tLoad the existing index and perform the following query:\n"
    //          << "\tsupersearch query \"your_query_here\"\n\n"
    //          << "\tStart a simple text-based user interface that lets the user load an index and perform multiple queries:\n"
    //          << "\tsupersearch ui\n\n";
    //     return 1;  // Exit with error code if insufficient arguments are provided
    // }

    // string command = argv[1];
    IndexHandler indexHandler;  // Create an IndexHandler instance

    // if (command == "query" && argc >= 3) {
    //     UI ui(indexHandler);
    //     string query = argv[2];
    //     ui.loadIndexFromPersist();  // Assuming there's a default or previously set index path
    //     ui.performSearch(query);
    // } else if (command == "ui") {
        UI ui(indexHandler);
        ui.mainMenu();
    // } else {
    //     cout << "Invalid command. Please check usage." << endl;
    //     return 1;
    // }
    return 0;
}


