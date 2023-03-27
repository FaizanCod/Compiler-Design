#include "firstAndFollow.h"

using namespace std;

int main() {
    ifstream fin;
    fin.open("CFG4.txt");
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
        // productions[temp[0]].first = curr_line;
        productions[temp[0]].push_back(k);
        prodNo[temp[0] + k] = curr_line;
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
    fin.close();

    // inserting $ into terminals
    vector<char> NTs(NonTerminals.begin(), NonTerminals.end());
    vector<char> Ts(Terminals.begin(), Terminals.end());
    Ts.push_back('$');

    for (int i=0; i<NTs.size(); i++)
        mp[NTs[i]] = i;
    
    for (int i=0; i<Ts.size(); i++)
        mp[Ts[i]] = i;

    vector <vector<set<string>>> LL (NTs.size(), vector<set<string>> (Ts.size()));
    for (auto p: productions) {
        char NT = p.first;
        // int prodNo = p.second.first;
        for (string prod: p.second) {
            // string s(1, NT);
            // string k = s + "->" + prod;
            if (prod == "#" || !checkEpsilon(prod, NT)) {
                // if the whole string derives epsilon
                for (char t: follow[NT])
                    LL[mp[NT]][mp[t]].insert(prod);
            }
            // else {
                for (int i=0; i<prod.size(); i++) {
                    if (islower(prod[i])) {
                        LL[mp[NT]][mp[prod[i]]].insert(prod);
                        break;
                    }
                    // if first of prod[i] contains epsilon
                    if (find(first[prod[i]].begin(), first[prod[i]].end(), '#') != first[prod[i]].end()) {
                        for (char t: first[prod[i]]) {
                            if (t != '#')
                                LL[mp[NT]][mp[t]].insert(prod);
                        }
                    }
                    // no epsilon in first of prod[i]
                    else {
                        for (char t: first[prod[i]])
                            LL[mp[NT]][mp[t]].insert(prod);
                        break;
                    }
                }
            // }
        }
    }

    // LL table
    cout<<endl<<"LL Table:\n\n";
    cout<<"\t";
    for (char t: Ts) {
        cout<<t<<"\t";
    }
    cout<<endl;
    for (int i=0; i<NTs.size(); i++) {
        cout<<NTs[i]<<"\t";
        for (int j=0; j<Ts.size(); j++) {
            for (int k=0; k<LL[i][j].size(); k++) {
                cout<<prodNo[NTs[i] + *next(LL[i][j].begin(), k)];
                if (k != LL[i][j].size()-1)
                    cout<<",";
            }
            cout<<"\t";
        }
        cout<<endl<<endl;
    }

    return 0;
}