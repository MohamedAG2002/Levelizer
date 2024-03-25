#include "level.h"
#include "entity.h"
#include "lexer.h"
#include "parser.h"

#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdio>
#include <fstream>

namespace lvl {

#define CHECK_VAR_EXISTS(level, types, entt, name, log) {                                                   \
   if(level.entities[entt].types.find(name) == level.entities[entt].types.end()) {                          \
    if(log) {                                                                                               \
     std::string err = "Variable with name \'" + name + "\' does not exist within entity \'" + entt + "\'"; \
     log_error(level, err);                                                                                 \
    }                                                                                                       \
    false;                                                                                                  \
   }                                                                                                        \
  true;                                                                                                     \
} 

#define CHECK_MAP_VALUE_EXISTS(level, types, entt, name) (level.entities[entt].types.find(name) != level.entities[entt].types.end())

static void log_error(Level& level, const std::string& msg) {
  printf("[LVL-ERROR, LVL = %s]: %s\n", level.name.c_str(), msg.c_str());
}

static void read_file_contents(const std::string& path, std::string& src) {
  std::stringstream str;
  std::ifstream file(path); 
  
  std::string extension = path.substr(path.find_last_of('.'), path.npos); 
  if(extension != ".lvl") {
    printf("[ERROR]: Could not take file with type \'%s\'\n", extension.c_str());
    return;
  }
 
  if(!file.is_open()) {
    printf("[ERROR]: Could not open file with name \'%s\'\n", path.c_str());
    return;
  }

  str << file.rdbuf();
  src = str.str();

  // Sort of useless but whatever
  file.close();
}

static void save_variable(Level& level, const std::string& name, const std::string& value) {
  // Just to determine wether to add a tab or not depending 
  // if the current scope is global or an entity. Just 
  // some styling stuff. Not important.
  std::string tab = ""; 
  if(level.current_entity != "global") {
    tab = "  ";
  } 

  level.src += tab + name + " = " + value + '\n';
}

Level load(const std::string& path) {
  Level lvl;
  lvl.name = path.substr(path.find_last_of('/'));
  lvl.path = path;
  lvl.current_entity = "global";
  read_file_contents(lvl.path, lvl.src);

  Lexer lxr = lexer_init(path, lvl.src);
  lexer_lex(lxr); 

  Parser psr = parser_init(lxr.tokens);
  parser_parse(psr, lvl);

  return lvl;
}

void unload(Level& level) {
  for(auto& [key, value] : level.entities) {
    entity_destroy(value);
  }
 
  level.entities.clear();
}

bool save(Level& level) {
  std::ofstream file(level.path, std::ios::trunc);

  if(!file.is_open()) {
    printf("[LVL-ERROR]: Could not open file \'%s\'\n", level.path.c_str());
    return false;
  }
 
  file << level.src;
  file.close();
 
  Lexer lxr = lexer_init(level.path, level.src);
  lexer_lex(lxr);

  Parser psr = parser_init(lxr.tokens);
  parser_parse(psr, level);

  return true;
}

Entity entity_create(Level& lvl, const std::string& name) {
  Entity entt;
  entt.name = name;
 
  lvl.entities[name] = entt;
  return entt;
}

void entity_begin(Level& level, const std::string& name, const bool can_create) {
  // We couldn't find the entity and we can't create a new one with the same name
  if(level.entities.find(name) == level.entities.end() && !can_create) {
    std::string err = "Could not find entity with name \'" + name + "\'";
    log_error(level, err); 

    return;
  }

  Entity entt = {name};
  level.entities[name] = entt;
  level.current_entity = name;

  // Start of a new entity in the file
  level.src += '\n' + name + " {\n";
}

void entity_end(Level& level) {
  level.src += "}\n";

  // Reset the current entity to global so that any subsequent 
  // calls to _set functions are target towards the global scope.
  level.current_entity = "global";
}

void number_set(Level& level, const std::string& name, const int value, const bool can_create) {
  // Could not find the variable and cannot create one either, so just log an error
  // and leave the function.
  if(!CHECK_MAP_VALUE_EXISTS(level, numbers, level.current_entity, name) && !can_create) {
    return;
  }

  // Otherwise create a new entity/set the value of the variable 
  // and save it to the file.
  level.entities[level.current_entity].numbers[name] = value;
  save_variable(level, name, std::to_string(value));
}

void real_set(Level& level, const std::string& name, const float value, const bool can_create) {
  if(!CHECK_MAP_VALUE_EXISTS(level, reals, level.current_entity, name) && !can_create) {
    return;
  }

  level.entities[level.current_entity].reals[name] = value;
  save_variable(level, name, std::to_string(value));
}

void bool_set(Level& level, const std::string& name, const bool value, const bool can_create) {
  if(!CHECK_MAP_VALUE_EXISTS(level, bools, level.current_entity, name) && !can_create) {
    return;
  }

  level.entities[level.current_entity].bools[name] = value;
  save_variable(level, name, std::to_string(value));
}

void string_set(Level& level, const std::string& name, const std::string& value, const bool can_create) {
  if(!CHECK_MAP_VALUE_EXISTS(level, strings, level.current_entity, name) && !can_create) {
    return;
  }

  level.entities[level.current_entity].strings[name] = value;
  save_variable(level, name, value);
}

const int& number_get(Level& level, const std::string& entt, const std::string& name) {
  CHECK_VAR_EXISTS(level, numbers, entt, name, true);
  return level.entities[entt].numbers[name];
}

const float& real_get(Level& level, const std::string& entt, const std::string& name) {
  CHECK_VAR_EXISTS(level, reals, entt, name, true);
  return level.entities[entt].reals[name];
}

const bool& bool_get(Level& level, const std::string& entt, const std::string& name) {
  CHECK_VAR_EXISTS(level, bools, entt, name, true);
  return level.entities[entt].bools[name];
}

const std::string& string_get(Level& level, const std::string& entt, const std::string& name) {
  CHECK_VAR_EXISTS(level, strings, entt, name, true);
  return level.entities[entt].strings[name];
}

}
