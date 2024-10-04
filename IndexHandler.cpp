#include "IndexHandler.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <set>

#include "document.h"
#include "porter2_stemmer/porter2_stemmer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

using namespace rapidjson;

IndexHandler::IndexHandler() {
    // Initializer - Tree constructors are called automatically.
}

//add word to index 
void IndexHandler::addWordToIndex(const std::string& term, const document& doc) {
    map<document, int>& documentMap = mainIndex[term];
    if(documentMap.count(doc)){
        documentMap[doc]++;
    }else{
        documentMap[doc]=1;
    }
}

//add word to index 
void IndexHandler::addPersonToIndex(const std::string& term, const document& doc) {
     map<document, int>& documentMap = personIndex[term];
    if(documentMap.count(doc)){
        documentMap[doc]++;
    }else{
        documentMap[doc]=1;
    }
}

//add word to index 
void IndexHandler::addOrgToIndex(const std::string& term, const document& doc) {
     map<document, int>& documentMap = organizationIndex[term];
    if(documentMap.count(doc)){
        documentMap[doc]++;
    }else{
        documentMap[doc]=1;
    }
}


void IndexHandler::removeFromIndex(const std::string& term, const document& doc) {
    map<document, int>& postings = mainIndex[term];
    postings.erase(doc);
    if (postings.empty()) {
        mainIndex.remove(term);
    }
}

std::vector<pair<document, int>> IndexHandler::queryIndex(const std::string& query) const {
    std::vector<std::string> words;
    std::vector<std::string> notWords;
    std::vector<std::string> people;
    std::vector<std::string> notPeople;
    std::vector<std::string> orgs;
    std::vector<std::string> notOrgs;
    std::vector<std::string> terms = splitIntoWords(query);

    for (size_t i = 0; i < terms.size(); ++i) {
        bool isWord = false;
        bool isPerson = false;
        bool isOrg = false;
        bool isNot = false;
        
        
        std::string processedTerm = terms[i];

        if (processedTerm.substr(0, 4) == "ORG:") {
            processedTerm = processedTerm.substr(4);
            isOrg = true;
        } else if (processedTerm.substr(0, 7) == "PERSON:") {
            processedTerm = processedTerm.substr(7);
            isPerson = true;
        } else {
            isWord = true;
        }

        if (processedTerm.empty()) {
            continue;
        } else if (processedTerm[0] == '-') {
            processedTerm = processedTerm.substr(1);
            isNot = true;
        }

        transform(processedTerm.begin(), processedTerm.end(), processedTerm.begin(), ::tolower);
        int j =0;
        for(int i =0; i<processedTerm.size(); ++i){
            processedTerm[j] = processedTerm[i]; 
            if(isalpha(processedTerm[j])) {
                j++;
            }
        }
        processedTerm.resize(j);
        Porter2Stemmer::stem(processedTerm);

        if (isOrg) {
            if (isNot) {
                notOrgs.push_back(processedTerm);
            } else {
                orgs.push_back(processedTerm);
            }
        } else if (isPerson) {
            if (isNot) {
                notPeople.push_back(processedTerm);
            } else {
                people.push_back(processedTerm);
            }
        } else if (isWord) {
            if (isNot) {
                notWords.push_back(processedTerm);
            } else {
                words.push_back(processedTerm);
            }
        }
    }

    map<document, int> resultsMap;

    for(int i=0; i<words.size(); ++i){
        if (mainIndex.contains(words[i])==false) {
            continue;
        }

        const map<document, int>& docsForWord = mainIndex.at(words[i]);
        if (resultsMap.empty() == true) {
            resultsMap = docsForWord;
            continue;
        }

        for(const auto& ele: docsForWord) {
            if (docsForWord.count(ele.first) == true) {
                resultsMap[ele.first] += ele.second;
            } else {
                resultsMap.erase(ele.first);
            }
        }
    }
    
    for(int i=0; i<orgs.size(); ++i){
        if (organizationIndex.contains(orgs[i])==false) {
            continue;
        }

        const map<document, int>& docsForOrg = mainIndex.at(orgs[i]);
        if (resultsMap.empty() == true) {
            resultsMap = docsForOrg;
            continue;
        }

        for(const auto& ele: docsForOrg) {
            if (docsForOrg.count(ele.first) == true) {
                resultsMap[ele.first] += ele.second;
            } else {
                resultsMap.erase(ele.first);
            }
        }
    }
    
    for(int i=0; i<people.size(); ++i){
        if (personIndex.contains(people[i])==false) {
            continue;
        }

        const map<document, int>& docsForPerson = mainIndex.at(people[i]);
        if (resultsMap.empty() == true) {
            resultsMap = docsForPerson;
            continue;
        }

        for(const auto& ele: docsForPerson) {
            if (docsForPerson.count(ele.first) == true) {
                resultsMap[ele.first] += ele.second;
            } else {
                resultsMap.erase(ele.first);
            }
        }
    }

    for(int i=0; i<notWords.size(); ++i){
        if (mainIndex.contains(notWords[i])==false) {
            continue;
        }

        const map<document, int>& docsForWord = mainIndex.at(notWords[i]);
        for(const auto& ele: docsForWord) {
            resultsMap.erase(ele.first);
        }
    }

    for(int i=0; i<notOrgs.size(); ++i){
        if (organizationIndex.contains(notOrgs[i])==false) {
            continue;
        }

        const map<document, int>& docsForWord = mainIndex.at(notOrgs[i]);
        for(const auto& ele: docsForWord) {
            resultsMap.erase(ele.first);
        }
    }
    
    for(int i=0; i<notPeople.size(); ++i){
        if (personIndex.contains(notPeople[i])==false) {
            continue;
        }

        const map<document, int>& docsForWord = mainIndex.at(notPeople[i]);
        for(const auto& ele: docsForWord) {
            resultsMap.erase(ele.first);
        }
    }

    vector<pair<document, int>> resultsVec(resultsMap.begin(), resultsMap.end());

    sort(resultsVec.begin(), resultsVec.end(), [](pair<document, int>& p1, pair<document, int>& p2) {
        return p1.second > p2.second;
    });

    return resultsVec;
}

