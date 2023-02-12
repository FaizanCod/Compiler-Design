#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
 
using namespace std;
 
vector< vector<int> > dfa;
int initial_state;
vector<int> final_states;
 
//int stoi(string s) {
//	int ans = 0;
//	for (int i=s.size()-1; i>=0; i--) {
//		if (isdigit(s[i])) {
//			int k = s[i] - '0';
//			ans = (ans * 10) + k;
//		}
//	}
//	return ans;
//}

int convertToInt(string s) {
	int num = 0;
	for (int i=0; i<s.size(); i++) {
		num = num*10 + (s[i]-'0');
	}
	return num;
}
 
int find (vector<int> &arr, int key) {
	for (int i=0; i<arr.size(); i++)
		if (arr[i] == key)
		   return i;
    return -1;
}
 
string check(vector< vector<int> > &dfa, string input) {
	int curr_state = initial_state;
	int i=0;
	for (int i=0; i<input.size(); i++) {
		if (input[i]-'0' >= dfa[0].size())
			return "INVALID INPUT";
	}
	cout<<"\nTransitions: ";
	while (i < input.size() && curr_state != -1) {
		curr_state = dfa[curr_state][input[i]-'0'];
		if (curr_state != -1)
			cout<<"q"<<curr_state<<" -> ";
		else
			cout<<"Dead state ";
		i++;
	}
	cout<<endl<<endl;
	if (curr_state == -1)
	   return "NOT ACCEPTED: DEAD STATE";
	else if (find(final_states, curr_state) == -1) 
	   return "NOT ACCEPTED: NON FINAL STATE";
    return "ACCEPTED";
}
 
int main() {
    ifstream fin;
    fin.open("dfa.txt");
	int curr_line = 0, mx = 0;
    string line;
    cout<<"\n20BCS021\nFAIZAN CHOUDHARY\n\n";
    cout<<"Given DFA: "<<endl;
    // read until EOF
 
    while (getline(fin, line)) {
    	cout<<line<<endl;
	}
	// clearing eof flags and seeking to start of file
	fin.clear();
	fin.seekg(0);
    while (fin) {
        getline(fin, line);
        vector<int> temp;
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
        	for (int i=0; i<line.size(); i++) {
   	 	    	if (line[i] != ' ') {
   	 	    		if (line[i] == '-') {
   	 	    		   	i++;
   	 	    		   	temp.push_back(-(convertToInt(line.substr(i, 1))));
	                }
	                else
   	 	    			temp.push_back(convertToInt(line.substr(i, 1)));
				}
	        }
	        dfa.push_back(temp);
		}
//        cout<<line<<endl;
        curr_line++;
    }
    string input;
    cout<<"\nEnter input consisting of 0's and 1's: ";
    getline(cin, input);
	// if (input.size() == 0)
    string ans = check(dfa, input);
    cout<<ans<<endl;
//    for (int i=0; i<final_states.size(); i++)
//   		cout<<final_states[i]<<endl;
//   		
//    cout<<endl;
//    for (int i=0; i<dfa.size(); i++) {
//    	for (int j=0; j<dfa[0].size(); j++)
//    		cout<<dfa[i][j]<<" ";
//    	cout<<endl;
//	}
 
    fin.close();
    return 0;
}