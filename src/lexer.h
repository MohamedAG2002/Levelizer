#pragma once

#include "token.h"

#include <string>
#include <vector>

namespace lvl {

struct Lexer {
  std::string source, path;
  
  int current, start, lines;
  std::vector<Token> tokens;
};
  
Lexer lexer_init(const std::string& path, const std::string& src);
void lexer_lex(Lexer& lexer);

}
