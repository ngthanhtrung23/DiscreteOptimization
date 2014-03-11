#include <bits/stdc++.h>
using namespace std;

const int MAXN = 111;
const double SOLUTION = 21930;
const int NPARENT = 10000;
const int NCHILD = 50;

struct Point {
    double x, y;

    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator - (Point a) { return Point(x-a.x, y-a.y); }

    double len() { return sqrt(x*x + y*y); }
} a[MAXN];

int n;
double dist[MAXN][MAXN];

struct Result {
    double len;
    int id[MAXN];

    void calculate() {
        len = 0;
        for(int i = 1; i <= n; ++i) {
            int u = id[i], v = (i == n) ? id[1] : id[i+1];
            len += dist[u][v];
        }
    }
} current, optimal, child[NPARENT * NCHILD + 11], parent[NPARENT + 11];

bool operator < (const Result &a, const Result &b) {
    return a.len < b.len;
}

void ans() {
    cout << (fixed) << setprecision(3) << optimal.len << " 0" << endl;
    for(int i = 1; i <= n; ++i)
        cout << optimal.id[i] - 1 << ' ';
    cout << endl;
}

void update(Result &optimal, const Result &current) {
    if (current.len >= optimal.len - 1e-9) return ;
    cerr << "Update to: " << current.len << endl;
    optimal.len = current.len;
    for(int i = 1; i <= n; ++i)
        optimal.id[i] = current.id[i];
    if (optimal.len < SOLUTION) {
        ans();
        exit(0);
    }
}

int lab[MAXN];
int getRoot(int u) {
    return (lab[u] < 0) ? u : lab[u] = getRoot(lab[u]);
}

void merge(int u, int v) {
    if (lab[u] > lab[v]) swap(u, v);
    int x = lab[u] + lab[v];
    lab[u] = x;
    lab[v] = u;
}

const int MAX_NEIGHBOUR = 500;

struct Edge {
    int u, v;
    double c;

    Edge() {}
    Edge(int u, int v, double c) : u(u), v(v), c(c) {}
} edges[MAXN * MAX_NEIGHBOUR];
bool operator < (const Edge &a, const Edge &b) {
    return a.c < b.c;
}
int nEdge;

void addEdge(int u, int v, double c) {
    edges[++nEdge] = Edge(u, v, c);
}

vector<int> tree[MAXN];
int pos;

void dfs(int u, int fu) {
    ++pos;
    current.id[pos] = u;

    for(int i = 0; i < tree[u].size(); ++i) {
        int v = tree[u][i];
        if (v == fu) continue;
        dfs(v, u);
    }
}

void solve() {
    memset(lab, -1, sizeof lab);
    for(int i = 1; i <= n; ++i) {
        set< pair<double,int> > s;
        for(int j = 1; j <= n; ++j) if (j != i) {
            s.insert(make_pair(dist[i][j], j));
            if (s.size() > MAX_NEIGHBOUR) {
                __typeof(s.end()) it = s.end(); --it;
                s.erase(it);
            }
        }

        for(__typeof(s.begin()) it = s.begin(); it != s.end(); ++it)
            addEdge(i, it->second, it->first);
    }
    cerr << "Done init graph\n";
    sort(edges+1, edges+nEdge+1);

    for(int i = 1; i <= nEdge; ++i) {
        int u = edges[i].u, v = edges[i].v;
        if (getRoot(u) == getRoot(v)) continue;

        merge(getRoot(u), getRoot(v));

        tree[u].push_back(v); tree[v].push_back(u);
    }
    cerr << "Done mst\n";

    dfs(1, -1);
    current.calculate();
    update(optimal, current);
}

void optimize() {
    int nParent = 1;
    for(int i = 1; i <= n; ++i)
        parent[1].id[i] = optimal.id[i];
    parent[1].calculate();

    for(int turn = 0; turn < 1000111000; ++turn) {
        // cerr << "Start gen child\n";
        int nChild = 0;
        int mid = n / 2;
        for(int parentId = 1; parentId <= nParent; ++parentId) {
            for(int born = 1; born <= NCHILD; ++born) {
                ++nChild;
                // cerr << "Turn = " << turn << " " << parentId << " " << born << " " << nChild << endl;
                for(int i = 1; i <= n; ++i)
                    child[nChild].id[i] = parent[parentId].id[i];
                if (born > 1) {
                    int dice = rand() % 4;
                    if (dice == 0) {
                        int len = rand() % (mid / 2);
                        int from1 = rand() % (mid-len) + 1;
                        int to1 = from1 + len - 1;

                        int from2 = rand() % (n - mid - len) + mid + 1;
                        int to2 = from2 + len - 1;

                        for(int x = from1, y = from2; x <= to1; ++x, ++y)
                            swap(child[nChild].id[x], child[nChild].id[y]);
                    }
                    else if (dice == 1) {
                        int len = rand() % 15 + 1;
                        int from1 = rand() % (mid-len) + 1;
                        int to1 = from1 + len;
                        reverse(child[nChild].id + from1, child[nChild].id + to1);
                    }
                    else if (dice == 2) {
                        int x = rand() % n + 1;
                        int y = rand() % n + 1;
                        swap(child[nChild].id[x], child[nChild].id[y]);
                    }
                    else {
                        int len = rand() % 15 + 1;
                        int from1 = rand() % (mid-len) + 1;
                        int to1 = from1 + len;
                        int save[20];
                        for(int i = from1; i <= to1; ++i) {
                            save[i - from1] = child[nChild].id[i];
                        }
                        int start = from1 + rand() % len;
                        for(int i = from1; i <= to1; ++i) {
                            child[nChild].id[i] = save[start - from1];
                            ++start; if (start > to1) start = from1;
                        }
                    }
                }
                child[nChild].calculate();
            }
        }
        // cerr << "Done gen child\n";
        sort(child+1, child+nChild+1);
        nParent = min(NPARENT, nChild);
        for(int i = 1; i <= nParent; ++i) {
            parent[i].len = child[i].len;
            for(int x = 1; x <= n; ++x)
                parent[i].id[x] = child[i].id[x];
        }
        // cerr << "Done creating parent\n";

        update(optimal, parent[1]);
    }
}

int main(int argc, char** argv) {
    srand(7777);
    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n;
    cerr << (fixed) << setprecision(10);
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].x >> a[i].y;
    }
    for(int i = 1; i <= n; ++i) {
        for(int j = 1; j <= n; ++j) {
            dist[i][j] = (a[i] - a[j]).len();
        }
    }
    for(int i = 1; i <= n; ++i) {
        optimal.id[i] = i;
    }
    optimal.calculate();
    solve();
    optimize();
    ans();
}
