// Winter'24
// Instructor: Diba Mirza
// Student name: 
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>

#include <chrono>

using namespace std;

#include "utilities.h"
#include "movies.h"

struct bests{
    string prefix;
    string title;
    double rating;
};

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    vector<Movie> movies;
    MovieList movie_list;

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        // Use std::string movieName and double movieRating
        // to construct your Movie objects
        Movie m(movieName, movieRating);
        // cout << movieName << " has rating " << movieRating << endl;
        cout << movieName << " has rating " << movieRating << endl;
        // insert elements into your data structure
        movies.push_back(m);
    }

    movieFile.close();

    if (argc == 2){
        set<Movie> movie_set;
        for (auto m : movies) 
            movie_set.insert(m);
        cout << endl;
        for (auto m : movie_set)
            cout << m.title << ", " << m.rating << endl;
        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    auto start = chrono::high_resolution_clock::now();//timing utility
    vector<int> movieIndexes;
    vector<bests> best;
    
    for(int i = 0; i<movies.size(); i++){
        movie_list.insert(movies.at(i).title,i);
    }

    // int prefixIndex = movie_list.findPrefixIndex("ab");
    // movie_list.accumulateMovies(prefixIndex,movieIndexes);
    // for(auto i:movieIndexes){
    //     cout<<movies.at(i).title<<endl;
    // }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message

    

    for(string prefix:prefixes){
        movieIndexes.clear(); 
        int prefixIndex = movie_list.findPrefixIndex(prefix);

        if(prefixIndex==-1){
            cout << "No movies found with prefix "<< prefix <<"\n";
            continue;
        }

        movie_list.accumulateMovies(prefixIndex,movieIndexes);  // implement sorting by rating and alphabetics

        sort(movieIndexes.begin(), movieIndexes.end(), [&](int firstMovieInd, int secondMovieInd) {   //custom instructions on how to sort movieIndexes (by rating, then by alpha)
            if (movies.at(firstMovieInd).rating != movies.at(secondMovieInd).rating)
                return movies.at(firstMovieInd).rating > movies.at(secondMovieInd).rating; //sort by rating first
            return movies.at(firstMovieInd).title < movies.at(secondMovieInd).title; //if rating equal, sort by alpha
        });

        bests prefixBests;
        prefixBests.prefix = prefix;
        prefixBests.title = movies.at(movieIndexes.at(0)).title;
        prefixBests.rating = movies.at(movieIndexes.at(0)).rating;   // Maybe this is unoptimal, could try optimizing
        best.push_back(prefixBests);

        for(auto i:movieIndexes){
            cout<<movies.at(i).title<<", "<<movies.at(i).rating<<"\n";
        }
        cout<<endl;
    }
    
    

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    for(auto b:best){
    cout << "Best movie with prefix " << b.prefix << " is: " << b.title << " with rating " << std::fixed << std::setprecision(1) << b.rating << "\n";
}
    auto end = chrono::high_resolution_clock::now();
    double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0; //timing utility
    cout <<"Time: "<< time_ms << " ms"<<endl;
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}