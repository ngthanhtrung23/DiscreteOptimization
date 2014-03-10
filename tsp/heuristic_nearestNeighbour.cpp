#include <bits/stdc++.h>
using namespace std;

const int MAXN = 40111;

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
    if (current.len > optimal.len) return ;
    optimal.len = current.len;
    for(int i = 1; i <= n; ++i)
        optimal.id[i] = current.id[i];
}

bool used[MAXN];

void solve() {
    memset(used, false, sizeof used);
    used[7] = true;
    current.id[1] = 7;

    for(int i = 2; i <= n; ++i) {
        double bestDist = 1e6;
        int save = -1;
        for(int j = 1; j <= n; ++j) {
            double curDist = (a[i-1] - a[j]).len();
            if (!used[j] && curDist < bestDist) {
                bestDist = curDist;
                save = j;
            }
        }
        current.id[i] = save;
        used[save] = true;
    }
    current.calculate();
    update(optimal, current);
}

int main(int argc, char** argv) {
    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n;
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].x >> a[i].y;
    }
    for(int i = 1; i <= n; ++i) {
        optimal.id[i] = i;
    }
    optimal.calculate();
    solve();
    ans();
}
