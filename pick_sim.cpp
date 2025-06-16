#include<iostream>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<format>
#include<cassert>
#include<memory>

using namespace std;

bool PRINT_SHELL = false;

struct Filter {
	Filter() {}
	virtual string print() = 0;
};

struct FilterRA : Filter {
	bool rev;
	int k;
	FilterRA(bool r, int k) { rev = r; this->k = k; }
	string print() {
		return format("ra {} {}", rev?"true":"false", k);
	}
};

struct FilterRSS : Filter {
	int i, j;
	FilterRSS(int i, int j) { this->i = i; this->j = j; }
	string print() {
		return format("rss {} {}", i, j);
	}
};

int get_id(string x) {
	int id = 0;
		
	bool skipped = false;
	for(auto c : x) {
		if(c == '/')
			skipped = true;
		if(skipped && '0' <= c && c <= '9') {
			id *= 10;
			id += c-'0';
		}
	}

	cerr << "[debug] " << x << " -> " << id << '\n';
	return id;
}

inline void print_walk(ifstream &walk, ofstream &out, int n) {
	for(int i=0; i<n; ++i) {
		int x;
		walk >> x;
		out << x << ' ';
		int m; 
		walk >> m;
		out << m << ' ';
		for(int j=0; j<m; ++j) {
			int x;
			walk >> x;
			out << x << ' ';
		}
		out << endl;
	}
}

void flexfringe_predict(const string &comb_path, const string &model) {
	auto cmd = format("./build/flexfringe --ini ini/edsm.ini {} --mode predict --aptafile {} --predicttype true > /dev/null 2>&1", comb_path, model);
	if(PRINT_SHELL)
		cerr << "[debug] calling: " << cmd << '\n';
	int result = system(cmd.c_str());
	assert(result == 0);
}

void grep_labels(const string &comb_path, const string &model) {
	auto cmd = format("mlr --fs ';' --icsvlite --ocsv cut -f ' predicted trace type' {}.result | tail -n +2 > {}.res", model, comb_path);
	if(PRINT_SHELL)
		cerr << "[debug] calling: " << cmd << '\n';
	int result = system(cmd.c_str());
	assert(result == 0);
}

string copy_transform(const string &comb_path, const unique_ptr<Filter> &trans1, const string &dir, int idx) {
	string config_path = comb_path + ".conf";
	ofstream config(config_path);
	config << 1 << endl;
	config << trans1->print() << endl;
	config.close();
	auto cmd = format("./preproc {} {} {} 2> /dev/null", comb_path, config_path, dir);
	if(PRINT_SHELL)
		cerr << "[debug] calling: " << cmd << '\n';
	int result = system(cmd.c_str());
	assert(result == 0);

	string target = comb_path + "." + to_string(idx);
	cmd = format("mv {} {}", dir + "/model_00.dat", target);
	if(PRINT_SHELL)
		cerr << "[debug] calling: " << cmd << '\n';
	result = system(cmd.c_str());
	assert(result == 0);

	return target;
}

int32_t main(int argc, char* argv[]) {
	if(argc < 7) {
		cerr << "usage: pre_pick.txt, to_pick threshold comb_path config_path trash_path" << endl;
		return 1;
	}

	ifstream pre_pick(argv[1]);
	int to_pick = stoi(argv[2]);
	double threshold = stod(argv[3]);
	string comb_path = argv[4];
	string config_path = argv[5];
	string trash_path = argv[6];

	ifstream config(config_path);

	int config_n;
	config >> config_n;
	vector<unique_ptr<Filter>> filters(config_n);
	for(int i=0; i<config_n; ++i) {
		string mode;
		config >> mode;
		if(mode == "ra") {
			string revs, ks;
			config >> revs >> ks;
			bool rev = ((revs == "true") ? true : false);
			filters[i] = make_unique<FilterRA>(rev, stoi(ks));
		}
		else if(mode == "rss") {
			string is, js;
			config >> is >> js;
			filters[i] = make_unique<FilterRSS>(stoi(is), stoi(js));
		}
		else {
			cerr << "unknown mode\n";
			config.close();
			pre_pick.close();
			exit(1);
		}
	}

	config.close();

	vector<pair<string, int>> picked;
	auto max_sim = [&](string name, int id2) {
		double res = 0.0;

		for(auto [md, id1] : picked) {
			// cout << md << '\n';
			
			// combine both walks
			ifstream walk1(md+".walk"), walk2(name+".walk");
			if(!walk1.good()) {
				cerr << "problem with: " << md+".walk";
				exit(1);
			}

			if(!walk2.good()) {
				cerr << "problem with: " << name+".walk";
				exit(1);
			}

			ofstream combined(comb_path);
			int n1, n2, a1, a2;
			walk1 >> n1 >> a1;
			walk2 >> n2 >> a2;

			combined << n1+n2 << ' ' << max(a1, a2) << '\n';
			print_walk(walk1, combined, n1);
			print_walk(walk2, combined, n2);
			walk1.close(); walk2.close();

			// transform
			auto c1_path = copy_transform(comb_path, filters[id1], trash_path, 1);
			auto c2_path = copy_transform(comb_path, filters[id2], trash_path, 2);

			// compute predictions for both

			flexfringe_predict(c1_path, md);
			flexfringe_predict(c2_path, name);

			// get only labels
			vector<int> res1(n1+n2), res2(n1+n2);

			grep_labels(comb_path, md);

			ifstream input(comb_path+string(".res"));
			for(int i=0; i<n1+n2; ++i)
				input >> res1[i];
			input.close();

			grep_labels(comb_path, name);

			input = ifstream(comb_path+string(".res"));
			for(int i=0; i<n1+n2; ++i)
				input >> res2[i];
			input.close();

			// check sim

			int same = 0;
			for(int i=0; i<n1+n2; ++i)
				if(res1[i] == res2[i])
					++same;

			double sim = double(same)/double(n1+n2);
			cerr << md << ' ' << name << ": " << sim << endl;
			res = max(res, sim);
			if(res >= threshold)
				return res;
		}

		return res;
	};
	
	string line;
	while(picked.size() < to_pick && getline(pre_pick, line)) {
		// cout << "::: " << line << '\n';
		int start_idx = 0;
		string tmp = "";
		while(line[start_idx] != ';')
			tmp.push_back(line[start_idx]), ++start_idx;
		double acc = stod(tmp);
		string model_name = line.substr(start_idx+1, line.size()+10);
		int id = get_id(model_name);
		model_name.erase(model_name.size()-4);
		cerr << model_name << " -> acc: " << acc << endl;
		
		// make sure that the model is sound
		if(acc < 0.55)
			continue;
		
		auto mx_sim = max_sim(model_name, id);
		if(mx_sim < threshold) {
			cerr << "added: " << model_name << ", " << mx_sim << ' ' << threshold << endl;
			picked.push_back({model_name, id});
		}
	}

	cerr << "printing answer: " << endl;
	for(auto model : picked)
		cout << model.first << endl;

	pre_pick.close();
	return 0;
}
