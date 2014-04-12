#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1011;
const int NCOLOR = 88;

int n, color[MAXN];
vector<int> ke[MAXN];
int blocked[MAXN][NCOLOR + 1], deg[MAXN], possibleColor[MAXN];
bool isAdj[MAXN][MAXN];

int cntUsed[NCOLOR + 1], weight[MAXN + 1];

const int NRANDOM = 0;
pair<int,int> randomConstraint[1000111];

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

        isAdj[u][v] = isAdj[v][u] = true;
        ++deg[u]; ++deg[v];
    }

    for(int i = 1; i <= n; ++i) {
        possibleColor[i] = NCOLOR;
    }

    for(int i = 1; i <= NRANDOM; ++i) {
        randomConstraint[i] = make_pair(rand() % n + 1, rand() % n + 1);
    }

    while (true) {
        bool hasBad = false;
        for(int i = 1; i <= NRANDOM; ++i)
            for(int j = 1; j <= NRANDOM; ++j)
                for(int k = 1; k <= NRANDOM; ++k) {
                    if (randomConstraint[i].second == randomConstraint[j].first
                            && randomConstraint[j].second == randomConstraint[k].first
                            && randomConstraint[k].second == randomConstraint[i].first) {
                        hasBad = true;
                        randomConstraint[k] = make_pair(rand() % n + 1, rand() % n + 1);
                    }
                }
        if (!hasBad) break;
    }
    for(int i = 1; i <= NRANDOM; ++i) {
        cerr << randomConstraint[i].first << ' ' << randomConstraint[i].second << endl;
    }
}

void propagate(int u, int c, int what) {
    if (what) {
        color[u] = c;
        cntUsed[c]++;
    }
    else {
        color[u] = 0;
        cntUsed[c]--;
    }

    for(int i = 0; i < ke[u].size(); ++i) {
        int v = ke[u][i];
        if (what) {
            blocked[v][c]++;
            --deg[v];

            if (blocked[v][c] == 1) {
                possibleColor[v]--;
            }
        }
        else {
            blocked[v][c]--;
            ++deg[v];

            if (blocked[v][c] == 0) {
                possibleColor[v]++;
            }
        }
    }

    for(int i = 1; i <= NRANDOM; ++i) {
        int x = randomConstraint[i].first, y = randomConstraint[i].second;
        // color[x] <= color[y]
        if (x == u) {
            for(int yc = 1; yc < c; ++yc)
                blocked[y][yc] = true;
        }
        else if (y == u) {
            for(int xc = c + 1; xc <= NCOLOR; ++xc)
                blocked[x][xc] = true;
        }
    }
}

int bestTurn = 0;

struct Color {
    int x;
};

bool operator < (const Color &a, const Color &b) {
    return cntUsed[a.x] < cntUsed[b.x];
}

void attempt(int turn) {
    if (turn > n) {
        cout << NCOLOR << ' ' << 0 << endl;
        for(int i = 1; i <= n; ++i) {
            cout << color[i] - 1 << ' ';
        }
        cout << endl;
        exit(0);
    }
    if (turn > bestTurn) {
        bestTurn = turn;
        cerr << bestTurn << endl;
    }
    int best = 0, bestDeg = -1, save = -1, bestWeight = 0;
    for(int i = 1; i <= n; ++i)
        if (!color[i]) {
            int sum = possibleColor[i];
            // if (deg[i] > bestDeg || (deg[i] == bestDeg && sum < best)) {
            // if (sum < best || (sum == best && deg[i] > bestDeg)) {
            // if (sum < best || (sum == best && bestWeight < weight[i])) {
            if (sum < best) {
                best = sum;
                bestWeight = weight[i];
                save = i;
            }
        }
    save = turn;
    if (save < 0) {
        for(int i = 1; i <= n; ++i) {
            if (!color[i]) ++weight[i];
        }
        return ;
    }

    for(int c = 1; c <= NCOLOR; ++c)
    if (!blocked[save][c])
        if (c == 1 || cntUsed[c] || cntUsed[c-1])
        // if (c == 1 || cntUsed[c] <= cntUsed[c-1] + 1)
        {
            propagate(save, c, 1);
            attempt(turn + 1);
            propagate(save, c, 0);
            if (cntUsed[c] == 0) break;
        }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    read(argv[1]);
    attempt(1);
}
