#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main () {
    // open file
    ifstream fin;
    while (true) {
        cout << "Enter file name: ";
        string fname;
        cin >> fname;
        fin.open (fname);
        if (fin) {
            cout << "The file was opened successfully" << endl;
            break;
        }
        else { cout << "Couldn't open the file!" << endl; }
    }

    // read all words from file while keeping track on their count
    map <string, int> words;
    string word;
    char t = 0;
    do {
        if (isalpha (t)) { word += tolower (t); }
        else if (word.size () > 0) {
            words [word] ++;
            word.clear ();
        }
    }
    while (fin.get (t));
    fin.close ();

    // print all met words with their count
    for (auto i : words) {
        cout << i.first << ": " << i.second << endl;
    }
    return 0;
}