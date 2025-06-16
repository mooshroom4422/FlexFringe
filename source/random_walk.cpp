#include "random_walk.h"

#include <random>

#include "input/inputdatalocator.h"

void perform_random_walk(state_merger* m, std::ofstream& output) {
    std::cerr << "starting random walk" << std::endl;

    std::random_device rd;
    std::mt19937 rng(rd());
    if (WALK_SEED != -1)
        rng = std::mt19937(WALK_SEED);

    output << NUM_WALKS << ' ' << inputdata_locator::get()->get_alphabet_size() << std::endl;
    for (int i=0; i<NUM_WALKS; ++i) {
        std::vector<std::string> labels;
        std::string type = "asd";

        apta_node* now = m->get_aut()->get_root();
        while (true) {
            // following the formula from stamina paper, sec. 3.2.1
            auto tmp = now->guards_start();
            int outdeg = 0;
            while (tmp != now->guards_end())
                ++outdeg, ++tmp;
            auto draw = std::uniform_int_distribution<int>(1, 1+WALK_FACTOR*outdeg)(rng);
            if ((draw == 1 || outdeg == 0) && "1" == inputdata_locator::get()->string_from_type(now->get_data()->predict_type(nullptr))) {
				type = "1";
                break;
            }
            // std::cout << "now: " << now->get_number() << std::endl;
            draw = std::uniform_int_distribution<int>(0, outdeg-1)(rng);
            // std::cout << "draw: " << draw << std::endl;
            tmp = now->guards_start();
            while (draw--)
                ++tmp;
            // std::cout << inputdata_locator::get()->string_from_symbol(tmp->first) << std::endl;
            // std::cout << tmp->first << std::endl;
            labels.push_back(inputdata_locator::get()->string_from_symbol(tmp->first));
            now = tmp->second->get_target();
            // std::cout << "next: " << now->get_number() << std::endl;
        }

        output << type << ' ' << labels.size();
        for (auto &label : labels)
            output << ' ' << label;
        output << std::endl;
    }
}
