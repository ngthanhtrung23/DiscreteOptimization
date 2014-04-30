#include <bits/stdc++.h>

#define FOR(i,a,b) for(int i=(a),_b=(b); i <= _b; ++i)
#define REP(i,a) for(int i=0,_a=(a); i < _a; ++i)
#define DEBUG(x) { cout << #x << " = " << x << endl; }
using namespace std;

const char* SOLUTION_FILE = "output_4_backup.txt";
const int MAXN = 611;
const double START_TEMP = 1000;
const double COOL_DOWN = 0.997;
const int NTURN = 1000;

int NLOCAL = 100000;

struct Point {
    double x, y;
    Point() { x = y = 0; }
    Point(double x, double y) : x(x), y(y) {}

    Point operator - (Point a) { return Point(x-a.x, y-a.y); }
};

int n;
Point a[MAXN];
double cost[MAXN][MAXN];

void read(string s) {
    fstream fin; fin.open(s.c_str(), fstream :: in);
    fin >> n;
    REP(i,n) fin >> a[i].x >> a[i].y;
    fin.close();

    REP(i,n) REP(j,n) {
        double dx = a[i].x - a[j].x;
        double dy = a[i].y - a[j].y;
        cost[i][j] = sqrt(dx*dx + dy*dy);
    }
}

struct Solution {
    int id[MAXN];
    double len;

    void initRandom() {
        REP(i,n) id[i] = i;
        REP(i,n) {
            swap(id[i], id[rand()%(i+1)]);
        }
        calculate();
    }

    void calculate() {
        len = 0;
        REP(i,n) {
            int j = (i == n-1) ? 0 : i+1;
            len += cost[id[i]][id[j]];
        }
    }

    void print() {
        cout << (fixed) << setprecision(4) << len << ' ' << 0 << endl;
        REP(i,n) cout << id[i] << ' ';
        cout << endl;
    }

    void load() {
        fstream fin; fin.open(SOLUTION_FILE, fstream :: in);
        fin >> len;
        int tmp; fin >> tmp;
        REP(i,n) fin >> id[i];
        fin.close();
    }

    void move_2opt(int u, int v) {
        for(int x = u+1, y = v; x < y; ++x, --y)
            swap(id[x], id[y]);
        calculate();
    }

    void localSearch(double t) {
        REP(turn,NLOCAL) {
            int u = rand() % (n-1);
            int v = rand() % (n-1);
            if (u == v) continue;

            double delta = (cost[id[u]][id[u+1]] + cost[id[v]][id[v+1]])
                    - (cost[id[u]][id[v]] + cost[id[u+1]][id[v+1]]);

            if (delta > 0) {
                move_2opt(u, v);
            }
            else {
                if ((double)rand() / RAND_MAX < exp(delta / t)) {
                    move_2opt(u, v);
                }
            }
        }
    }
} sol, best;

void solve() {
    sol.load();
    best = sol;
    best.len = 1e20;
    double t = START_TEMP;
    cout << (fixed) << setprecision(4);
    REP(turn,NTURN) {
        NLOCAL = (turn + 1) * (turn + 1) * 2;
        sol.localSearch(t);
        if (sol.len < best.len) {
            best = sol;
        }
        cout << sol.len << endl;
        t = t * COOL_DOWN;
    }
    DEBUG(t);
    best.print();
}

int main(int argc, char** argv) {
    srand(time(NULL));
    switch (argv[1][0]) {
        case '1':
            read("./data/tsp_51_1");
            break;
        case '2':
            read("./data/tsp_100_3");
            break;
        case '3':
            read("./data/tsp_200_2");
            break;
        case '4':
            read("./data/tsp_574_1");
            break;
        case '5':
            read("./data/tsp_1889_1");
            break;
        case '6':
            read("./data/tsp_33810_1");
            break;
    }
    solve();
}
