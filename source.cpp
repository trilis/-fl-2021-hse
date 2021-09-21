//#define _GLIBCXX_DEBUG
// #pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,popcnt")
// #pragma GCC target("avx,avx2")
#pragma GCC optimize("Ofast,unroll-loops")


#include <bits/stdc++.h>
// #include "optimization.h"
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned int uint;

#define all(a) (a).begin(), (a).end()
#define forn(i, n) for (int i = 0; i < (ll)(n); i++)

random_device dev;
mt19937 rng(dev());
ll mod = 1e9 + 7;
uniform_int_distribution<ll> uid(1, ll(1e5));

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;  
    }
};


void timeS(const char *s = 0) {
    static double start = 0;
    fprintf(stderr, "%.2f : %s\n", 1. * (clock() - start) / CLOCKS_PER_SEC, s ? s : "");
    start = clock();
}


void solve() {
    int n;
    cin >> n;

    assert(n && n <= 1024);
    regex r("^((1(00)*1)|10((00)*1(00)*1)*(00)*(10(00)*1|01)|0*)*");
    
    forn(i, n) {
        
        bool ok = regex_match(bitset<10>(i).to_string(), r);
        if (bool(i % 3) == ok) {
            cout << "FAILED TEST # " << i << " " << bitset<10>(i) << "\n"; 
        }
    }
}


int main() {
    ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int t = 1;
	//cin >> t;
	//t = readInt();
    while (t--) {
		solve();
	}
	return 0;
}
