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

const bool __test_cases = false;
 
const int INF = 1e9+2137;

mt19937_64 rng;

struct word {
	int l, n;
	vt<int> a;
};
const ll prime = 1019; const ll mod = 1'000'000'007;
ll hs(const word &w) {
	ll res = 0;

	res += ((w.l+1)*prime)%mod;
	res += ((w.n+1)*prime)%mod;

	for(auto &x : w.a)
		res += ((x+1)*prime)%mod;

	return res;
}

int rand(int l, int r) {
	return uniform_int_distribution<int>(l, r)(rng);
}

int n;
vt<word> words;
map<ll, vt<int>> hstoi;
int split(int color, vt<int> &vis, double ratio) {
	int ile = 0;
	while(double(ile)/double(n) < ratio) {
		int idx = rand(0, n-1);
		ll h = hs(words[idx]);
		if(vis[idx] != 0 || sz(hstoi[h]) > double(n)*double(ratio)/4.0)
			continue;
		for(auto i : hstoi[h]) {
			vis[i] = color;
			++ile;
		}
	}
	return ile;
}

int32_t main(int argc, char* argv[]) {
	
	if(argc < 5) {
		cerr << "usage: ./split [input_file] [output_file] [seed] [split_ratio]\n";
		cerr << "where split is the size of the test data in range [0-1]\n";
		return 1;
	}
	
	string in_file_name = argv[1];
	string out_file_name = argv[2];
	rng = mt19937_64(stoi(argv[3]));

	ifstream input(in_file_name);
	if(!input.good()) {
		cerr << "error opening input file: " << in_file_name << '\n';
		return 1;
	}

	ofstream train(out_file_name+".train"), test(out_file_name+".test"), valid(out_file_name+".valid");
	if(!train.good() || !test.good()) {
		cerr << "error opening output file: " << out_file_name << '\n';
		return 1;
	}

	double test_ratio = stod(argv[4]);
	double valid_ratio = -1;
	if(argc >= 6)
		valid_ratio = stod(argv[5]);
	n = 0;
	int c = 0;
	input >> n >> c;

	cout << "[debug] " << n << ' ' << c << '\n';

	words.resize(n);
	FOR(i, 0, n) {
		word w;
		input >> w.l >> w.n;
		w.a.resize(w.n);
		FOR(j, 0, w.n)
			input >> w.a[j];
		words[i] = w;
		hstoi[hs(words[i])].pb(i);
	}

	vt<int> vis(n, 0);

	int test_cnt = split(1, vis, test_ratio);
	int valid_cnt = split(2, vis, valid_ratio);

	train << n-test_cnt-valid_cnt << ' ' << c << endl;
	test << test_cnt << ' ' << c << endl;
	valid << valid_cnt << ' ' << c << endl;
	
	int gte = 0, gtr = 0, gval = 0;
	FOR(i, 0, n) {
		if(vis[i] == 0) {
			train << words[i].l << ' ' << words[i].n << ' ';
			for(auto x : words[i].a)
				train << x << ' ';
			train << endl;
			++gtr;
		}
		else if(vis[i] == 1) {
			test << words[i].l << ' ' << words[i].n << ' ';
			for(auto x : words[i].a)
				test << x << ' ';
			test << endl;
			++gte;
		}
		else if(vis[i] == 2) {
			valid << words[i].l << ' ' << words[i].n << ' ';
			for(auto x : words[i].a)
				valid << x << ' ';
			valid << endl;
			++gval;
		}
	}

	cout << "[debug] " << gte << ' ' << gtr << ' ' << gval << ' ' << double(gte)/double(gtr+gte) << ' ' << double(gte)/double(gtr+gte) << '\n';

	return 0;
}
