#include "movies.h"
using namespace std; 

void MovieList::insert(const string& MovieTitle, int index){  //int index is the movie's index within global Movies vector
    int currIndex = 0; //current index within the vector of Nodes Trie

    for(char c:MovieTitle){
        auto& letterMapping = trie[currIndex].next;  //this is the hashmap containing next letters
        auto letterTracker = letterMapping.find(c); //checks if there already exists a mapping for the current character c

        if(letterTracker == letterMapping.end()){ //if existing mapping not found for current character 
            int nextFreeTrieIndex = trie.size();  //next available index in trie once new element added, as indices will = size-1+1
            letterMapping.emplace(c,nextFreeTrieIndex);  //adds a new mapping to the "dictionary" for new character c
            trie.emplace_back();            //creates empty node to modify on next character iteration
            currIndex = nextFreeTrieIndex;    //moves current index to index just created, on next pass will work on node just created
        }
        else{ //character mapping already exists
            currIndex = letterTracker->second;  //gets the next index within trie for this word via the key,value mapping in hashmap next
        }
    }
    trie[currIndex].titleIndices.push_back(index); //the final node in sequence of characters (the index the final character mapped to) contains the index of the movie in Movies
}

void MovieList::accumulateMovies(int index, vector<int>& movieIndexes){ //this function starts from a specified point within Trie (index) and gathers indexes of movies stored within each node's titleIndices
    movieIndexes.insert(movieIndexes.end(),trie[index].titleIndices.begin(),trie[index].titleIndices.end()); //titleIndices contains the indices of movies whose titles end at this letter in trie, 
    for(auto& mapping : trie[index].next){                                                                      //so collect those indexes into a vector to store
        int nextIndex = mapping.second;     //recursively explore each mapping within the hashmap next and collect those indices as well,
        accumulateMovies(nextIndex,movieIndexes); // and will eventually reach a node with no mappings, end there
    }
}

<<<<<<< Updated upstream
int MovieList::findPrefixIndex(const string& prefix){ //this function finds which index within Trie the specified prefix ends
    int currIndex = 0;

    for(char c:prefix){ // iterates through each character in the prefix
        auto& letterMapping = trie.at(currIndex).next; // gets the hashmap for 
        auto letterTracker = letterMapping.find(c); //
        if(letterTracker==letterMapping.end()){
=======
int MovieList::findPrefixIndex(const string& prefix){//this function finds which index within Trie the specified prefix ends
    int currIndex = 0; //current index within trie aka vector of nodes

    for(char c:prefix){ //iterate through each character in the prefix
        auto& letterMapping = trie[currIndex].next; //get the hashmap for the node at currIndex
        auto letterTracker = letterMapping.find(c); //within the mappings for this hashmap, see if there exists a mapping for key char c
        if(letterTracker==letterMapping.end()){ //no mappings for the current char path, means no movies with specified prefix exist
>>>>>>> Stashed changes
            return -1;
        }
        else{
            currIndex = letterTracker->second; //if current char mapping exists, move onto the next character in prefix and move to the next mapping
        }
    }
    return currIndex; //if the entire prefix existed within a mapping in trie, return the index where the prefix ends (useful to find all words with prefix)
}
