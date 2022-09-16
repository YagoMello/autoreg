#ifndef LIBAUTOREG__AUTOREG_HPP
#define LIBAUTOREG__AUTOREG_HPP

/* Automatic Object Registration Library
 * Author:  Yago T. de Mello
 * e-mail:  yago.t.mello@gmail.com
 * Version: 2.3.3 2022-09-16
 * License: Apache 2.0
 * C++ >= 11
 */

/*
Copyright 2022 Yago Teodoro de Mello
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// autoreg::list -> std::map
#include <map>

// unique_ptr and shared_ptr
#include <memory>

// Check for type_identity (C++20) support
#if __cplusplus > 202002L 
    
    #include <type_traits> // std::type_identity
    
    namespace autoreg {
    
    template<typename T>
    using std::type_identity<T>;
    
    template<typename T>
    using std::type_identity_t<T>;
    
#else // __cplusplus > 202002L == false
    
    // lets implement our type identity
    
    namespace autoreg {
    
    template<typename T>
    struct type_identity {
        using type = T;
    };
    
    // the compiler doesn't deduce T
    template<typename T>
    using type_identity_t = typename type_identity<T>::type;
    
#endif // __cplusplus > 202002L 


// A function that constructs objects
template <typename base_type>
using builder_type = base_type * (*)();

// Maps a key to the object constructor
template <typename key_type, typename base_type>
using list = std::map<key_type, builder_type<base_type>>;

// The static list func type
template <typename key_type, typename base_type>
using func = autoreg::list<key_type, base_type> & (*)();

// Constructs an object
template <typename base_type, typename derived_type>
base_type * default_builder() {
    return new derived_type;
}

// The class does nothing, the important thing here is that constructors can be used
// to run code before the program start.
template <typename derived_type>
class reg {
public:
    // Deleting all normal constructors
    reg()            = delete;
    reg(const reg &) = delete;
    reg(reg &&)      = delete;
    
    // Automatically creates the builder function and saves it to the list.
    // THE LIST MUST BE INITIALIZED BEFORE USED IN THIS FUNCTION,
    // the easiest to do this is by creating a function with a "static autoreg::list<type>" then
    // returning a reference to the list.
    template <typename key_type, typename base_type>
    reg(
        autoreg::list<key_type, base_type> & ls, 
        const type_identity_t<key_type>    & key     = derived_type::key(), 
        autoreg::builder_type<base_type>     builder = &autoreg::default_builder<base_type, derived_type>
    ) {
        ls[key] = builder;
    }
    
    // Automatically creates the builder function and saves it to the list.
    template <typename key_type, typename base_type>
    reg(
        autoreg::func<key_type, base_type>   fn,
        const type_identity_t<key_type>    & key     = derived_type::key(),
        autoreg::builder_type<base_type>     builder = &autoreg::default_builder<base_type, derived_type>
    ) {
        fn()[key] = builder;
    }
};

// If the object builder exists, build an object, otherwise returns a nullptr.
template <typename key_type, typename base_type>
inline base_type * build(
    autoreg::list<key_type, base_type> & ls, 
    const type_identity_t<key_type> & key
) {
    auto iter = ls.find(key);
    
    if(iter != ls.end()) {
        return (iter->second)(); // calls the builder
    }
    else {
        return nullptr;
    }
}

// If the object builder exists, build an object, otherwise returns a nullptr.
template <typename key_type, typename base_type>
inline base_type * build(
    autoreg::func<key_type, base_type> fn, 
    const type_identity_t<key_type> & key
) {
    return autoreg::build(fn(), key);
}

// If the object builder exists, build an object, otherwise returns a nullptr.
template <typename key_type, typename base_type>
inline std::unique_ptr<base_type> build_unique(
    autoreg::list<key_type, base_type> & ls, 
    const type_identity_t<key_type> & key
) {
    return std::unique_ptr<base_type>(autoreg::build(ls, key));
}

// If the object builder exists, build an object, otherwise returns a nullptr.
template <typename key_type, typename base_type>
inline std::unique_ptr<base_type> build_unique(
    autoreg::func<key_type, base_type> fn, 
    const type_identity_t<key_type> & key
) {
    return std::unique_ptr<base_type>(autoreg::build(fn, key));
}

// If the object builder exists, build an object, otherwise returns a nullptr.
template <typename key_type, typename base_type>
inline std::shared_ptr<base_type> build_shared(
    autoreg::list<key_type, base_type> & ls, 
    const type_identity_t<key_type> & key
) {
    return std::shared_ptr<base_type>(autoreg::build(ls, key));
}

// If the object builder exists, build an object, otherwise returns a nullptr.
template <typename key_type, typename base_type>
inline std::shared_ptr<base_type> build_shared(
    autoreg::func<key_type, base_type> fn, 
    const type_identity_t<key_type> & key
) {
    return std::shared_ptr<base_type>(autoreg::build(fn, key));
}

// Always returns a list, even when the input is a func
template <typename key_type, typename base_type>
autoreg::list<key_type, base_type> & as_list(
    autoreg::list<key_type, base_type> & ls
) {
    return ls;
}

// Always returns a list, even when the input is a func
template <typename key_type, typename base_type>
const autoreg::list<key_type, base_type> & as_list(
    const autoreg::list<key_type, base_type> & ls
) {
    return ls;
}

// Always returns a list, even when the input is a func
template <typename key_type, typename base_type>
autoreg::list<key_type, base_type> & as_list(
    autoreg::func<key_type, base_type> fn
) {
    return fn();
}

} // namespace autoreg


// Simplifies the creation of safe lists
// Header declaration of the list
#define AUTOREG_STATIC_LIST_DECL(name, key_type, base_type) \
autoreg::list<key_type, base_type> & name();

// Simplifies the creation of safe lists
#define AUTOREG_STATIC_LIST(name, key_type, base_type) \
autoreg::list<key_type, base_type> & name() { \
    static autoreg::list<key_type, base_type> static_list; \
    return static_list; \
}

#endif // LIBAUTOREG__AUTOREG_HPP
