#include "hashTable.h"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{

    // take input from 'compin'
    ifstream in("compin");

    if (!in)
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    // create output file 'compout'
    ofstream out("compout");

    // create a hash table
    HashTable<string, int> table(4096);

    // insert all the characters from 0-255 into the hash table
    for (int i = 0; i < 256; i++)
    {
        string s;
        s += (char)i;
        table.insert(s, i);
    }

    // The Ziv-Lempel Algorithm

    char ch;
    int code = 256; // the next code to be assigned
    string p;  // the longest prefix that has been found
                    // in the table so far

    while (!in.eof()) // eof returns true if next character
    {                 // is eof
        in.get(ch);   // get command reads next character
  
        if(in.eof()) break; // if the next character is eof
                            // break out of the loop

        p += ch; // append ch to p

        if (!table.find(p)) // if pc is not in the table
        {

            // assign new code only if it is possible
            if (code < 4095)
            {
                // assign code to p
                table.insert(p, code);
                // cout << p << " : " << code << endl;
                code++;
            }

            // output the code for p-1
            // remove the last character from p
            p.pop_back();
            // write the code for p to the output file
            out << table.getValue(p) << " ";

            // reset p to the last character
            p.clear();
            p += ch;
        }
    }

    // write the code for p to the output file
    out << table.getValue(p) << " ";

    // close the files
    in.close();
    out.close();
}
