#pragma once

#include "lexer.h"
#include "level.h"

#include <vector>

namespace lvl {

struct Parser {
  std::vector<Token> tokens;
  int current;
};

Parser parser_init(const std::vector<Token>& tokens);
void parser_parse(Parser& parser, Level& level);

}
