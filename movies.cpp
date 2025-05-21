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
    vector<int>  titleIndices; //A vector of integers storing the indices of movies whose title ends at this letter node (indices within global movie vector for fast retrieval)
};

void insertMovie(const string& MovieTitle, int index){  //int index is the movie's index within global Movies vector
    int currIndex = 0; //current index within the vector of Nodes Trie

    for(char c:MovieTitle){
        auto& nextLetters = trie.at(currIndex).next;
    }
}