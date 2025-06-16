#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

int32_t main(int argc, char* argv[]) {
	if(argc < 4) {
		std::cerr << "usage: ./combine [out_file] [m] [n] [list of args of size n] [optional: accepting th.]" << std::endl;
		return 1;
	}

	int n = std::stoi(argv[3]);
	int m = std::stoi(argv[2]);
	std::ofstream out(argv[1]);
	if(!out.good()) {
		std::cerr << "error opening out_file" << std::endl;
		return 1;
	}

	double threshold = 0.5;
	if(argc > n+4)
		threshold = std::stod(argv[n+4]);
	std::cerr << "threshold: " << threshold << '\n';

	std::vector<std::vector<std::pair<int, double>>> predictions(n);

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
		int ok = 0;
		for(int i=0; i<n; ++i) {
			auto [label, prob] = predictions[i][j];
			if(label == 1)
				++ok;
		}
		double rt = double(ok)/double(n);
		int pred = (rt>threshold?1:0);
		out << pred << ";" << 1-pred-rt << std::endl;
	}

	return 0;
}
