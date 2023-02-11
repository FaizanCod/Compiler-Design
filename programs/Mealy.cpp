#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
 
using namespace std;
 
vector< vector<pair<int, string> > > mealy;
int initial_state;

int convertToInt(string s) {
	int num = 0;
	for (int i=0; i<s.size(); i++) {
		num = num*10 + (s[i]-'0');
	}
	return num;
}
 
string check(string input) {
	string out;
	pair<int, string> t = {initial_state, " "};
	int i=0;
	cout<<"\nTransitions: ";
	while (i < input.size() && t.first != -1) {
		if (t.first != -1)
			cout<<"q"<<t.first<<" -> ";
		if (t.first == -1)
			break;
		else
			out += t.second;
		t = mealy[t.first][input[i]-'0'];
		i++;
	}
	if (t.first != -1)
		cout<<"q"<<t.first<<" -> ";
	out += t.second;
	cout<<endl;
	return out;
}
 
int main() {
    ifstream fin;
    fin.open("mealy3.txt");
	int curr_line = 0;
    string line;

    cout<<"\n20BCS021\nFAIZAN CHOUDHARY\n\n";
    cout<<"Given Mealy: "<<endl;
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
		vector< pair<int, string> > temp;
        if (curr_line == 0)
           initial_state = convertToInt(line);
		// no final states in mealy
        else {
			int i = 0;
			while (i < line.size()) {
				string s;
				while (line[i] != ' ' && i < line.size())
					s += line[i++];
				if (s == "-1") {
					state = -1;
					output = " ";
					i+=2;
				}
				else {
					if (isdigit(s[0]))
						state = convertToInt(s);
					else
						output = s;
				}
				// cout<<"State: "<<state<<" "<<output<<endl;
				if (output != "")
					temp.push_back({state, output});
				i++;
			}
			mealy.push_back(temp);
		}
        curr_line++;
    }

    string input;
    cout<<"\nEnter input consisting of 0's and 1's: ";
    getline(cin, input);

    string ans = check(input);
	cout<<"Output of Mealy machine: "<<ans<<endl; 		

    fin.close();
    return 0;
}