# Assignment 4: Answers

**Complete this document, commit your changes to Github and submit the repository URL to Canvas.** Keep your answers short and precise.

Names of all team members: Razkia Rouabah, Abderrahim Latreche


Used free extension: [ ] 24 hrs or [ x ] 48 hrs 
(Each of us had two 24 hour extensions remaining, so in total we used a 48 hour extension)

[ ] Early submission (48 hrs)

[ ] Bonus work. Describe: ...

Place [x] for what applies.


## Answers

### Project Management

Complete the tasks and planned completion columns **BEFORE** You start with 
coding!


Weekly milestones/tasks. Set out a plan at the beginning and then record when you got it done.

| Task        | Planned completion | Actual completion | Time spent in hrs by Raz | Time spent in hrs by Rahim |
| ----------- | :-----------------:| :---------------: | :--------------------:   | :--------------------:     |
| AVL Tree    | Apr 15             | Apr 14 - Apr 30   | 16+                      |                            |
| Document    | Apr 28             | Apr 30            |                          | 2                          |
| Docparsing functions  (merged in indexHandler class) | Apr 28      | Apr 30   | 1     | 7 |
| Index       | Apr 24             | Apr 26            | 12                       | 
| Main        | Apr 28             | Apr 30            |                          | 2                          |
| Ui          | Apr 28             | Apr 30            |                          | 12+                        |
| Answers.md  | Apr 28             | Apr 30            | 6                        | 0.5                        |
| UML         | Apr 30             | Apr 30            | 0                        | 1                          |


# High-Level Pseudo Code for Search Engine Components

## IndexHandler Component

### Constructor
- Initialize AVL Trees for terms, people, and organizations

### AddWordToIndex(term, document)
- Find or create node in mainIndex for term
- Add document to the map associated with the term, increase frequency count

### AddPersonToIndex(person, document)
- Similar to AddWordToIndex, but for personIndex

### AddOrgToIndex(organization, document)
- Similar to AddWordToIndex, but for organizationIndex

### RemoveFromIndex(term, document)
- Locate term in mainIndex
- Remove document from the map, or decrement count
- If map is empty, remove term from the tree

### SerializeIndex()
- Traverse each tree (mainIndex, personIndex, organizationIndex)
- Serialize nodes to a file including term and associated documents and counts

### DeserializeIndex()
- Read from files
- Rebuild each tree (mainIndex, personIndex, organizationIndex) from serialized data

### QueryIndex(query)
- Parse query into terms, people, organizations
- Retrieve documents for each term from corresponding trees
- Aggregate results based on frequency and relevance
- Return sorted list of documents based on aggregated scores

## Query Component

### ProcessQuery(query, wordIndex, personIndex, organizationIndex, stopWordTree)
- Initialize documentScores map
- Split query into terms
- For each term:
  - If term is a stopword, continue
  - If term starts with "ORG:", process as organization
  - If term starts with "PERSON:", process as person
  - Otherwise, process as general term
- Apply any exclusions based on terms starting with '-'
- Rank results based on documentScores
- Return sorted list of results

### ProcessTerms(term, index, scores)
- If term exists in index:
  - Retrieve document map
  - For each document in map, add score to scores map

### ProcessOrganizations(organization, index, scores)
- Similar to ProcessTerms, but specific to organizations

### ProcessPeople(person, index, scores)
- Similar to ProcessTerms, but specific to people

### ApplyExclusions(excludedTerms, scores)
- For each term in excludedTerms:
  - Remove term and associated documents from scores map

### RankResults(scores)
- Convert scores map to list
- Sort list by scores in descending order
- Return sorted list

## UI Component

### Constructor(handler)
- Initialize with reference to IndexHandler

### MainMenu()
- Display menu options repeatedly until user exits:
  - Load index from persistence
  - Load index from article files
  - Search
  - Write index to persistence
  - Exit

### LoadIndexFromPersist()
- Call IndexHandler's DeserializeIndex method
- Display success message

### LoadIndexFromArticleFiles(filePath)
- Call IndexHandler's ProcessAllFiles method with filePath
- Display number of documents processed and success message

### PerformSearch(query)
- Call IndexHandler's QueryIndex with query
- Display results or message if no results found

### DisplayResults(results)
- For each result in results:
  - Print document title and brief metadata
  - Allow user to select document to view more details or exit


### Used Data Structures
Explanation of what data structures are used where. For each, state
    - time and space complexity (Big-Oh),
    - why it is the appropriate data structure and what other data structure could have been used.


This section outlines the data structures utilized in the search engine project, explaining their use in specific components like `IndexHandler` and `Query`, along with their complexities, justification for their use, and potential alternatives.

## Detailed Explanation

### IndexHandler Specifics

- **AVL Trees**
  - **Usage**: Storing indexes for terms, organizations, and people.
  - **Time Complexity**: \(O(\log n)\) for insertion, deletion, and search operations.
  - **Space Complexity**: \(O(n)\).
  - **Justification**: Ensures that indices remain balanced, providing efficient search times which are crucial for quickly finding relevant documents during query processing.
  - **Alternative**: Red-Black Trees could be used for similar benefits with slightly different balance and performance characteristics.

- **Maps**
  - **Usage**: Linking terms to their respective documents and occurrences within AVL tree nodes.
  - **Time Complexity**: \(O(\log n)\).
  - **Space Complexity**: \(O(n)\).
  - **Justification**: Ideal for relevance calculations during searches where the frequency of terms in documents impacts their relevance scores.
  - **Alternative**: `std::unordered_map` for potentially faster average-case performance at the expense of losing order.