void IndexHandler::serializeIndex() {
    std::ofstream out("wordTree.txt");
    serializeTree(out, mainIndex);
    out.close();

    std::ofstream out0("personTree.txt");
    serializeTree(out0, personIndex);
    out0.close();

    std::ofstream out1("organizationTree.txt");
    serializeTree(out1, organizationIndex);
    out1.close();
}

void IndexHandler::deserializeIndex() {
    std::ifstream in("wordTree.txt");
    if (in.good() == false) {
        cerr << "could not open wordTree.txt" << endl;
    }
    deserializeTree(in, mainIndex);
    in.close();

    std::ifstream in0("personTree.txt");
    deserializeTree(in0, personIndex);
    in0.close();

    std::ifstream in1("organizationTree.txt");
    deserializeTree(in1, organizationIndex);
    in1.close();
}

void IndexHandler::serializeTree(std::ofstream& out, const AvlTree<std::string, std::map<document, int>>& tree) {
    tree.traverse([&out](const std::string& key, const std::map<document, int>& documents) {
        out << key << endl;
        for (const auto& doc : documents) {
            out << doc.first.docTitle<< endl;
            out << doc.first.docIdentifier<< endl;
            out << doc.first.docPublicationDate<< endl;
            out << doc.first.docAuthor<< endl;
            out << doc.first.docPath<< endl;
           
            out << doc.second << endl;
        }
        out << '-' << endl;
    });
    out << '=' << endl;
}

//! make sure to test this function out with the sample data
void IndexHandler::deserializeTree(std::ifstream& in, AvlTree<std::string, std::map<document, int>>& tree) {
    std::string line;
    while(in.eof() == false){
        getline(in, line);
        string word = line;
        if(word=="="){
            break;
        }

        while(true){
            getline(in, line);
            string docTitle = line;
            if(docTitle=="-"){
                break;
            }
            getline(in, line);
            string docIdentifier = line;
            getline(in, line);
            string docPublicationDate = line;
            getline(in, line);
            string docAuthor = line;
            getline(in, line);
            string docPath = line;
            getline(in, line);
            int freq = stoi(line);

            document doc(docTitle, docIdentifier, docPublicationDate, docAuthor, docPath);
            tree[word].emplace(doc, freq);
        }
    }
}

size_t IndexHandler::getNumberOfIndexedTerms() const {
    // Assuming each tree is independent and there are no shared terms across them
    return mainIndex.size() + personIndex.size() + organizationIndex.size();
}

// -----------------------------


