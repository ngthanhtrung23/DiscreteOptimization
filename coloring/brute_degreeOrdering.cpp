#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1011;
const int SOLUTION = 17;

vector<int> ke[MAXN];
int n, m, color[MAXN], nColor;
int bestColor[MAXN], bestNColor;
bool adj[MAXN][MAXN];

int deg[MAXN];

void ans() {
    cout << bestNColor << ' ' << 0 << endl;
    for(int i = 1; i <= n; ++i) {
        cout << bestColor[i] - 1 << ' ';
    }
    cout << endl;
}

void read(char* filename) {
    fstream fin; fin.open(filename, fstream :: in);
    fin >> n >> m;
    while (m--) {
        int u, v; fin >> u >> v;
        ++u; ++v;

        adj[u][v] = true;

        ke[u].push_back(v);
        ke[v].push_back(u);

        deg[u]++;
        deg[v]++;
    }
}

void update() {
    if (nColor >= bestNColor) return ;
    bestNColor = nColor;
    for(int i = 1; i <= n; ++i)
        bestColor[i] = color[i];

    cerr << bestNColor << endl;
    if (bestNColor == SOLUTION) {
        ans();
        exit(0);
    }
}

void paint(int u, int c) {
    color[u] = c;
    if (c) {
        for(int i = 0; i < ke[u].size(); ++i) {
            int v = ke[u][i];
            deg[v]--;
        }
    }
    else {
        for(int i = 0; i < ke[u].size(); ++i) {
            int v = ke[u][i];
            deg[v]++;
        }
    }
}

int getColor(int u) {
    set<int> has;
    for(int i = 0; i < ke[u].size(); ++i) {
        int v = ke[u][i];
        if (color[v]) has.insert(color[v]);
    }
    int res = 1;
    while (has.find(res) != has.end()) ++res;
    return res;
}

void attempt(int turn) {
    cerr << turn << endl;
    if (nColor >= bestNColor) return ;
    if (nColor > SOLUTION) return ;

    if (turn > n) {
        update();
        return ;
    }
    int cur = 0;
    set< pair<int,int> > s;
    for(int i = 1; i <= n; ++i)
        if (!color[i]) {
            set<int> has;
            for(int x = 0; x < ke[i].size(); ++x) {
                int j = ke[i][x];
                if (color[j]) has.insert(color[j]);
                if (has.size() == SOLUTION) return ;
            }

            s.insert(make_pair(-deg[i], i));
            if (s.size() > SOLUTION) {
                __typeof(s.begin()) it = s.end(); --it;
                s.erase(it);
            }
        }

    int MAX_SIZE = 2, cnt = 0;
    for(__typeof(s.begin()) it = s.begin(); it != s.end(); ++it) {
        ++cnt; if (cnt > MAX_SIZE) break;
        int u = it->second;

        paint(u, getColor(u));
        int oldVal = nColor;
        nColor = max(nColor, color[u]);

        attempt(turn+1);
        nColor = oldVal;
        paint(u, 0);
    }
}

int main(int argc, char** argv) {
    read(argv[1]);
    bestNColor = n;
    for(int i = 1; i <= n; ++i)
        bestColor[i] = i;
    attempt(1);
    ans();
}
