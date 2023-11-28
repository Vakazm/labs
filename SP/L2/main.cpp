#include <iostream>
#include <fstream>
#include <list>

using namespace std;

// transition structure
struct transition {
    int from;
    int to;
    char c;
};


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
    
    // read data
    int alfsize, statesize, first, fstatenum;
    fin >> alfsize >> statesize >> first >> fstatenum;
    int final [fstatenum];
    for (int i = 0; i < fstatenum; i ++) { fin >> final [i]; }
    list < transition > transitions;
    transition t;
    while (fin >> t.from >> t.c >> t.to) {
        transitions.push_back (t);
    }
    fin.close ();

    // perform BFS on graph, setting 0 if unreached, 1 if deadend, and 2 if reached
    char states [statesize] = {0};
    list < int > queue;
    queue.push_back (first);
    while (!queue.empty ()) {
        int state = queue.front ();
        queue.pop_front ();
        if (states [state] != 0) { continue; }
        states [state] = 1;
        for (auto it = transitions.begin (); it != transitions.end (); it ++) {
            if (it -> from == state) {
                states [state] = 2;
                queue.push_back (it -> to);
                it = transitions.erase (it);
                it --;
            }
        }
    }

    // print lists of unreachable and deadend states
    cout << "Unreachable states: ";
    for (int i = 0; i < statesize; i ++) {
        if (states [i] == 0) { cout << i << ' ';}
    }
    cout << endl;
    cout << "Deadend states: ";
    for (int i = 0; i < statesize; i ++) {
        if (states [i] == 1) {
            bool found = false;
            for (int j = 0; j < fstatenum && !found; j ++) {
                if (final [j] == i) { found = true; }
            }
            if (!found) { cout << i << ' '; }
        }
    }
    cout << endl;
    return 0;
}
