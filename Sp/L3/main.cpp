#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <map>
#include <windows.h>

using namespace std;

enum type { NONE, STRING, COMMENT, PREPR, RESERVED, NUMBER, OPERATOR };
struct chunk {
    int start, end;
    mutable type type;
};
bool operator < (const chunk& a, const chunk& b) { return a.start < b.start; }

void parse (const string& str, const string& sreg, set < chunk >& chunks, type type) {
    regex exp = regex (sreg);
    smatch res;
    auto it = chunks.begin ();
    int start = 0, end;
    while (start < str.size ()) {
        while (it != chunks.end () && start == it -> start) {
            start = it -> end;
            it ++;
        }
        if (it == chunks.end ()) { end = str.size (); }
        else { end = it -> start; }

        while (regex_search (str.begin () + start, str.begin () + end, res, exp)) {
            int pos = res.suffix ().first - str.begin ();
            chunks.insert ((chunk) { pos - (int) res.str ().size (), pos, type });
            start = pos;
        }
        start = end;
    }
}

int main () {
    // open and read file
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
    string str ((istreambuf_iterator <char> (fin)), istreambuf_iterator <char> ());
    fin.close ();
    //cout << str << endl;

    set < chunk> chunks;
    parse (str, R"(\"(?:\\.|[^\"])*\"|\/\/[^\n]*|\/\*[\s\S]*?\*\/)", chunks, type::STRING);
    for (auto& it : chunks) {
        if (str [it.start] == '/') { it.type = type::COMMENT; }
    }
    parse (str, R"(^\s*#.*$)", chunks, type::PREPR);
    parse (str, R"(\b(?:auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)\b)", chunks, type::RESERVED);
    parse (str, R"(\b(?:0[bB][01]+|0[oO][0-7]+|0[xX][0-9a-fA-F]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)\b)", chunks, type::NUMBER);
    parse (str, R"(\b(?:\+|\-|\*|=|\!=|\<|\<=|\>|>=|\&|\||\^|\~|\<<|\>>|\&=|\|=|\^=|\?|\:|\,|\-\>)\b)", chunks, type::OPERATOR);
    
    map < int, pair < string, int > > types = {
        {type::NONE, {"TEXT", 7}},
        {type::STRING, {"STRING", 6}},
        {type::COMMENT, {"COMMENT", 2}},
        {type::PREPR, {"PREPROCESSOR", 8}},
        {type::RESERVED, {"RESERVED", 3}},
        {type::NUMBER, {"NUMBER", 5}},
        {type::OPERATOR, {"OPERATOR", 8}}
    };
    for (auto it : chunks) {
        cout << types [it.type].first << ": \t from " <<  it.start << " to " << it.end << endl;
    }
    cout << endl;

    auto it = chunks.begin ();
    int start = 0, end;
    HANDLE con = GetStdHandle (STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute (con, types [type::NONE].second);
    while (start < str.size ()) {
        while (it != chunks.end () && start == it -> start) {
            SetConsoleTextAttribute (con, types [it -> type].second);
            cout << string (str, start, it -> end - start);
            cout << flush;
            start = it -> end;
            it ++;
        }
        if (it == chunks.end ()) { end = str.size (); }
        else { end = it -> start; }
        SetConsoleTextAttribute (con, types [type::NONE].second);
        cout << string (str, start, end - start);
        cout << flush;
        start = end;
    }
    SetConsoleTextAttribute (con, types [type::NONE].second);
    cout << endl;
    return 0;
}
