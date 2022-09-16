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
class base {...};
class derived : public base {...};

AUTOREG_STATIC_LIST(list, std::string, base);

autoreg::reg<derived> derived_reg(list, "derived");
```

