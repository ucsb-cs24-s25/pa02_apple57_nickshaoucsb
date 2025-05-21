#include "movies.h"
#include <unordered_map>
#include <vector>
using namespace std; 

struct Movie {
    string title;
    double rating;
};

vector<Node> trie(1);
vector<Movie> Movies; 

struct Node{
    unordered_map<char,int> next; //An unordered map of the next possible characters following the current one
    vector<int>  titleIndices; //A vector of integers storing the indices of movies whose title ends at this letter node (indices within global Movies vector for fast retrieval)
};

void insertMovie(const string& MovieTitle, int index){  //int index is the movie's index within global Movies vector
    int currIndex = 0; //current index within the vector of Nodes Trie

    for(char c:MovieTitle){
        auto& letterMapping = trie.at(currIndex).next;  //this is the hashmap containing next letters
        auto letterTracker = letterMapping.find(c); //checks if there already exists a mapping for the current character c

        if(letterTracker == letterMapping.end()){ //if existing mapping not found for current character 
            int nextFreeTrieIndex = trie.size();  //next available index in trie once new element added , might need to cast size
            letterMapping.emplace(c,nextFreeTrieIndex);  //adds a new mapping to the "dictionary" for new character c
            trie.emplace_back();            //creates empty node to modify on next character iteration
            currIndex = nextFreeTrieIndex;    //moves current index of focus to index just created
        }
        else{ //character mapping already exists
            currIndex = letterTracker->second;  //gets the next index within trie for this word via the key,value mapping in hashmap next
        }
    }
    trie.at(currIndex).titleIndicies.push_back(index); //the final node in sequence of characters (the index the final character mapped to) contains the index of the movie in Movies
}

