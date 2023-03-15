#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
 
using namespace std;

vector< vector<char> > CFG;
int noOfNTs = 0;
map <string, bool> isVisited;
set <char> NonTerminals;
set <char> Terminals;
map <char, set<char>> first;
map <char, set<char>> follow;
map <char, vector<string>> productions;

bool isUpper(string s) {
    for (int i=0; i<s.size(); i++)
        if (islower(s[i]))
            return false;
    return true;
}

bool checkEpsilon(string p, char n) {
    bool allEpsilon = true;
    if (isUpper(p)) {
        for (string x: productions[p[0]]) {
            if (x != "#") {
                allEpsilon = false;
                break;
            }
        }
        if (allEpsilon)
            first[n].insert('#');
    }
    return allEpsilon;
}

// to find first of a non-terminal recursively
void findFirst(string p, char n) {
    // cout<<n<<": "<<p<<endl;
    isVisited[p] = true;
    if (islower(p[0])) {
        // if first of p is terminal, then add it to first of n
        first[n].insert(p[0]);
        return;
    }
    else {
        for (int i=0; i<p.size(); i++) {
            if (isupper(p[i]) || !isalpha(p[i]) && p[i] != '#') {
                // if first of p is epsilon, then add first of next symbols in p until first of a non-epsilon symbol is found, else add epsilon to first of n
                for (string x: productions[p[i]]) {
                    if (!isVisited[x])
                        findFirst(x, n);
                }
                first[n].insert(first[p[i]].begin(), first[p[i]].end());
                if (find(productions[p[i]].begin(), productions[p[i]].end(), "#") == productions[p[i]].end())
                    break;
                else {
                    // if all productions of n are epsilon, then add epsilon to first of n

                    if (i != p.size()-1 && !checkEpsilon(p, n))
                        first[n].erase('#');
                    // continue;
                }
            }
            else if (i > 0 && islower(p[i])) {
                if (find(productions[p[i-1]].begin(), productions[p[i-1]].end(), "#") != productions[p[i-1]].end()) {
                    // if first of previous symbol in p is epsilon, then add first of p to first of n
                    first[n].insert(p[i]);
                    break;
                }
            }
        }
        if (p == "#")
            first[n].insert('#');
    }
}

void findFollow(char c) {
    // cout<<c<<endl;
    isVisited[to_string(c)] = true;
    for (auto x: productions) {
        for (string p: x.second) {
            for (int i=0; i<p.size(); i++) {
                if (p[i] == c) {
                    if (i == p.size()-1) {
                        // if c is last symbol in p, then add follow of x.first to follow of c
                        if (isupper(x.first) && x.first != c)
                            if (!isVisited[to_string(x.first)])
                                findFollow(x.first);
                        follow[c].insert(follow[x.first].begin(), follow[x.first].end());
                    }
                    else {
                        // if c is not last symbol in p, then add first of next symbol in p to follow of c
                        for (int j=i+1; j<p.size(); j++) {
                            if (isupper(p[j])) {
                                if (!isVisited[to_string(p[j])])
                                    findFollow(p[j]);
                                // if first of p[j] is epsilon, then add first of next symbols in p until first of a non-epsilon symbol is found, else add epsilon to follow of c
                                follow[c].insert(first[p[j]].begin(), first[p[j]].end());
                                if (find(productions[p[j]].begin(), productions[p[j]].end(), "#") != productions[p[j]].end()) {
                                    follow[c].erase('#');
                                    if (j == p.size()-1)
                                        follow[c].insert(follow[x.first].begin(), follow[x.first].end());
                                    // else
                                    //     continue;
                                    // break;
                                }
                                else
                                    break;
                            }
                            else {
                                follow[c].insert(p[j]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
 
int main() {
    ifstream fin;
    fin.open("CFG5.txt");
	int curr_line = 0;
    string line;

    cout<<"\n20BCS021\nFAIZAN CHOUDHARY\n\n";
    cout<<"Given CFG: "<<endl;
    // read until EOF
 
    while (getline(fin, line)) {
    	cout<<line<<endl;
	}
	// clearing eof flags and seeking to start of file
	fin.clear();
	fin.seekg(0);
    while (getline(fin, line)) {
        char s;
        stringstream ss(line);
        vector<char> temp;
        int col = 0;
        while (ss >> s) {
            if (isupper(s)) {
                noOfNTs++;
                NonTerminals.insert(s);
                // first NT of first production is start symbol and its follow is $
                if (curr_line == 0 && col == 0)
                    follow[s].insert('$');
            }
            else if (s == '#') {
                // epsilon
                first[temp.back()].insert('#');
            }
            else {
                Terminals.insert(s);
                // first of the terminal is the terminal itself
                first[s].insert(s);
            }
            col++;
            temp.push_back(s);
        }
        // adding production to map
        string k;
        for (int i=1; i<temp.size(); i++) {
            k += temp[i];
        }
        productions[temp[0]].push_back(k);
        CFG.push_back(temp);
        curr_line++;
    }

    for (auto x: NonTerminals)
        isVisited[to_string(x)] = false;

    // FIRST
    for (char n: NonTerminals) {
        // p is a string of production
        for (string p: productions[n]) {
            findFirst(p, n);
        }
    }

    for (auto x: NonTerminals)
        isVisited[to_string(x)] = false;
    
    // FOLLOW
    for (char n: NonTerminals) {
        findFollow(n);
    }

    // for (auto x: productions) {
    //     cout<<x.first<<" : ";
    //     for (auto s: x.second)
    //         cout<<s<<" ";
    //     cout<<endl;
    // }
    cout<<endl<<"Firsts:\n";
    for (auto x: first) {
        cout<<"First("<<x.first<<") : ";
        for (auto s: x.second)
            cout<<s<<" ";
        cout<<endl;
    }

    cout<<endl<<"Follows:\n";
    for (auto x: follow) {
        cout<<"Follow("<<x.first<<") : ";
        for (auto s: x.second)
            cout<<s<<" ";
        cout<<endl;
    }
    // PARSED CFG:
    // cout<<endl;
    // for (auto c: CFG) {
    //     for (auto x: c)
    //         cout<<x<<" ";
    //     cout<<endl;
    // }
    fin.close();
    return 0;
}