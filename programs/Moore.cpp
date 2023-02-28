#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
 
using namespace std;
 
vector< vector<int> > moore;
int initial_state;
vector<string> stateOutputs;

int convertToInt(string s) {
	int num = 0;
	for (int i=0; i<s.size(); i++) {
		num = num*10 + (s[i]-'0');
	}
	return num;
}
 
string check(string input) {
	string out;
	int t = initial_state;
	int i=0;
	cout<<"\nTransitions: ";
	while (i <= input.size() && t != -1) {
		if (t != -1)
			cout<<"q"<<t<<" -> ";
		if (t == -1)
			break;
		// else
			out += stateOutputs[t];
		t = moore[t][input[i]-'0'];
		i++;
	}
	cout<<endl;
	return out;
}
 
int main() {
    ifstream fin;
    fin.open("moore.txt");
	int curr_line = 0;
    string line;

    cout<<"\n20BCS021\nFAIZAN CHOUDHARY\n\n";
    cout<<"Given Moore: "<<endl;
    // read until EOF
 
    while (getline(fin, line)) {
    	cout<<line<<endl;
	}
	// clearing eof flags and seeking to start of file
	fin.clear();
	fin.seekg(0);
    while (getline(fin, line)) {
		int state;
		string output;
		vector<int> temp;
        if (curr_line == 0)
           initial_state = convertToInt(line);
		// no final states in moore
        else {
			int i = 0;
			int j = line.size()-1;
            while (line[j] != ' ')
                output += line[j--];
			reverse(output.begin(), output.end());
			// cout<<"OUTPUT: "<<output<<endl;
			while (i < line.size()-output.size()) {
				string s;
				while (line[i] != ' ' && i < line.size())
					s += line[i++];
				if (s == "-1") {
					state = -1;
					temp.push_back(state);
                    i++;
                    continue;
				}
				else if (isdigit(s[0]))
					state = convertToInt(s);

				// cout<<"State: "<<state<<" "<<output<<endl;
				if (output != "")
				    temp.push_back(state);
					
				i++;
			}
			stateOutputs.push_back(output);
			moore.push_back(temp);
		}
        curr_line++;
    }

    string input;
    cout<<"\nEnter input consisting of 0's and 1's: ";
    getline(cin, input);

    string ans = check(input);
	cout<<"Output of Moore machine: "<<ans<<endl; 		

    // for (int i=0; i<moore.size(); i++) {
	// 	for (int j=0; j<moore[0].size(); j++)
	// 		cout<<moore[i][j]<<" "<<stateOutputs[i]<<"  ";
	// 	cout<<endl;
	// }
 
    fin.close();
    return 0;
}