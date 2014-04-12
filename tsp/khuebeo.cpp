#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstring>
using namespace std;

#define INP "TORCH_1.IN"
#define ANS "TORCH_1.ANS"
#define MAXX 211
#define SOCON 25
#define MAXN 2011

#define FOR(i, a, b) for (int i = a; i <= b; i++)
#define FORD(i, a, b) for (int i = a; i >= b; i--)

struct ch {
	int a[MAXN];
	double len;
};

bool mycmp(ch a, ch b) {
	return a.len < b.len;
}

int n;
double x[MAXN], y[MAXN];
double dist[MAXN][MAXN];
ch dangTinh, myBest;

void read_data() {
	
	cin >> n;
	FOR(i, 1, n) cin >> x[i] >> y[i];
	FOR(i, 1, n) FOR(j, 1, n) {
		dist[i][j] = pow(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2), 0.5);
	}

}


void calc(ch &p) {
	p.len = 0;
	p.a[n + 1] = 1;
	FOR(i, 1, n) {
		p.len += dist[p.a[i]][p.a[i + 1]];
	}
}

ch make(ch p, int u, int v) {
	while (u < v) {
		swap(p.a[u], p.a[v]);
		u++;
		v--;
	}
	calc(p);
	return p;
}

void process1() {
	bool used[MAXN];
	memset(used, false, sizeof(used));
	used[1] = true;
	myBest.a[1] = 1;
	myBest.a[n + 1] = 1;
	FOR(pos, 2, n) {
		int chose = -1;
		int prev = myBest.a[pos - 1];
		FOR(i, 1, n) if (!used[i]) {
			if (chose == -1 || dist[prev][i] < dist[prev][chose]) chose = i;
		}
		myBest.a[pos] = chose;
		used[chose] = true;
	}
	calc(myBest);
	while (true) {
		bool stop = true;
		FOR(u, 2, n) FORD(v, n, u + 1) {
			double t1 = dist[myBest.a[u - 1]][myBest.a[u]] + dist[myBest.a[v]][myBest.a[v + 1]];
			double t2 = dist[myBest.a[u - 1]][myBest.a[v]] + dist[myBest.a[u]][myBest.a[v + 1]];
			if (t1 > t2) {
				myBest = make(myBest, u, v);
				stop = false;
				calc(myBest);
			}
		}
		if (stop) break;
	}
}

int turn = 0;
void process2() {
	for (int i = 1; i <= n; i++) dangTinh.a[i] = i;
	for (int j = 0; j < n * 10; j++) {
		int u = rand() % (n - 1) + 2;
		int v = rand() % (n - 1) + 2;
		swap(dangTinh.a[u], dangTinh.a[v]);
	}
	dangTinh.a[n + 1] = 1;
	calc(dangTinh);
	while (true) {
		bool stop = true;
		FOR(u, 2, n) FORD(v, n, u + 1) {
			double t1 = dist[dangTinh.a[u - 1]][dangTinh.a[u]] + dist[dangTinh.a[v]][dangTinh.a[v + 1]];
			double t2 = dist[dangTinh.a[u - 1]][dangTinh.a[v]] + dist[dangTinh.a[u]][dangTinh.a[v + 1]];
			if (t1 > t2) {
				dangTinh = make(dangTinh, u, v);
				stop = false;
				calc(dangTinh);
			}
		}
		if (stop) break;
	}	
	if (myBest.len > dangTinh.len) {
		myBest = dangTinh;
		cout.precision(5);
		cout << fixed << myBest.len << ' ' << 0 << endl;
		FOR(i, 1, n) cout << myBest.a[i] - 1 << " ";
		cout << endl;
		cerr << endl << myBest.len << endl;
		turn = 1;
	}
}


int main() {
	time_t start,end;
	time(&start);
	srand(0);
	read_data();
	myBest.len = 1e10;
	process1();
	cerr << myBest.len << endl;
	for (int i = 0; ; i++) {
		++turn;
		process2();
		time(&end);
		double dif = difftime (end,start);
		if (myBest.len < 30000) break;
  	}
	cout.precision(5);
	cout << fixed << myBest.len << ' ' << 0 << endl;
	FOR(i, 1, n) cout << myBest.a[i] - 1 << " ";
	cout << endl;
	return 0;
}
