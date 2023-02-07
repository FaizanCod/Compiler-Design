#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
 
using namespace std;
 
vector< vector<pair<int, char> > > mealy;
int initial_state;
 
int find (vector<int> &arr, int key) {
	for (int i=0; i<arr.size(); i++)
		if (arr[i] == key)
		   return i;
    return -1;
}

void reverse(string &s) {
    int i = 0, j = s.size()-1;
    while (i<j) {
        char k = s[i];
        s[i] = s[j];
        s[j] = k;
        j--;
        i++;
    }
}
 
string check(string input) {
	// int curr_state = initial_state;
    string out;
	pair<int, char> t = {initial_state, ' '};
	int i=0;
	while (i < input.size() && t.first != -1) {
		// curr_state = mealy[curr_state][input[i]-'0'];
		t = mealy[t.first][input[i]-'0'];
		if (t.first == -1)
			break;
		else
			out += t.second;
		i++;
	}
	return out;
}
 
int main() {
    ifstream fin;
    fin.open("2sComplement.txt");
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
    while (fin) {
		int state;
		char output;
		vector< pair<int, char> > temp;
        getline(fin, line);
        if (curr_line == 0)
           initial_state = line[0] - '0';
		// no final states in mealy
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
			mealy.push_back(temp);
			// cout<<"Hello: "<<mealy.back()[0].first<<" "<<mealy.back()[0].second<<" "<<mealy.back()[1].first<<" "<<mealy.back()[1].second<<endl;
		}
//        cout<<line<<endl;
        curr_line++;
    }


    string input;
    cout<<"\nEnter binary number to convert to 2's complement: ";
    getline(cin, input);
    // RTL for 2's complement
    reverse(input);
    string ans = check(input);
    reverse(ans);
    cout<<"2\'s complement: "<<ans<<endl;
//    for (int i=0; i<final_states.size(); i++)
//   		cout<<final_states[i]<<endl;
//   		
//    cout<<endl;
    // for (int i=0; i<mealy.size(); i++) {
	// 	for (int j=0; j<mealy[0].size(); j++)
	// 		cout<<mealy[i][j].first<<" "<<mealy[i][j].second;
	// 	cout<<endl;
	// }
 
    fin.close();
    return 0;
}