#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>

#include "json.hpp"
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    std::vector<int> vec(20);
    std::iota(vec.begin(), vec.end(), 0);

    for(int i : vec)
        std::cout << i << '\n';
    
    json j;

    json j_vec(vec);
    j["din mor"] = j_vec;

    std::ofstream o("pretty.json");
    o << std::setw(4) << j << '\n';

    std::cout << "json done :)\n";

    return 0;
}
