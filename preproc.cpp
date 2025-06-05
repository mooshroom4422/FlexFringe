#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define sz(x) (int)(x).size()

struct trace {
	int l, n;
	std::vector<std::string> labels;
};

int n, m;
std::string out_file;
std::vector<trace> traces;

std::string conv(int id) {
	if(id < 10)
		return "0" + std::to_string(id);
	return std::to_string(id);
}

void cut_ps(bool reverse, int cut_pref, int cut_suf, int id) {
	std::string out_name = out_file+"/model_"+conv(id)+".dat";
	std::ofstream file(out_name);
	std::cerr << "[debug] out_ps\n";
	std::cerr << "[debug] output file: " << out_name << std::endl;
	if(!file.good()) {
		std::cerr << "error openning the file" << std::endl;
		exit(1);
	}

	file << n << ' ' << m << std::endl;
	for(trace tr : traces) {
		if(reverse)
			std::reverse(tr.labels.begin(), tr.labels.end());

		file << tr.l << ' ' << tr.n-cut_pref-cut_suf << ' ';
		for(int i=cut_pref; i<sz(tr.labels)-cut_suf; ++i)
			file << tr.labels[i] << ' ';
		file << std::endl;
	}
}

void reverse_after(bool reverse, int k, int id) {
	std::string out_name = out_file+"/model_"+conv(id)+".dat";
	std::ofstream file(out_name);
	std::cerr << "[debug] out_ra\n";
	std::cerr << "[debug] output file: " << out_name << std::endl;
	if(!file.good()) {
		std::cerr << "error openning the file" << std::endl;
		exit(1);
	}

	file << n << ' ' << m << std::endl;

	for(auto tr : traces) {
		if(reverse)
			std::reverse(tr.labels.begin(), tr.labels.end());

		file << tr.l << ' ' << tr.n << ' ';
		for(int i=0; i<std::min(sz(tr.labels), k); ++i)
			file << tr.labels[i] << ' ';
		for(int i=sz(tr.labels)-1; i>=k; --i)
			file << tr.labels[i] << ' ';
		file << std::endl;
	}
}

void reverse_substring(int l, int r, int id) {
	std::string out_name = out_file+"/model_"+conv(id)+".dat";
	std::ofstream file(out_name);
	std::cerr << "[debug] out_rss\n";
	std::cerr << "[debug] output file: " << out_name << std::endl;
	if(!file.good()) {
		std::cerr << "error openning the file" << std::endl;
		exit(1);
	}

	file << n << ' ' << m << std::endl;


	for(auto tr : traces) {
		std::cerr << "[debug] original sequence:\n";
		std::cerr << tr.l << ' ' << tr.n << ": ";
		for(auto x : tr.labels)
			std::cerr << x << ' ';
		std::cerr << '\n';

		std::cerr << "[debug] modified sequence, l=" << l << ", r=" << r << ":\n";
		std::cerr << tr.l << ' ' << tr.n << ": ";

		file << tr.l << ' ' << tr.n << ' ';
		for(int i=0; i<std::min(sz(tr.labels), l); ++i) {
			file << tr.labels[i] << ' ';
			std::cerr << tr.labels[i] << ' ';
		}
		for(int i=std::min(r, sz(tr.labels)-1); i>=l; --i) {
			file << tr.labels[i] << ' ';
			std::cerr << tr.labels[i] << ' ';
		}
		for(int i=r+1; i<sz(tr.labels); ++i) {
			file << tr.labels[i] << ' ';
			std::cerr << tr.labels[i] << ' ';
		}
		file << std::endl;

		std::cerr << '\n';
	}
}

int32_t main(int argc, char* argv[]) {
	if(argc != 4) {
		std::cerr << "usage: ./preproc [input_path] [config_path] [out_path]" << std::endl;
		return 1;
	}

	std::cerr << "[debug] input: " << argv[1] << std::endl;
	std::cerr << "[debug] config: " << argv[2] << std::endl;
	std::cerr << "[debug] output: " << argv[3] << std::endl;

	std::ifstream file(argv[1]), config(argv[2]);
	out_file = argv[3];

	if(!file.good() || !config.good()) {
		std::cerr << "error openning the file" << std::endl;
		file.close();
		config.close();
		return 1;
	}

	file >> n >> m;

	for(int i=0; i<n; ++i) {
		std::string label, m;
		file >> label >> m;
		std::vector<std::string> a(std::stoi(m));
		for(int j=0; j<std::stoi(m); ++j) {
			std::string x;
			file >> x;
			a[j] = x;
		}
		traces.push_back(trace { std::stoi(label), std::stoi(m), a } );
	}

	int n_models;
	config >> n_models;

	for(int i=0; i<n_models; ++i) {
		std::string mode;
		config >> mode;
		if(mode == "cps") {
			std::string reverse, pref, suf;
			config >> reverse >> pref >> suf;
			bool rev = ((reverse == "true") ? true : false);
			std::cout << "[cps] reading: " << reverse << ' ' << pref << ' ' << suf << "... ";
			cut_ps(rev, std::stoi(pref), std::stoi(suf), i);
			std::cout << "done\n";
		}
		else if(mode == "ra") {
			std::string reverse, k;
			config >> reverse >> k;
			bool rev = ((reverse == "true") ? true : false);
			std::cout << "[ra] reading: " << reverse << ' ' << k << "... ";
			reverse_after(rev, std::stoi(k), i);
			std::cout << "done\n";
		}
		else if(mode == "rss") {
			std::string l, r;
			config >> l >> r;
			std::cout << "[rss] reading: " << l << ' ' << r << "... ";
			reverse_substring(std::stoi(l), std::stoi(r), i);
			std::cout << "done\n";
		}
	}

	return 0;
}
