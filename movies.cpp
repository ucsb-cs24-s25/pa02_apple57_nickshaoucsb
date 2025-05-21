#include "movies.h"
using namespace std; 

void MovieList::insert(const string& MovieTitle, int index){  //int index is the movie's index within global Movies vector
    int currIndex = 0; //current index within the vector of Nodes Trie

    for(char c:MovieTitle){
        auto& letterMapping = trie.at(currIndex).next;  //this is the hashmap containing next letters
        auto letterTracker = letterMapping.find(c); //checks if there already exists a mapping for the current character c

        if(letterTracker == letterMapping.end()){ //if existing mapping not found for current character 
            int nextFreeTrieIndex = trie.size();  //next available index in trie once new element added , might need to cast size
            letterMapping.emplace(c,nextFreeTrieIndex);  //adds a new mapping to the "dictionary" for new character c
            trie.emplace_back();            //creates empty node to modify on next character iteration
            currIndex = nextFreeTrieIndex;    //moves current index to index just created
        }
        else{ //character mapping already exists
            currIndex = letterTracker->second;  //gets the next index within trie for this word via the key,value mapping in hashmap next
        }
    }
    trie.at(currIndex).titleIndices.push_back(index); //the final node in sequence of characters (the index the final character mapped to) contains the index of the movie in Movies
}

void MovieList::accumulateMovies(int index, vector<int>& movieIndexes){ //this function starts from a specified point within Trie (index) and gathers indexes of movies stored within each node's titleIndices
    movieIndexes.insert(movieIndexes.end(),trie.at(index).titleIndices.begin(),trie.at(index).titleIndices.end());

    for(auto mapping : trie.at(index).next){
        int nextIndex = mapping.second;
        accumulateMovies(nextIndex,movieIndexes);
        return;
    }
    return;
}

int MovieList::findPrefixIndex(const string& prefix){//this function finds which index within Trie the specified prefix ends
    int currIndex = 0;

    for(char c:prefix){
        auto& letterMapping = trie.at(currIndex).next;
        auto letterTracker = letterMapping.find(c);
        if(letterTracker==letterMapping.end()){
            return -1;
        }
        else{
            currIndex = letterTracker->second;
        }
    }
    return currIndex;
}