// Parses a JSON file and indexes its contents into various AVL trees
void IndexHandler::parseJsonDocument(const string &filePath) {
    cout << "Current directory: " << std::filesystem::current_path() << endl;
    ifstream input(filePath);
    if (!input.is_open()) {
        cerr << "Cannot open file: " << filePath << "\n";
        cerr << "Attempted to open from directory: " << std::filesystem::current_path() << endl;
        return;
    }

    // Wrap the file input stream with a RapidJSON stream wrapper
    //  and Parse the JSON data into a document object
    IStreamWrapper isw(input);
    Document jsonDoc;
    jsonDoc.ParseStream(isw);

   // Extracting document details from the JSON object
    string uuid = jsonDoc["uuid"].GetString();
    string content = jsonDoc["text"].GetString();
    string author = jsonDoc["author"].GetString();
    string title = jsonDoc["title"].GetString();

    // Check if 'published' field exists before accessing it
    string published = jsonDoc.HasMember("published") && !jsonDoc["published"].IsNull() ? jsonDoc["published"].GetString() : "Unknown";

    // Creating and storing the document object in the document AVL tree
    document newDocument(title, uuid, published, author, filePath);

    // Processing person entities and updating the people AVL tree
    auto persons = jsonDoc["entities"]["persons"].GetArray();
    for (auto &person : persons) {
        string personName = person["name"].GetString();
        transform(personName.begin(), personName.end(), personName.begin(), ::tolower);
        replace(personName.begin(), personName.end(), ' ', '-');
        addPersonToIndex(personName, newDocument);
    }

    // Processing organization entities and updating the organizations AVL tree
    auto orgs = jsonDoc["entities"]["organizations"].GetArray();
    for (auto &org : orgs) {
        string orgName = org["name"].GetString();
        transform(orgName.begin(), orgName.end(), orgName.begin(), ::tolower);
        replace(orgName.begin(), orgName.end(), ' ', '-');
        addOrgToIndex(orgName, newDocument);
    }

    // Tokenizing the document's text and updating the word AVL tree
    vector<string> words = splitIntoWords(content);
    for (auto &word : words) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        // todo: remove special characters
        int j =0;
        for(int i =0; i<word.size(); ++i){
            word[j] = word[i]; 
            if(isalpha(word[j])) {
                j++;
            }
        }
        word.resize(j);

        if(stopWordTree.contains(word) || word.empty()){
            continue;
        }
        Porter2Stemmer::stem(word);
        addWordToIndex(word, newDocument);
    }

    // Closing the input file
    input.close();
}





// Splits the provided text into words, considering only alphanumeric characters as part of words.
vector<string> IndexHandler::splitIntoWords(const string &inputText) const {
    vector<string> extractedWords; // Stores the extracted words
    string wordBuffer; // Dynamic buffer for assembling words

    // Iterate over each character in the input text
    for (char currentChar : inputText) {
        // Check if the current character is alphanumeric (part of a word)
        if (isalnum(currentChar)) {
            wordBuffer += tolower(currentChar); // Add to buffer and convert to lowercase
        } else if (!wordBuffer.empty() && currentChar != '\'') { // Ensure apostrophes within words are not considered as delimiters
            extractedWords.push_back(wordBuffer); // Add the current word to the list
            wordBuffer.clear(); // Clear the buffer for the next word
        }
    }

    // Add the last word if the text did not end with a non-alphanumeric character
    if (!wordBuffer.empty()) {
        extractedWords.push_back(wordBuffer); // Add the last word to the list
    }

    return extractedWords; // Return the list of words
}




// Iterates through all files within the specified directory and its subdirectories,
// processing each JSON file to populate indexing structures and counting processed documents.
void IndexHandler::processAllFiles(const string &directoryPath) {
    // Use recursive directory iterator to access all files and subdirectories
    auto it = filesystem::recursive_directory_iterator(directoryPath);

    // Load stop words at the start of the file processing
    loadExclusionWords();

    // Reset the counter for the number of documents processed
    totalDocumentsProcessed = 0;

    // Loop through each file in the directory
    for (const auto &entry : it) {
        // Check if the file is a regular file and has a .json extension
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            // Read and process the JSON file
            parseJsonDocument(entry.path().string());
            totalDocumentsProcessed++; // Increment the count of processed documents

            // Provide a status update every 10000 documents processed
            if (totalDocumentsProcessed % 10000 == 0) {
                cout << totalDocumentsProcessed << " documents processed." << endl;
            }
        }
    }

    // Output the total number of documents processed after completion
    cout << totalDocumentsProcessed << " total documents processed." << endl;
}


// Loads stop words from a file into an AVL tree to filter out common words during parsing.
void IndexHandler::loadExclusionWords() {
    ifstream input("../stopwords.txt"); // Open the stopwords file

    // Check if the file was successfully opened
    if (!input.is_open()) {
        throw runtime_error("Failed to open the stopwords file.");
    }

    string word;
    // Read each line from the file as a stop word
    while (getline(input, word)) {
        stopWordTree.insert(word, ""); // Insert each stop word into the AVL tree
    }

    input.close(); // Close the file after reading all words
}

