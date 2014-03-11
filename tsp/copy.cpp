#include <bits/stdc++.h>
using namespace std;

int main() {
    fstream fin; fin.open("output_6.txt", fstream :: in);
    string tmp;
    while (getline(fin, tmp)) {
        cout << tmp << endl;
    }
    fin.close();
}

