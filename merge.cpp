#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
 
#define mp make_pair
#define pb push_back
#define ppb pop_back
#define pf push_front
#define ppf pop_front
#define eb emplace_back
#define st first
#define nd second
#define vt vector
#define VAR(__var)  #__var << ": " << __var << " "
#define PAIR(__var) #__var << ": " << __var.first << ", " << __var.second << " "
#define FOR(__var, __start, __end)  for(int __var=__start; __var<__end; ++__var)
#define FORB(__var, __start, __end) for(int __var=__start; __var>__end; --__var)
#define REP(__var, __cnt) for(int __var=0; __var<(__cnt); ++__var)
#define EACH(__var, __x) for(auto __var : __x)
#define maxi(__x, __y) __x = (__x>__y?__x:__y)
#define mini(__x, __y) __x = (__x<__y?__x:__y)
#define all(__var)     (__var).begin(),(__var).end()
#define rall(__var)    (__var).rbegin(),(__var).rend()
#define sz(__var)      (int)(__var).size()

using namespace std;
 
using namespace __gnu_pbds;
template <typename T>
using ord_set = tree<T,null_type,less<T>,rb_tree_tag,tree_order_statistics_node_update>;

#ifdef DEBUG
template<typename Stream, typename T1, typename T2>
Stream& operator << (Stream& out, pair<T1, T2> a) {return out << "(" << a.st << ", " << a.nd << ")";}
 
template<typename Stream, typename T>
Stream &operator << (Stream& out, T v) {
	out << "{";
	int i = 0;
	for (auto x : v)
		out << x << ((++ i) != sz(v) ? ", " : "");
	return out << "}";
}
 
template<typename... Args>
void dump(Args... x) {((cerr << x << ", "), ...) << '\n';}
 
#define debug(x...) cerr << "[" #x "]: ", dump(x)
#else
#define debug(...) 0
#endif
 
typedef       long long ll;
typedef     long double ld;
typedef   pair<ll, ll> pll;
typedef pair<int, int> pii;

int32_t main(int argc, char* argv[]) {
	
	ifstream test(argv[1]), valid(argv[2]);
	ofstream out(argv[3]);

	int n, m, c1, c2;
	test >> n >> c1;
	valid >> m >> c2;
	out << n+m << ' ' << max(c1,c2) << '\n';
	FOR(i, 0, n) {
		int l, k;
		test >> l >> k;
		out << l << ' ' << k << ' ';
		FOR(j, 0, k) {
			int x;
			test >> x;
			out << x << ' ';
		}
		out << '\n';
	}

	FOR(i, 0, m) {
		int l, k;
		valid >> l >> k;
		out << l << ' ' << k << ' ';
		FOR(j, 0, k) {
			int x;
			valid >> x;
			out << x << ' ';
		}
		out << '\n';
	}

	return 0;
}
