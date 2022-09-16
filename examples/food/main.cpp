#include <string>
#include <iostream>
#include "../../autoreg.hpp"
#include "regs.hpp"

int main() {
    std::string food_name;
    
    // Ask the food name
    std::cout << "Select a food:\n";
    
    // Iterate through the foods in the list
    for(auto & [key, builder] : autoreg::as_list(food_list)) {
        std::cout << "- " << key << "\n";
    }
    
    // Read the food name
    std::cin >> food_name;
    
    // Try to build a food instance
    auto food_instance = autoreg::build_shared(food_list, food_name);
    
    // Test if the creation was successfull
    if(food_instance != nullptr) {
        std::cout << food_name << " tastes " << food_instance->taste() << "\n";
    }
    else {
        std::cout << food_name << " not found\n";
    }
    
    return 0;
}
