#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

int32_t main(int argc, char* argv[]) {
	if(argc < 4) {
		std::cerr << "usage: ./combine [out_file] [m] [n] [list of args of size n]" << std::endl;
		return 1;
	}

	int n = std::stoi(argv[3]);
	int m = std::stoi(argv[2]);
	std::ofstream out(argv[1]);
	if(!out.good()) {
		std::cerr << "error opening out_file" << std::endl;
		return 1;
	}

	std::vector<std::vector<std::pair<int, double>>> predictions(m);

	for(int i=0; i<n; ++i) {
		std::ifstream now(argv[i+4]);
		if(!now.good()) {
			std::cerr << "error opening: " << argv[i+4] << std::endl;
			return 1;
		}
		std::cerr << "[debug] reading: " << argv[i+4] << "... ";

		for(int j=0; j<m; ++j) {
			int l;
			char x;
			double c;
			now >> l >> x >> c;
			predictions[i].push_back({l, c});
		}

		std::cerr << "done" << std::endl;
	}
	
	for(int j=0; j<m; ++j) {
		std::map<int, std::pair<int, double>> mp;
		for(int i=0; i<n; ++i) {
			auto [label, prob] = predictions[i][j];
			if(prob <= 0.5 && false)
				continue;
			double prev = mp[label].second;
			mp[label] = {mp[label].first+1, (prev==0.0?1.0:prev)*prob};
		}

		typedef std::tuple<int, int, double> state;

		std::vector<state> to_sort;
		for(auto [k, v] : mp) {
			auto [c, p] = v;
			to_sort.push_back({k, c, p});
		}

		std::sort(to_sort.begin(), to_sort.end(), 
			[](const state &l, const state &r){
				if(std::get<1>(l) == std::get<1>(r))
					return std::get<2>(l) > std::get<2>(r);
				return std::get<1>(l) > std::get<1>(r);
			}
		);
		
		if(to_sort.empty()) {
			out << -1 << ";" << 0 << std::endl;
			continue;
		}
		out << std::get<0>(to_sort[0]) << ";" << std::get<2>(to_sort[0])  << std::endl;
	}

	return 0;
}
