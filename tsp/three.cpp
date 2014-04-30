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
} current, optimal, tmp;

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
    const int BOUND1 = 2000;
    const int BOUND2 = 500;
    const int STEP = 1;
    const int START = 2;
    const int BACK = 10;
    // for(int i = 1; i <= n; ++i) {
    //     if (rand() % 10 == 0) {
    //         swap(current.id[i], current.id[rand() % (i+1) + 1]);
    //     }
    // }
    current.calculate();
    optimal = current;
    while (true) {
        cerr << '.';
        bool stop = true;
        for(int u = START; u <= n; u += STEP) {
            bool has = false;
            // From: u-1 --> u --> v --> v+1
            // To:   u-1 --> v --> u --> v+1
            for(int v = min(n-1, u + BOUND1); v > u; --v) {
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
                    has = true;
                }
            }
            cerr << u << endl;
            update(optimal, current);
            // From: u-1 --> u --> v --> v+1 --> w --> w+1
            // To:   u-1 --> v --> u --> w --> v+1 --> w+1
            for(int v = min(n-2, u + BOUND1); v > u; --v)
                for(int w = min(n-1, v + BOUND2); w > v; --w) {
                    double t1 = (a[current.id[u-1]] - a[current.id[u]]).len()
                            + (a[current.id[v]] - a[current.id[v+1]]).len()
                            + (a[current.id[w]] - a[current.id[w+1]]).len();
                    double t2 = (a[current.id[u-1]] - a[current.id[v]]).len()
                            + (a[current.id[u]] - a[current.id[w]]).len()
                            + (a[current.id[v+1]] - a[current.id[w+1]]).len();
                    if (t1 > t2) {
                        for(int i = u, j = v; i <= j; ++i, --j) {
                            swap(current.id[i], current.id[j]);
                        }
                        for(int i = v+1, j = w; i <= j; ++i, --j) {
                            swap(current.id[i], current.id[j]);
                        }
                        current.calculate();
                        cout << "case 2: " << current.len << endl;
                        stop = false;
                        has = true;
                    }
                }
            update(optimal, current);

            // From: u-1 --> u --> v --> v+1 --> w --> w+1
            // To:   u-1 --> w --> v+1 --> u --> v --> w+1
            for(int v = min(n-2, u + BOUND1); v > u; --v)
                for(int w = min(n-1, v + BOUND2); w > v; --w) {
                    double t1 = (a[current.id[u-1]] - a[current.id[u]]).len()
                            + (a[current.id[v]] - a[current.id[v+1]]).len()
                            + (a[current.id[w]] - a[current.id[w+1]]).len();
                    double t2 = (a[current.id[u-1]] - a[current.id[w]]).len()
                            + (a[current.id[u]] - a[current.id[v+1]]).len()
                            + (a[current.id[v]] - a[current.id[w+1]]).len();
                    if (t1 > t2) {
                        for(int x = u; x <= w+1; ++x)
                            tmp.id[x] = current.id[x];
                        int now = u-1;
                        for(int x = w; x >= v+1; --x)
                            current.id[++now] = tmp.id[x];
                        for(int x = u; x <= v; ++x)
                            current.id[++now] = tmp.id[x];

                        current.calculate();
                        cout << "case 3: " << current.len << endl;
                        stop = false;
                        has = true;
                    }
                }
            update(optimal, current);
            if (has) {
                u -= BACK;
            }
        }
        if (stop) break;
    }
}

void load() {
    fstream fin; fin.open(FILENAME, fstream :: in);
    int tmp;
    fin >> current.len >> tmp;
    for(int i = 1; i <= n; ++i) {
        fin >> current.id[i];
        ++current.id[i];
    }
    for(int i = 1; i <= 10; ++i) cout << current.id[i] << ' '; cout << endl;

    current.calculate();
}

int main(int argc, char** argv) {
    cerr << (fixed) << setprecision(3);

    fstream fin; fin.open(argv[1], fstream :: in);
    fin >> n;
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].x >> a[i].y;
    }
    load();
    // for(int i = 1; i <= n; ++i)
    //     current.id[i] = i;
    // current.calculate();
    cout << current.len << endl;
    optimal = current;
    cout << optimal.len << endl;
    optimize();
    ans();
}