### Query Specifics

- **Vectors**
  - **Usage**: Storing and managing search results dynamically.
  - **Time Complexity**: \(O(1)\) amortized for appending elements, \(O(n)\) for accessing specific elements.
  - **Space Complexity**: \(O(n)\).
  - **Justification**: Allows efficient appending of new results and random access to any result, supporting dynamic result management.

- **Sets**
  - **Usage**: Managing unique identifiers or terms, especially for filtering results based on stopwords.
  - **Time Complexity**: \(O(\log n)\).
  - **Space Complexity**: \(O(n)\).
  - **Justification**: Efficient at preventing duplicates and fast checks for existence, critical for managing stopwords.
  - **Alternative**: `std::unordered_set` for faster average-case performance but without the inherent order.

### Additional Considerations

- **Document Class (`document.h`)**
  - **Purpose**: Organizes document metadata for easy retrieval and comparison, supporting sorting and displaying search results.

### User Documentation - What are the features of your software?

Our search engine software, named "supersearch", provides the following key features:

1. **Indexing**: The software can process a large collection of financial news articles stored in JSON format. It builds an inverted index using AVL trees to efficiently store and retrieve the mapping between index terms and the corresponding documents. The indexing process includes:
   - Parsing each news article
   - Removing stopwords
   - Stemming words
   - Extracting relevant information such as article metadata and entities (organizations and persons)

2. **Querying**: Users can search for articles using a simple query language. The query processor supports the following features:
   - Searching for articles containing specific words
   - Excluding articles containing specific words using the "-" operator
   - Searching for articles mentioning specific organizations using the "ORG:" operator
   - Searching for articles mentioning specific persons using the "PERSON:" operator
   - Combining multiple search terms and operators to refine the search results

3. **Ranking**: The search results are ranked based on relevancy. The relevancy ranking takes into account factors such as term frequency and inverse document frequency (TF-IDF) to determine the importance of each search term in the articles.

4. **User Interface**: The software provides a simple text-based user interface with the following options:
   - Creating an index from a directory of JSON article files
   - Writing the index to files for persistence and reading the index from files
   - Entering search queries and displaying the top 15 search results
   - Viewing the complete text of a selected article from the search results
   - Displaying basic statistics about the search engine, such as indexing time, query time, total number of indexed articles, and total number of unique indexed words

5. **Persistence**: The inverted index is made persistent by storing the AVL trees to files. This allows the index to be loaded from files in subsequent runs of the software, avoiding the need to rebuild the index each time.

6. **Performance**: The software is designed to handle a large dataset of financial news articles efficiently. It optimizes indexing and query processing speed through the use of appropriate data structures and algorithms.

7. **Extensibility**: The modular design of the software allows for easy extension and customization. New features such as additional query operators, advanced relevancy ranking algorithms, or different data formats can be incorporated with minimal changes to the existing codebase.

Our search engine software provides a solution for indexing, searching, and retrieving relevant financial news articles based on user queries. It offers a simple yet powerful interface for users to explore and discover information from a large corpus of articles.

# How to Use the Search Engine Software

## Overview

This document provides instructions on how to operate the search engine, which includes loading indexes, performing searches, and managing indexed data.

## Using the Software

### Starting the Application

Run the program from the command line which will bring up the main menu with several options.

### Menu Options

1. **Load Index from Persistence File**: Load previously indexed data from a file.
2. **Load Index from Article Files**: Index data from specified article files.
3. **Search**: Perform a search query to find relevant documents.
4. **Write Index to Persistence File**: Save the current index to a file for future use.
5. **Exit**: Exit the application.

### Step-by-Step Usage

1. **Loading an Index**:
   - Select `1` from the main menu to load an index from a persistence file.
   - The system will confirm once the index is loaded successfully.

2. **Indexing Documents**:
   - Select `2` to load and index documents from a specified directory.
   - You will be prompted to enter the file path to the directory containing the article files (e.g., `../sample_data`).
   - The system will display each document being processed and confirm the total number of documents indexed.

3. **Performing Searches**:
   - Select `3` to perform a search.
   - Enter your search query (e.g., "german").
   - The system will display search results, listing relevant documents with a brief description.

4. **Saving an Index**:
   - Choose `4` to write the current state of the index to a persistence file.
   - The system will confirm once the index has been successfully written to the file.

5. **Exiting**:
   - Select `5` to exit the application.

## Example Queries

### Query: "german"

**Action**: Enter `3` for Search, then input "german".

**Results**:

German firms doing business in UK gloomy about Brexit - survey
Select one of the above articles or enter 'e' to exit this menu:


**Follow-Up**:
- Selecting `1` will display the full details of the selected article, including the title, author, unique identifier, and the article's content.

**Article Details**:

German firms doing business in UK gloomy about Brexit - survey
Reuters Editorial
e17252151a2d36344b7104d14805414a55042abe
../sample_data/coll_1/news_0064567.json

[Article Content]
"BERLIN (Reuters) - German businesses that trade in Britain are pessimistic about their prospects there, and some are planning to move operations to other markets, a survey by Germany’s DIHK Chambers of Industry and Commerce showed on Tuesday. [...]"


**Exiting the Menu**:
- Enter 'e' to return to the main menu.

### Performance
- Provide statistics (word counts, timing, etc) for indexing all documents (or as many as you can in a reasonable amount of time).
    
### Bonus Work
Did you attempt any bonuses? If so, please give a brief description of what you did.

   > Your answer
