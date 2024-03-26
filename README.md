# What Is It?
Levelizer is a simple and very bare-bones data serialization language specifically made to serialize and deserialize levels and scenes. It is made with scene and level serialization in mind. Even though it can be used for other purposes, it was specifically geared towards game engines and world serialization in general. It uses C++ and the STL library of course. The lexer and the parser of Levelizer is very simple, much like the language. 

Levelizer mainly takes '.lvl' files, lexes them, parses them, and returns a `Level` struct back with the required information. You can then access variables indivisually from the `Level` struct. The `Level` struct contains many `Entity` structs. Inside the `Entity` struct, there's information about all of the variables that exists within the specific `Entity` scope. From the `Entity`, you can retrieve numbers (1, 2, etc.), reals (3.14159, 89,43, etc.), strings, and bools (true or false). Unfortunately, Levelizer does not support variables with mulitple types. Only single value variables. By default, a "global" `Entity` is created once the `Level` is loaded. That way, you can add global variables and access them pretty easily.  

I would like to make a disclaimer, however. Levelizer is only a hobby project; nothing more. It is *not* to be used in big projects as it is not fast and not optimized whatsoever. There are way better methods to create a DSL than this. I'm only making this for educational purposes. 

# How To Build?
Levelizer, like most C++ projects, uses CMake as its build system. So use the commands below to build and compile the project.

```
git clone https://github.com/MohamedAG2002/Levelizer.git 
cd Levelizer
mkdir build && cd build 
cmake -DCMAKE_BUILD_TYPE=SHARED .. 
make 
```

# How To Use?
Here is the most basic usage of Levelizer:

```cpp
#include <levelizer/level.h>

int main() {
    // Load a .lvl file from the specified path. 
    // NOTE: Please make sure the file *actually* exists. 
    lvl::Level lvl = lvl::load("assets/level/main_level.lvl");

    // "Start" on an entity skeleton. 
    // NOTE: If the 'entity_begin' function was called, 
    // the 'entity_end' *must* be called.
    lvl::entity_begin(lvl, "player");
    
    // Set a real named "pos_x" to the value 100.9. 
    // NOTE: The 'real_set' function has an optional parametar called 'can_create'.
    // When this parametar is set to true, the function will create a new variable 
    // with the same name and give it the specified value. Otherwise, Levelizer 
    // will log an error and return from the function.
    // By default, this parametar is set to true.
    lvl::real_set(lvl, "pos_x", 100.9f);

    // Set another real. 
    lvl::real_set(lvl, "pos_y", 281.45f);

    // Set a number. 
    lvl::number_set(lvl, "health", 100);
   
    // Set a string
    lvl::string_set(lvl, "id", "player");
   
    // Set a bool
    lvl::bool_set(lvl, "is_active", true);

    // Here we end the entity. You can start a new entity after this point.
    lvl::entity_end(lvl);

    // When the 'entity_end' function is called, the scope gets reset to 
    // "global". Meaning, if you want to set a global variable in the 
    // file, you simply call any of the previously shown '_set' functions 
    // without surrounding it with 'entity_begin' and 'entity_end' functions.
    lvl::number_set(lvl, "score", 245);

    // After setting all of the variables and creating all of the entities needed, 
    // You must save the 'Level' struct. The 'save' function will take the 'src' 
    // string that exists within the 'Level' struct and save it to the file 
    // given when the level was first loaded. After that it runs the lexer and parser 
    // again to put the changes into effect. 
    lvl::save(lvl);

    // You can retrieve any variable with the '_get' functions. 
    // NOTE: If the variable was not found, the function will 
    // log an error and return a default value. 
    int player_health = lvl::number_get(lvl, "player", "health");
}
```
