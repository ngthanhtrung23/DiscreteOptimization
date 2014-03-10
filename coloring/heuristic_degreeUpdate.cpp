#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1011;

vector<int> ke[MAXN];
int n, m, color[MAXN];

struct Node {
    int deg, id;
} nodes[MAXN], remaining[MAXN];

bool operator < (const Node &a, const Node &b) {
    return a.deg > b.deg;
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

void read(char* filename) {
    fstream fin; fin.open(filename, fstream :: in);
    fin >> n >> m;
    for(int i = 1; i <= n; ++i) {
        nodes[i].id = i;
    }
    while (m--) {
        int u, v; fin >> u >> v;
        ++u; ++v;

        ke[u].push_back(v);
        ke[v].push_back(u);

        nodes[u].deg++;
        nodes[v].deg++;
    }
}

int main(int argc, char** argv) {
    read(argv[1]);
    for(int turn = 1; turn <= n; ++turn) {
        int cur = 0;
        for(int i = 1; i <= n; ++i)
            if (!color[nodes[i].id]) {
                remaining[++cur] = nodes[i];
            }
        sort(remaining+1, remaining+cur+1);
        int u = remaining[1].id;
        set<int> has;
        for(int i = 0; i < ke[u].size(); ++i) {
            int v = ke[u][i];
            if (color[v]) has.insert(color[v]);
        }
        color[u] = 1;
        while (has.find(color[u]) != has.end()) ++color[u];

        cout << u << ' ' << nodes[u].deg << ' ' << color[u] << endl;

        for(int i = 0; i < ke[u].size(); ++i) {
            int v = ke[u][i];
            nodes[v].deg--;
        }
    }
    ans();
}
