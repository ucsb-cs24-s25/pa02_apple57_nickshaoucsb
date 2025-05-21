#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Movie {
    string title;
    double rating;
    Movie(string t, double r) : title(t), rating(r) {};
    bool operator<(const Movie& other) const{
        return title < other.title;
    }
};

struct Node{
    unordered_map<char,int> next; // unordered map of the next possible characters
    vector<int> titleIndices; // vector of ints storing indices of movies whose title ends at this letter node (indices within global Movies vector for fast retrieval)
};



class MovieList {
    public:
        MovieList() : trie(1) {};
        void insert(const string& MovieTitle, int index);
        void accumulateMovies(int index, vector<int>& moveIndexes);
        int findPrefixIndex(const string& prefix);
        
    private:
        vector<Node> trie;
    
    
};
