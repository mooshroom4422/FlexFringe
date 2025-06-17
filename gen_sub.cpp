#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

#define sz(x) (int)(x).size()

int32_t main(int argc, char* argv[]) {
	if(argc != 6) {
		std::cerr << "usage: ./gen_sub [input_path] [out_path] [mode] [limit] [seed]" << std::endl;
		std::cerr << "to used random_device, provide seed=0" << std::endl;
		return 1;
	}

	std::cerr << "[debug] input: " << argv[1] << std::endl;
	std::cerr << "[debug] output: " << argv[2] << std::endl;

	std::ifstream file(argv[1]);
	std::ofstream out(argv[2]);

	std::string mode = argv[3];
	int limit = std::stoi(argv[4]);
	int seed = std::stoi(argv[5]);

	if(!file.good() || !out.good()) {
		std::cerr << "error openning the file" << std::endl;
		file.close();
		out.close();
		return 1;
	}

	int n, m;
	file >> n >> m;
	
	int longest = 0;
	for(int i=0; i<n; ++i) {
		std::string label, m;
		file >> label >> m;
		for(int j=0; j<std::stoi(m); ++j) {
			std::string x;
			file >> x;
		}
		if(longest < std::stoi(m))
			longest = std::stoi(m);
	}

	std::cerr << "[debug] longest trace: " << longest << ", limit: " << limit << std::endl;
	
	std::vector<std::pair<std::string, int>> res;
	std::cerr << "[debug] mode={" << mode << "}" << std::endl;
	if(mode == "ra") {
		for(int i=0; i<longest; ++i) {
			res.push_back({"true", i});
			res.push_back({"false", i});
		}
	}
	else {
		for(int i=0; i<longest-1; ++i)
			for(int j=i+1; j<longest; ++j) {
				res.push_back({std::to_string(i), j});
			}
	}
	std::cerr << "[debug] finished gen gen_sub" << std::endl;

	std::random_device rd;
	std::mt19937 rng(rd());
	if(seed != 0)
		rng = std::mt19937(seed);
	std::shuffle(res.begin(), res.end(), rng);

	if(mode == "rss") {
		res[0] = {std::to_string(0), 0}; // make sure that the prefixtree is always included in the set
	}

	std::cerr << "[debug] finished pick gen_sub" << std::endl;

	while((int)res.size() < limit) {
		res.push_back(res[std::uniform_int_distribution<int>(0, res.size()-1)(rng)]);
	}
	
	int iter = 0;
	// out << std::min((int)res.size(), limit) << '\n';
	out << limit << '\n';
	for(auto [i, j] : res) {
		out << mode << ' ' << i << ' ' << j << std::endl;
		++iter;
		if(iter >= limit)
			break;
	}

	std::cerr << "[debug] finished gen_sub" << std::endl;

	return 0;
}
