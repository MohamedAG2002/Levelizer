#pragma once

#include <string>
#include <vector>

namespace lvl {

enum TokenType {
  TOKEN_ERROR = -1,
  TOKEN_EOF   = 0,

  TOKEN_LEFT_CURLY_BRACKETS  = 123,
  TOKEN_RIGHT_CURLY_BRACKETS = 125,
  TOKEN_EQUAL                = 61,
  
  TOKEN_NUMBER_LIT = 276, 
  TOKEN_REAL_LIT   = 277, 
  TOKEN_STRING_LIT = 278, 
  TOKEN_IDEN_LIT   = 279, 
  
  TOKEN_TRUE  = 280, 
  TOKEN_FALSE = 281, 
};

struct Token {
  TokenType type;
  std::string lexeme; 
  int line;
};

struct Lexer {
  std::string source, path;
  
  int current, start, lines;
  std::vector<Token> tokens;
};
  
Lexer lexer_init(const std::string& path, const std::string& src);
void lexer_lex(Lexer& lexer);

}
