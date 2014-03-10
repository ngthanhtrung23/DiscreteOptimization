#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1011;

vector<int> ke[MAXN];
int n, m, color[MAXN];

struct Node {
    int deg, id;
} nodes[MAXN];

bool operator < (const Node &a, const Node &b) {
    return a.deg > b.deg;
}

int main(int argc, char** argv) {
    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n >> m;
    while (m--) {
        int u, v; fin >> u >> v;
        ++u; ++v;

        ke[u].push_back(v);
        ke[v].push_back(u);

        nodes[u].deg++;
        nodes[v].deg++;
    }
    sort(nodes+1, nodes+n+1);

    int nColor = 0;
    for(int u = 1; u <= n; ++u) {
        set<int> has;
        for(int i = 0; i < ke[u].size(); ++i) {
            int v = ke[u][i];
            if (color[v]) has.insert(color[v]);
        }
        color[u] = 1;
        while (has.find(color[u]) != has.end()) ++color[u];
        nColor = max(nColor, color[u]);
    }
    cout << nColor << ' ' << 0 << endl;
    for(int i = 1; i <= n; ++i) {
        cout << color[i] - 1 << ' ';
    }
    cout << endl;
}
