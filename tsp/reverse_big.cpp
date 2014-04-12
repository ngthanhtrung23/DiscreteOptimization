#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

const int MAXN = 34111;
const char* FILENAME = "output_6.txt";

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

bool operator < (const Result &a, const Result &b) {
    return a.len < b.len;
}

void ans() {
    cout << (fixed) << setprecision(3) << optimal.len << " 0" << endl;
    for(int i = 1; i <= n; ++i)
        cout << optimal.id[i] - 1 << ' ';
    cout << endl;
}

void save() {
    fstream fout; fout.open(FILENAME, fstream :: out);
    fout << (fixed) << setprecision(3) << optimal.len << " 0\n";
    for(int i = 1; i <= n; ++i) {
        fout << optimal.id[i] - 1 << ' ';
    }
    fout << endl;
}

void update(Result &optimal, const Result &current) {
    if (current.len >= optimal.len) return ;

    cerr << "\nUpdated to: " << current.len << endl;
    optimal.len = current.len;
    for(int i = 1; i <= n; ++i)
        optimal.id[i] = current.id[i];
    save();
}

void optimize() {
    while (true) {
        cerr << '.';
        bool stop = true;
        for(int u = 2; u <= n; ++u) {
            for(int v = n-1; v > u; --v) {
                double t1 = (a[current.id[u-1]] - a[current.id[u]]).len()
                        + (a[current.id[v]] - a[current.id[v+1]]).len();
                double t2 = (a[current.id[u-1]] - a[current.id[v]]).len()
                        + (a[current.id[u]] - a[current.id[v+1]]).len();
                if (t1 > t2) {
                    for(int i = u, j = v; i <= j; ++i, --j) {
                        swap(current.id[i], current.id[j]);
                    }
                    current.calculate();
                    stop = false;
                }
            }
            cerr << u << endl;
            update(optimal, current);
        }
        if (stop) break;
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
    current = optimal;
    optimize();
    ans();
}
