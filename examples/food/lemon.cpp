#include <string>
#include "../../autoreg.hpp"
#include "regs.hpp"

struct lemon : public food {
    std::string taste() const override {
        return "citric";
    }
};

autoreg::reg<lemon> lemon_reg(food_list, "lemon");
