#include "lexer.h"
#include "token.h"

#include <string>
#include <vector>
#include <cstdio>

namespace lvl {

static void log_error(Lexer& lexer, const std::string& msg) {
  printf("[LVL-ERROR, FILE = \'%s\', LINE = %i]: %s\n", lexer.path.c_str(), lexer.current, msg.c_str());
}

static bool is_eof(Lexer& lexer) {
  return lexer.current >= lexer.source.size();
}

static void token_add(Lexer& lexer, TokenType type, const std::string& lexeme) {
  lexer.tokens.push_back(Token{type, lexeme, lexer.lines});
}

static char char_consume(Lexer& lexer) {
  lexer.current++;
  return lexer.source[lexer.current - 1];
}

static char char_peek(Lexer& lexer) {
  if(is_eof(lexer)) {
    return '\0';
  }

  return lexer.source[lexer.current];
}

static char char_peek_next(Lexer& lexer) {
  if((lexer.current + 1) >= lexer.source.size()) {
    return '\0';
  }

  return lexer.source[lexer.current + 1];
}

static bool char_is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

static bool char_is_alpha(char ch) {
  return (ch >= 'a' && ch <= 'z') || 
         (ch >= 'A' && ch <= 'Z') || 
         (ch == '_');
}

static bool char_is_alpha_num(char ch) {
  return char_is_alpha(ch) || char_is_digit(ch);
}

static void str_lit(Lexer& lexer) {
  // Consume chars until you find another '"' 
  while(char_peek(lexer) != '"' && !is_eof(lexer)) {
    if(char_peek(lexer) == '\n') {
      lexer.lines++;
    }

    char_consume(lexer);
  }

  // Searched the whole file and could not find the closing '"' 
  if(is_eof(lexer)) {
    log_error(lexer, "String missing closing \" \n");
    return;
  }

  // Consume the closing "
  char_consume(lexer);

  // Only take the literal inside the ""
  std::string text = lexer.source.substr(lexer.start + 1, (lexer.current - lexer.start) - 2);
  token_add(lexer, TOKEN_STRING_LIT, text);
}

static void num_lit(Lexer& lexer) {
  // Take the integer part
  while(char_is_digit(char_peek(lexer))) {
    char_consume(lexer);
  }

  // Check for a fractional point 
  if(char_peek(lexer) == '.') {
    if(!char_is_digit(char_peek_next(lexer))) {
      log_error(lexer, "Incomplete expression");
      return;
    }

    // Consume the '.'
    char_consume(lexer);

    // Take the fractional part 
    while(char_is_digit(char_peek(lexer))) {
      char_consume(lexer);
    }

    std::string text = lexer.source.substr(lexer.start, (lexer.current - lexer.start));
    token_add(lexer, TOKEN_REAL_LIT, text);
  }
  // There is no fractional part therefore it is just a number 
  else {
    std::string text = lexer.source.substr(lexer.start, (lexer.current - lexer.start));
    token_add(lexer, TOKEN_NUMBER_LIT, text);
  }
}

static TokenType iden_to_token(const std::string& text) {
  if(text == "true") {
    return TOKEN_TRUE;
  }
  else if(text == "false") {
    return TOKEN_FALSE;
  }

  return TOKEN_ERROR; 
}

static void iden_lit(Lexer& lexer) {
  while(char_is_alpha_num(char_peek(lexer))) {
    char_consume(lexer);
  }

  std::string text = lexer.source.substr(lexer.start, (lexer.current - lexer.start));
  TokenType type = iden_to_token(text);
  if(type == TOKEN_ERROR) {
    token_add(lexer, TOKEN_IDEN_LIT, text);
  }
  else {
    token_add(lexer, type, "");
  }
}

Lexer lexer_init(const std::string& path, const std::string& src) {
  Lexer lxr; 
  lxr.path = path;
  lxr.source = src;
  lxr.current = 0;
  lxr.start = 0; 
  lxr.lines = 1;

  return lxr;
}

void lexer_lex(Lexer& lexer) {
  while(!is_eof(lexer)) {
    lexer.start = lexer.current;
    char ch = char_consume(lexer);
 
    switch(ch) {
      case '{':
        token_add(lexer, TOKEN_LEFT_CURLY_BRACKETS, ""); 
        break;
      case '}':
        token_add(lexer, TOKEN_RIGHT_CURLY_BRACKETS, ""); 
        break;
      case '=':
        token_add(lexer, TOKEN_EQUAL, ""); 
        break;
      case '"': 
        str_lit(lexer);
        break;
      case ' ':
      case '\t':
      case '\r':
        break;
      case '\n':
        lexer.lines++;
        break;
      default: 
        if(char_is_digit(ch)) {
          num_lit(lexer);
        }
        else if(char_is_alpha(ch)) {
          iden_lit(lexer); 
        }
        else {
          log_error(lexer, "Unknown token detected");
        }
        break;
    }
  } 

  token_add(lexer, TOKEN_EOF, "");
}

}
