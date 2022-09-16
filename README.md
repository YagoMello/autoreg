# Autoreg
### C++ Automatic Object Registration Header-only Library

Registers types to a list before main is invoked

## Requirements
- C++ >= 11

## How to use
- `#include "autoreg.hpp"`
- create a list
- add objects to the list
- build the objects

### Creating a list
For single .cpp files `autoreg::list<key_type, base_type> list` is enough.

For multiple files, the list must be initialized before the objects are registered.
The simplest way to ensure the correct initialization order is using a function with a static var and then returning the var reference. There is a macro to simplify this:
```cpp
#define AUTOREG_STATIC_LIST(name, key_type, base_type) \
autoreg::list<key_type, base_type> & name() { \
    static autoreg::list<key_type, base_type> list; \
    return list; \
}
```
And a macro for the function signature, to use in headers:
```cpp
#define AUTOREG_STATIC_LIST_DECL(name, key_type, base_type) \
autoreg::list<key_type, base_type> & name();
```

### Adding objects to the list
The class `autoreg::reg<type>` registers the object `type` to the list\
Create a instance using `autoreg::reg<type> reg_name(list, key)` where:
- `type` is the derived type
- `reg_name` is a variable name
- `list` is the list created in the previous section
- `key` is a key to identify the registry later

Example:
```cpp
// The objects
class base {...};
class derived : public base {...};

// The list to register the objects
AUTOREG_STATIC_LIST(list, std::string, base);

// Registering the derived class
autoreg::reg<derived> derived_reg(list, "derived");

// The base class can be registered too
autoreg::reg<base> base_reg(list, "base");
```

### Building the objects
The `build` functions dynamically create the objects:
```cpp
// Raw pointer
base * obj = autoreg::build(list, key); // dont forget to delete obj later

// Unique pointer
std::unique_ptr<base> obj_unique = autoreg::build_unique(list, key);

// Shared pointer
std::shared_ptr<base> obj_shared = autoreg::build_shared(list, key);
```

## Example
### Declaring the base type
food.hpp
```cpp
#include <string>
#include "autoreg.hpp"

struct food {
    virtual std::string taste() const {
        return "undef.";
    }
};
```

### Declaring the list
regs.hpp
```cpp
#include <string>
#include "autoreg.hpp"
#include "food.hpp"

AUTOREG_STATIC_LIST_DECL(food_list, std::string, food);
```

### Defining the list
regs.cpp
```cpp
#include "regs.hpp"

AUTOREG_STATIC_LIST(food_list, std::string, food);
```

### Creating derived classes and adding to the list
lemon.cpp
```cpp
#include <string>
#include "autoreg.hpp"
#include "regs.hpp"

struct lemon : public food {
    std::string taste() const override {
        return "citric";
    }
};

autoreg::reg<lemon> lemon_reg(food_list, "lemon");
```

strwaberry.cpp
```cpp
#include <string>
#include "autoreg.hpp"
#include "regs.hpp"

struct strawberry : public food {
    std::string taste() const override {
        return "sweet";
    }
};

autoreg::reg<strawberry> strawberry_reg(food_list, "strawberry");
```

### The main
main.cpp
```cpp
#include <string>
#include <iostream>
#include "autoreg.hpp"
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

```

## Helper functions
### `as_list(list_or_func)`
Returns a `list` reference (or const reference).

When using the macro `AUTOREG_STATIC_LIST`, the list is a function. This may break generic code, for example:
```cpp
for(auto & [key, builder] : list) {
    std::cout << "- " << key << "\n";
}
```
Which becomes
```cpp
for(auto & [key, builder] : autoreg::as_list(list)) {
    std::cout << "- " << key << "\n";
}
```
