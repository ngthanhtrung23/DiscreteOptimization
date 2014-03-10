#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <algorithm>
#include <iomanip>
using namespace std;

const int MAXN = 10111;
const int CACHE_ROW = 20;
const int SOLUTION = 1099893;
const int MAXV = 10111000;

int n, cap;
map<int,int> f[MAXN], trace[MAXN], cacheF[MAXN], cacheTrace[MAXN];
bool mark[MAXN], isCached[MAXN];

double upperBound[MAXV];
int totalRemaining[MAXN];

struct Object {
    int weight, value, index;
} a[MAXN];

bool operator < (const Object &a, const Object &b) {
    return a.value*(long long) b.weight > a.weight*(long long) b.value;
}

void getRow(int n) {
    int start = 0;
    for(int i = n; i >= 0; --i)
        if (isCached[i]) {
            start = i;
            f[i] = cacheF[i];
            trace[i] = cacheTrace[i];
            break;
        }
    for(int i = start + 1; i <= n; ++i) {
        for(__typeof(f[i-1].begin()) it = f[i-1].begin(); it != f[i-1].end(); ++it) {
            int u = it->first;

            if (it->second + totalRemaining[i+1] >= SOLUTION) {
                if (!f[i].count(u) || f[i][u] < it->second) {
                    f[i][u] = it->second;
                    trace[i][u] = trace[i-1][u];
                }
            }

            // Add current object
            if (u + a[i].weight <= cap 
                    && it->second + (int) (upperBound[cap - u - a[i].weight] + 0.5)
                        + a[i].value >= SOLUTION
            ) {
                int totalValue = it->second + a[i].value;
                if (!f[i].count(u + a[i].weight) || f[i][u + a[i].weight] < totalValue) {
                    f[i][u + a[i].weight] = totalValue;
                    trace[i][u + a[i].weight] = i;
                }
            }
        }
        if (i % CACHE_ROW == 0) {
            isCached[i] = true;
            cacheF[i] = f[i];
            cacheTrace[i] = trace[i];
        }
        cerr << "Processed " << i << " rows. Expanded: " << f[i].size() << endl;
        f[i-1].clear(); trace[i-1].clear();
    }
}

void solve() {
    f[0][0] = 0;
    trace[0][0] = -1;
    isCached[0] = true;
    cacheF[0] = f[0];
    cacheTrace[0] = trace[0];

    getRow(n);

    int bestTotal = 0, save = -1, curObject = n;
    for(__typeof(f[n].begin()) it = f[n].begin(); it != f[n].end(); ++it)
        if (it->second > bestTotal) {
            bestTotal = it->second;
            save = it->first;
        }

    cout << bestTotal << ' ' << 1 << endl;
    while (curObject > 0) {
        getRow(curObject);
        int lastObject = trace[curObject][save];
        mark[a[lastObject].index] = true;

        save -= a[lastObject].weight;
        curObject = lastObject - 1;
    }

    for(int i = 1; i <= n; ++i)
        cout << ((mark[i]) ? 1 : 0) << ' ';
    cout << endl;
}

void init() {
    sort(a+1, a+n+1);
    int total = 0;
    for(int i = 1; i <= n; ++i)
        total += a[i].weight;
    if (total < cap) cap = total;

    for(int i = n; i >= 1; --i) {
        totalRemaining[i] = totalRemaining[i+1] + a[i].value;
    }
    int cur = 0;
    for(int i = 1; i <= n; ++i) {
        double each = a[i].value / (double) a[i].weight;
        int next = cur + a[i].weight;

        for(int x = cur+1; x <= next && x <= cap; ++x) {
            upperBound[x] = upperBound[x-1] + each;
        }
        if (next > cap) break;
        cur = next;
    }
}

int main(int argc, char** argv) {
    string filename = string(argv[1]);
    fstream fin; fin.open(filename.c_str(), fstream :: in);
    fin >> n >> cap;
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].value >> a[i].weight;
        a[i].index = i;
    }
    init();
    solve();
}
