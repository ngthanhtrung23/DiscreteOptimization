#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <algorithm>
using namespace std;

const int MAXN = 10111;
const int CACHE_ROW = 20;

int n, cap;
map<int,int> f[MAXN], trace[MAXN], cacheF[MAXN], cacheTrace[MAXN];
bool mark[MAXN], isCached[MAXN];

struct Object {
    int weight, value, index;
} a[MAXN];

bool operator < (const Object &a, const Object &b) {
    if (a.weight != b.weight) return a.weight > b.weight;
    if (a.value != b.value) return a.value > b.value;
    return a.index < b.index;
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

            if (!f[i].count(u) || f[i][u] < it->second) {
                f[i][u] = it->second; trace[i][u] = trace[i-1][u];
            }

            // Add current object
            if (u + a[i].weight <= cap) {
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

    sort(a+1, a+n+1);
    reverse(a+1, a+n+1);
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

int main(int argc, char** argv) {
    string filename = string(argv[1]);
    fstream fin; fin.open(filename.c_str(), fstream :: in);
    fin >> n >> cap;
    for(int i = 1; i <= n; ++i) {
        fin >> a[i].value >> a[i].weight;
        a[i].index = i;
    }
    solve();
}
