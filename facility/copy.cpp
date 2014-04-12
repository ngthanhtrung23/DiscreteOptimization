#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    fstream fin; fin.open(argv[1], fstream :: in);
    string tmp;
    while (getline(fin, tmp)) {
        cout << tmp << endl;
    }
    fin.close();
}

