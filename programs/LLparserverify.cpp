#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;

set <char> NonTerminals;
set <char> Terminals;
map <char, vector<string>> productions;
map <int, string> prodNo;
map <char, int> mp;
char start;

int main() {
    ifstream fin;
    fin.open("CFG1.txt");
	int curr_line = 0;
    string line;

    cout<<"\n20BCS021\nFAIZAN CHOUDHARY\n\n";
    cout<<"Given CFG: "<<endl;
    // read until EOF
    while (getline(fin, line)) {
        cout<<curr_line<<" "<<line<<endl;
        char s;
        stringstream ss(line);
        vector<char> temp;
        int col = 0;
        while (ss >> s) {
            if (isupper(s)) {
                if (curr_line == 0 && col == 0)
                    start = s;
                NonTerminals.insert(s);
                // first NT of first production is start symbol and its follow is $
            }
            else {
                Terminals.insert(s);
                // first of the terminal is the terminal itself
            }
            col++;
            temp.push_back(s);
        }
        // adding production to map
        string k;
        for (int i=1; i<temp.size(); i++) {
            k += temp[i];
        }
        // productions[temp[0]].first = curr_line;
        productions[temp[0]].push_back(k);
        prodNo[curr_line] = k;
        curr_line++;
    }

    fin.close();

    vector<char> NTs(NonTerminals.begin(), NonTerminals.end());
    vector<char> Ts(Terminals.begin(), Terminals.end());
    Ts.push_back('$');

    fin.open("LLtable.txt");
    vector<vector<string>> table (NTs.size(), vector<string> (Ts.size(), " "));
    curr_line = 0;
    int c = 0;
    cout<<"\n\nGiven LL(1) parsing table: "<<endl<<endl;
    // read until EOF
    while (getline(fin, line)) {
        cout<<line<<endl;
        if (curr_line == 0) {
            string s;
            stringstream ss(line);
            while (ss >> s) {
                mp[s[0]] = c++;
            }
            c = 0;
        }
        else {
            int counter = 0, prevC = 0;
            mp[line[0]] = c++;
            char NT = line[0];
            for (int i=1; i<line.size(); i++) {
                if (line[i] != '\t') {
                    // cout<<line[i]<<endl;
                    string k;
                    while (line[i] != '\t')
                        k += line[i++];
                    if (counter % 2 == 0) {
                        if (prevC != counter)
                            // there exists an entry for the cell given by line[i] and col (counter/2)-1 row is mp[NT]
                            table[mp[NT]][(counter/2)-1] = k;
                    }
                }
                
                else {
                    prevC = counter;
                    counter++;
                }
                // cout<<counter<<endl;
            }
        }
        curr_line++;
    }

    // for (auto c: mp)
    //     cout<<c.first<<" "<<c.second<<endl;

    // for (int i=0; i<NTs.size(); i++) {
    //     for (int j=0; j<Ts.size(); j++)
    //         cout<<table[i][j]<<" ";
    //     cout<<endl;
    // }
    string check; 
    cout<<"\nEnter string to check: ";
    cin>>check;
    check += '$';
    stack <char> st;
    st.push('$');
    st.push(start);
    int i = 0;
    cout<<"\nTop of Stack\tInput String\tProduction applied\n";
    while (st.top() != '$') {
        char top = st.top();
        st.pop();
        // cout<<top<<endl;
        if (top == check[i]) {
            i++;
        }
        else if (isupper(top)) {
            int r = mp[top];
            int c = mp[check[i]];
            // cout<<r<<" "<<c<<endl;
            // cout<<table[r][c]<<endl;
            if (table[r][c] == " ") {
                cout<<top<<"\t\t"<<check.substr(i)<<endl;
                cout<<"\nString is not accepted!"<<endl;
                return 0;
            }
            string prod = prodNo[stoi(table[r][c])];
            if (prod[0] == check[i] || prod == "#")
                cout<<top<<"\t\t"<<check.substr(i)<<"\t\t"<<top<<"->"<<prod<<endl;
            else
                cout<<top<<"\t\t"<<check.substr(i)<<endl;

            if (prod != "#") {
                for (int i=prod.size()-1; i>=0; i--)
                    st.push(prod[i]);
            }
        }
        else {
            cout<<top<<"\t\t"<<check.substr(i)<<endl;
            cout<<"\nString not accepted!"<<endl;
            return 0;
        }
    }
    cout<<st.top()<<"\t\t"<<check.substr(i)<<endl;
    if (st.top() == '$' && check[i] == '$')
        cout<<"\nString is accepted"<<endl;
    else
        cout<<"\nString is not accepted!"<<endl;
    fin.close();

    return 0;
}