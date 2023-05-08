#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "hashTable.h"

using namespace std;

int main()
{

    // take input from 'compout'

    ifstream in("compout");
    if (!in)
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    ofstream out("decompout");

    // the dictionary
    map<int, string> mp;
    // initialize the codes array
    for (int i = 0; i < 256; i++)
    {
        mp[i]+=i;
    }

    vector<int> codes;
    while (!in.eof())
    {
        int code;
        in >> code;
        if(in.eof()) break;
        codes.push_back(code);
    }

    // the first code is always a single character
    string text = mp[codes[0]]; // current text for the code , text(x)


    int prev_code = codes[0];
    int next_code = 256; // the next code to be assigned

    out << text;

    for (int i = 1; i < codes.size(); i++)
    {
        int code = codes[i];

        if (mp.find(code)!=mp.end())
        {                    
            string cur_text = mp[code]; // code found in the dictionary
            
            mp[next_code] =text + cur_text[0];
            text = cur_text;
        }
        else
        {
            text = mp[prev_code];
            text += text[0];
            mp[code] = text;
            
        }

        out << text; // write the text to the output file
        prev_code = code;
        next_code++;
        
    }
    // close the files
    in.close();
    out.close();

    return 0;
}