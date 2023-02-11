#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
 
using namespace std;
 
vector< vector<pair<int, char> > > moore;
int initial_state;
 
string check(string input) {
	string out;
	// int curr_state = initial_state;
	// cout<<"\nTransitions: ";
	pair<int, char> t = {initial_state, ' '};
	int i=0;
	cout<<"\nTransitions: ";
	while (i < input.size() && t.first != -1) {
		// curr_state = mealy[curr_state][input[i]-'0'];
		t = moore[t.first][input[i]-'0'];
		if (t.first != -1)
			cout<<"q"<<t.first<<" -> ";
		if (t.first == -1)
			break;
		else
			out += t.second;
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
    while (fin) {
		int state;
		char output;
		vector< pair<int, char> > temp;
        getline(fin, line);
        if (curr_line == 0)
           initial_state = line[0] - '0';
		// no final states in moore
        else {
        	for (int i=0; i<line.size(); ) {
   	 	    	if (line[i] != ' ') {
   	 	    		if (line[i] == '-') {
   	 	    		   	i++;
   	 	    		   	state = (-(line[i] - '0'));
						output = (' ');
						temp.push_back({state, output});
						// cout<<temp.back().first<<" "<<temp.back().second<<endl;
						i+=5;
	                }
	                else {
						state = (line[i]-'0');
						output = (line[i+2]);
						temp.push_back({state, output});
						// cout<<temp.back().first<<" "<<temp.back().second<<endl;
						i+=4;
					}
				}
	        }
			moore.push_back(temp);
			// cout<<"Hello: "<<mealy.back()[0].first<<" "<<mealy.back()[0].second<<" "<<mealy.back()[1].first<<" "<<mealy.back()[1].second<<endl;
		}
        curr_line++;
    }

    string input;
    cout<<"\nEnter input consisting of 0's and 1's: ";
    getline(cin, input);
 
    string ans = check(input);
	cout<<"Output of Moore machine: "<<ans<<endl; 		

    // for (int i=0; i<mealy.size(); i++) {
	// 	for (int j=0; j<mealy[0].size(); j++)
	// 		cout<<mealy[i][j].first<<" "<<mealy[i][j].second;
	// 	cout<<endl;
	// }
 
    fin.close();
    return 0;
}