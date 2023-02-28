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

// set to have non-duplicate elements 
vector< vector< set<int> > > nfa;
vector< vector<int> > dfa;
int initial_state;
vector<int> final_states;

// states = no of rows, inputCount = no of columns
int states = 0, inputCount = 0;
// map and reverse map to replace combined states like q0q1 to another named state like q4
map<set<int>, int> stateMap;
map<int, set<int>> stateMap2;
// queue to keep track of the states which are not yet processed but can be reached by the current state by transitions
queue<int> q;

int convertToInt(string s) {
	int num = 0;
	for (int i=0; i<s.size(); i++) {
		num = num*10 + (s[i]-'0');
	}
	return num;
}

void helper() {
    // initially no of states = no of rows in the nfa
    int n = states;
    // iterating through the nfa
    for (int i=0; i<states; i++) {
        for (int j=0; j<inputCount; j++) {
            // checking for absence of -1's in the set and checking for the absence of the set in the map
            if (!nfa[i][j].count(-1) && stateMap.find(nfa[i][j]) == stateMap.end()) {
                // making a new state(set) by merging the old states, but before doing this renaming the state (like q0q1 -> q4)
                stateMap[nfa[i][j]] = n;
                stateMap2[n] = nfa[i][j];
                // pushing the set in the queue for further processing
                q.push(n);
                // incrementing no of states
                n++;
            }
        }
    }

    // processing while the queue is not empty
    while (!q.empty()) {
        // no of elements in the queue at the current time to be processed
        int k = q.size();
        for (int i=0; i<k; i++) {
            // popping
            int curr = q.front();
            q.pop();

            vector<set<int>> temp;
            // running a loop through the no of cols for each element in the queue
            for (int j=0; j<inputCount; j++) {
                set<int> t;
                // finding out the states for which the current state is renamed to
                for (auto state : stateMap2[curr]) {
                    // if the state doesnt contain any -1's then insert the elements of the set in the new set
                    if (!nfa[state][j].count(-1)) {
                        t.insert(nfa[state][j].begin(), nfa[state][j].end());
                    }
                }
                // if there are elements in the new set and if the map doesnt contain the set then make a new state
                if (!t.empty() && stateMap.find(t) == stateMap.end()) {
                    stateMap[t] = n;
                    q.push(n);
                    stateMap2[n] = t;
                    n++;
                }
                // otherwise insert a -1
                if (t.empty()) {
                    t.insert(-1);
                }
                temp.push_back(t);
            }
            nfa.push_back(temp);
        }
    }
    // updating final states due to merging of states
    set<int> final(final_states.begin(), final_states.end());
    // for each state in the map
    for (auto it : stateMap2) {
        // check if it is a final state
        if (!final.count(it.first)) {
            // if it is not a final state then check if any of the states in the set is a final state
            for (auto i : it.second) {
                if (final.count(i)) {
                    // if any of the states in the set is a final state then insert the set in the final states
                    final.insert(it.first);
                    break;
                }
            }
        }
    }

    ofstream fout;
    fout.open("convertedDFA.txt");  

    fout<<initial_state<<endl;
    for (auto itr = final.begin(); itr != final.end(); itr++) {
        if (next(itr) == final.end())
            fout<<*itr;
        else
            fout<<*itr<<",";
    }
    fout<<endl;
    for (int i=0; i<n; i++) {
        for (int j=0; j<inputCount; j++) {
            if (stateMap.find(nfa[i][j]) != stateMap.end()) {
                fout<<stateMap[nfa[i][j]]<<" ";
            }
            else {
                fout<<"-1 ";
            }
        }
        fout<<endl;
    }
    fout.close();
}
 
int main() {
    ifstream fin;
    fin.open("nfa.txt");
	int curr_line = 0;
    string line;

    cout<<"\n20BCS021\nFAIZAN CHOUDHARY\n\n";
    cout<<"Given NFA: "<<endl;
    // read until EOF
 
    while (getline(fin, line)) {
    	cout<<line<<endl;
	}
	// clearing eof flags and seeking to start of file
	fin.clear();
	fin.seekg(0);
    while (getline(fin, line)) {
		int state;
		vector<set<int>> temp;
        if (curr_line == 0)
           initial_state = convertToInt(line);
        else if (curr_line == 1) {
   	 	    for (int i=0; i<line.size(); i++) {
   	 	    	if (line[i] != ',') {
   	 	    		final_states.push_back(convertToInt(line.substr(i, 1)));
				}
	        }	
		}
        else {
            string s;
			stringstream ss(line);
            
            while (getline(ss, s, ' ')) {
                string k;
                set<int> t;
                // cout<<s<<endl;
                for (int i=0; i<s.size(); i++) {
                    if (s[i] != ',')
                        k += s[i];
                    else {
                        t.insert(convertToInt(k));
                        k = "";
                    }
                }
                if (k != "-1")
                    t.insert(convertToInt(k));
                else
                    t.insert(-1);
                temp.push_back(t);
            }
            nfa.push_back(temp);
		}
        curr_line++;
    }
    // updating no of rows and cols
    states = nfa.size();
    inputCount = nfa[0].size();

    // inserting the state values in the two maps
    for (int i=0; i<states; i++) {
        stateMap.insert({{i}, i});
        stateMap2.insert({i, {i}});
    }

    helper();

    fin.close();

    fin.open("convertedDFA.txt");
    cout<<"\nConverted DFA: "<<endl;
    while (getline(fin, line)) {
    	cout<<line<<endl;
    }
    fin.close();
    return 0;
}