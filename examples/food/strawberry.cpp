#include <string>
#include "../../autoreg.hpp"
#include "regs.hpp"

struct strawberry : public food {
    std::string taste() const override {
        return "sweet";
    }
};

autoreg::reg<strawberry> strawberry_reg(food_list, "strawberry");
