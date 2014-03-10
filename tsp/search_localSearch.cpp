#include <bits/stdc++.h>
using namespace std;

const int MAXN = 40111;
const double SOLUTION = 23433;

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

    cerr << "Updated to: " << current.len << endl;
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

void optimize() {
    int mid = n / 2;
    for(int turn = 0; turn < 1000111; ++turn) {
        for(int i = 1; i <= n; ++i)
            current.id[i] = optimal.id[i];

        int len = 5;
        int from1 = rand() % (mid-len) + 1;
        int to1 = from1 + len - 1;

        int from2 = rand() % (n - mid - len) + mid + 1;
        int to2 = from2 + len - 1;

        for(int x = from1, y = from2; x <= to1; ++x, ++y)
            swap(current.id[x], current.id[y]);

        current.calculate();

        update(optimal, current);
    }
}

int main(int argc, char** argv) {
    cerr << (fixed) << setprecision(3);

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
    optimize();
    ans();
}
