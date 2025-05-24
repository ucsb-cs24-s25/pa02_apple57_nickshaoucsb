// Winter'24
// Instructor: Diba Mirza
// Student name: Nicholas Shao and Anthony Lenz 
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
    int bestIndex;
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

    
    vector<int> movieIndexes;
    vector<bests> best;
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
        //cout << movieName << " has rating " << movieRating << endl;
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

    auto start = chrono::high_resolution_clock::now();
    
    for(int i = 0; i<movies.size(); i++){
        movie_list.insert(movies[i].title,i);
    }

    

    // int prefixIndex = movie_list.findPrefixIndex("ab");
    // movie_list.accumulateMovies(prefixIndex,movieIndexes);
    // for(auto i:movieIndexes){
    //     cout<<movies.at(i).title<<endl;
    // }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message

    //timing utility

    for(string prefix:prefixes){  // loop runs m times, where m is number of prefixes in prefix.txt
        movieIndexes.clear();     // clears every element in movieIndexes, as at most k movies can begin with an index,k elements cleared
        int prefixIndex = movie_list.findPrefixIndex(prefix);   //at worst, the longest prefix is the length of the longest movie name.

        if(prefixIndex==-1){
            cout << "No movies found with prefix "<< prefix <<"\n";
            continue;
        }

        movie_list.accumulateMovies(prefixIndex,movieIndexes);  // implement sorting by rating and alphabetics

        sort(movieIndexes.begin(), movieIndexes.end(), [&](int firstMovieInd, int secondMovieInd) {   //custom instructions on how to sort movieIndexes (by rating, then by alpha)
            if (movies[firstMovieInd].rating != movies[secondMovieInd].rating)
                return movies[firstMovieInd].rating > movies[secondMovieInd].rating; //sort by rating first
            return movies[firstMovieInd].title < movies[secondMovieInd].title; //if rating equal, sort by alpha
        });

        bests prefixBests;
        prefixBests.prefix = prefix;
        prefixBests.bestIndex = movieIndexes[0];
        //prefixBests.title = movies.at(movieIndexes.at(0)).title;
        //prefixBests.rating = movies.at(movieIndexes.at(0)).rating;   // Maybe this is unoptimal, could try optimizing
        best.push_back(prefixBests);

        for(auto i:movieIndexes){
            cout<<movies[i].title<<", "<<movies[i].rating<<"\n";
        }
        cout<<endl;
    }
    
    

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    for(auto b:best){
    cout << "Best movie with prefix " << b.prefix << " is: " << movies[b.bestIndex].title << " with rating " << std::fixed << std::setprecision(1) << movies[b.bestIndex].rating << "\n";
}
    auto end = chrono::high_resolution_clock::now();
    double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0; //timing utility
    //cout <<"Time: "<< time_ms << " ms"<<endl;
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block

Time Complexity analysis:
Assuming that all n movies are already stored in our data structure and all m prefixes are already stored in an array, the first 
thing to note is the outer for loop that iterates through each prefix in the array of prefixes. As there are m different prefixes,
the loop runs m times. There are many different operations that occur within this loop, so we will analyze them separately. The first
of these inner operations is the call to .clear(). The array .clear() is used on contains the indexes of movies that start with a certain prefix,
and as there are at most k movies that start with a certain prefix, it runs in O(k). Next is a function we implemented called findPrefixIndex().
It iterates through each letter of a given prefix to find the index where a specified prefix ends in our data structure trie. As a prefix can be
at most the length of the longest movie name, the outer loop of findPrefixIndex() can run at most l times, where l is the  maximum number of characters in a movie
name. Next in findPrefixIndex(), for the current node in the iteration, .find() is used with the node's hashmap of subsequent characters. As there are at most
256 possible next characters (ASCII), .find() technically runs in O(1) as the worst case cannot search more than 256 possible different mappings. The rest of
.findPrefixIndex() is all constant time, so the total time complexity of .findPrefixIndex is O(l). Next is a function we created called accumulateMovies(). 
accumulateMovies() performs a depth-first traversal of our data structure trie via a starting prefix index given by findPrefixIndex(), and then adds those movie indexes
to an array. The number of nodes depends on how many unique sequences of characters there are, and in the worst case, every movie is a unique sequence and the maximum length,
so the total number of nodes is at most n*l. Every movie can only be added once to the array, so insertions occur at most n times. Thus, at most
nl+n operations occur within accumulateMovies(), giving a time complexity of O(nl) as nl overtakes n's growth. Next executed within the loop is the sort function. We utilized
the stl sort function but modified its definition for comparison, but it still occurs in O(klogk) as the vector movie indices is at most k in length. The last operation that could
occur not in constant time is the for loop that prints each movie and its rating after sorting. As there are at most k matches per prefix, it occurs in O(k). Now accounting for the outer prefix loop,
we get that .clear() runs m times, giving O(mk) so far. Then we see that findPrefixIndex() also runs m times, giving a current total runtime of O(mk+ml). Then for accumulateMovies(),
it runs m times as well, giving a new time compexity of O(mk + ml + mnl). However, mnl will always dominate ml, giving a simplfiied complexity of O(mk + mnl). Next is the sort function, running
mklog(k) times after all iterations. Thus, our new time complexity is O(mk + mnl + mklog(k)), which is simplified to O(mklog(k)+mnl) as the new term dominates the other. Printing all rating/movie pairs
occurs in O(mk), and as this term is dominated by exisiting terms, our final time complexity of this loop is still O(mklog(k)+mnl). Outside of the loop, as we print the best movie for each prefix, it occurs
in O(m). However,this term is clearly already dominated and our final, total time complexity is O(mklog(k)+mnl).

Timing Results:
Size 20: 637.1 ms
Size 100: 537.5 ms
Size 1000: 487.7 ms
Size 76920: 3068.6 ms

Space Complexity Analysis:
In our program's memory, the most space-dominant variables to consider are:
- The trie holding every title,
- The list of movie indexes stored at the final node of each title,
- Prefix-search results stored in movieIndexes
- The vector containing the best movie of every prefix
- The recursion depth caused by the depth-first-search of accumulateMovies()

In the worst case scenario, each node in the trie would only hold one character, taking up n total movies times l longest 
movie length nodes, and each 'edge' between each character would also take the same space.

The list of movie indexes would only hold the amount of total movies, or n.

In the worst case scenario, the prefix-search results would take up k movies, as at most k movies can share a prefix.

The vector containing the best movie of every prefix in the worst case would be of size m, as there are at most m prefixes and each prefix
can only have one best movie.

The recursive depth of a depth first search in our trie would be as large as the longest prefix, thus giving O(l).

Therefore, if we store all results, the total extra memory would be O(n * l) + O(n) + O(k) + O(m) + O(l) = O((n * l) + k + m).

Exploring Tradeoffs:
We designed our solution with low time complexity in mind. Our target complexity was O(n). 
I think our solution did not have the lowest possible time complexity, but as a result of that, our space complexity was relatively
low as well. Every structure used within our solution stayed linear in space complexity except our trie, and even that was not unecessarily large.
However, it is noteworthy that our trie relied on the specific position of characters (i.e. an 'a' in the front of a word would be a different
element than an 'a' in as the second character in a word), thus potentially creating a source of minimizable space usage. It is definitely evident
that if we wanted to achieve an even lower time complexity, we might need to utilize more data structures and thus trade higher space complexity for
lower time complexity.

*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}