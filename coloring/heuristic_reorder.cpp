#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1011;

int n, color[MAXN];
vector<int> ke[MAXN];

struct Node {
    int id, color;
    int seed;
} nodes[MAXN];

bool operator < (const Node &a, const Node &b) {
    if (a.color != b.color) return a.color < b.color;
    return a.seed < b.seed;
}

void read(char* filename) {
    fstream fin; fin.open(filename, fstream :: in);
    int m;
    fin >> n >> m;
    while (m--) {
        int u, v;
        fin >> u >> v;
        ++u; ++v;

        ke[u].push_back(v);
        ke[v].push_back(u);
    }
}

void ans() {
    int nColor = 0;
    for(int i = 1; i <= n; ++i)
        nColor = max(nColor, color[i]);

    cout << nColor << ' ' << 0 << endl;
    for(int i = 1; i <= n; ++i) {
        cout << color[i] - 1 << ' ';
    }
    cout << endl;
}

void solve() {
    fstream fin; fin.open("output_6.txt", fstream :: in);
    int tmp; fin >> tmp >> tmp;
    for(int i = 1; i <= n; ++i) {
        nodes[i].id = i;
        fin >> color[i];
        ++color[i];
        nodes[i].color = color[i];
    }

    const int MAX_TURN = 10000;
    for(int turn = 0; turn < MAX_TURN; ++turn) {
        // Paint
        if (turn > 0) {
            memset(color, 0, sizeof color);
            for(int i = 1; i <= n; ++i) {
                int u = nodes[i].id;
                set<int> has;
                for(int x = 0; x < ke[u].size(); ++x) {
                    int v = ke[u][x];
                    if (color[v])
                        has.insert(color[v]);
                }
                color[u] = 1;
                while (has.find(color[u]) != has.end()) ++color[u];
                nodes[i].color = color[u];
            }
        }

        // Reorder
        for(int i = 1; i <= n; ++i)
            nodes[i].seed = rand() % (n*n);
        sort(nodes+1, nodes+n+1);
        if (turn % (MAX_TURN / 100) == 0)
            cerr << turn << ' ' << nodes[n].color << endl;
    }
    ans();
}

int main(int argc, char** argv) {
    read(argv[1]);
    solve();
}
