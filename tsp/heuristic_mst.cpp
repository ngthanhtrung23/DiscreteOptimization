#include <bits/stdc++.h>
using namespace std;

const int MAXN = 40111;
const double SOLUTION = 437655;

struct Point {
    double x, y;

    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator - (Point a) { return Point(x-a.x, y-a.y); }

    double len() { return sqrt(x*x + y*y); }
} a[MAXN];

int n;

struct Result {
    double len;
    int id[MAXN];

    void calculate() {
        len = 0;
        for(int i = 1; i <= n; ++i) {
            int u = id[i], v = (i == n) ? id[1] : id[i+1];
            len += (a[u] - a[v]).len();
        }
    }
} current, optimal;

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

struct Edge {
    int u, v;
    double c;

    Edge() {}
    Edge(int u, int v, double c) : u(u), v(v), c(c) {}
} edges[MAXN * MAXN];
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
        for(int j = 1; j <= n; ++j) if (j != i) {
            addEdge(i, j, (a[i] - a[j]).len());
        }
        if (i % 500 == 0) cerr << i << endl;
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
    cerr << current.len << endl;
    update(optimal, current);
}

void optimize() {
    int mid = n / 2;
    for(int turn = 0; turn < 1000111000; ++turn) {
        if (rand() % 2 == 0) {
            for(int i = 1; i <= n; ++i)
                current.id[i] = optimal.id[i];

            int len = rand() % 15;
            int from1 = rand() % (mid-len) + 1;
            int to1 = from1 + len - 1;

            int from2 = rand() % (n - mid - len) + mid + 1;
            int to2 = from2 + len - 1;

            for(int x = from1, y = from2; x <= to1; ++x, ++y)
                swap(current.id[x], current.id[y]);
        }
        else {
            for(int i = 1; i <= n; ++i)
                current.id[i] = optimal.id[i];

            int len = rand() % 15;
            int from1 = rand() % (mid-len) + 1;
            int to1 = from1 + len;
            reverse(current.id + from1, current.id + to1);
        }

        current.calculate();

        update(optimal, current);
    }
}

int main(int argc, char** argv) {
    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n;
    cerr << (fixed) << setprecision(10);
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].x >> a[i].y;
    }
    for(int i = 1; i <= n; ++i) {
        optimal.id[i] = i;
    }
    optimal.calculate();
    solve();
    optimize();
    ans();
}
