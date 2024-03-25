#include "parser.h"
#include "lexer.h"
#include "level.h"

#include <vector>
#include <cstdio>

namespace lvl {

static void log_error(Token& token, const std::string& msg) {
  printf("[PARSE-ERROR, LINE = %i]: %s\n", token.line, msg.c_str());
}

static bool is_eof(Parser& parser) {
  return parser.tokens[parser.current].type == TOKEN_EOF;
}

static Token token_last(Parser& parser) {
  return parser.tokens[parser.tokens.size() - 1];
}

static Token token_first(Parser& parser) {
  return parser.tokens[0];
}

static Token token_peek_next(Parser& parser) {
  if(is_eof(parser)) {
    return token_last(parser);
  } 

  return parser.tokens[parser.current];
}

static Token token_peek_prev(Parser& parser) {
  if(parser.current <= 0) {
    return token_first(parser);
  } 

  return parser.tokens[parser.current - 2];
}

static Token token_consume(Parser& parser) {
  if(is_eof(parser)) {
    return parser.tokens[parser.current];
  }
  
  parser.current++;
  return parser.tokens[parser.current - 1];
}

static void assign_expr(Parser& parser, Token& token, const std::string& scope, Level& lvl) {
  Token next_token = token_peek_next(parser);
  Token prev_token = token_peek_prev(parser);
      
  if(prev_token.type != TOKEN_IDEN_LIT) {
    log_error(token, "Assignment expression requires an identifier");
    return;
  }

  switch(next_token.type) {
    case TOKEN_NUMBER_LIT:
      lvl.entities[scope].numbers[prev_token.lexeme] = std::stoi(next_token.lexeme);
      break;
    case TOKEN_REAL_LIT:
      lvl.entities[scope].reals[prev_token.lexeme] = std::stof(next_token.lexeme);
      break;
    case TOKEN_STRING_LIT:
      lvl.entities[scope].strings[prev_token.lexeme] = next_token.lexeme;
      break;
    case TOKEN_TRUE:
      lvl.entities[scope].bools[prev_token.lexeme] = true;
      break;
    case TOKEN_FALSE:
      lvl.entities[scope].bools[prev_token.lexeme] = false;
      break;
    default:
      log_error(token, "Cannot assign to an unassignable type"); 
      break;
  }

  // Consume the value
  token_consume(parser);
}

static void entity_expr(Parser& parser, Level& lvl, Token& token, std::string& scope) {
  if(token_peek_prev(parser).type != TOKEN_IDEN_LIT) {
    log_error(token, "Entity statement must start with an identifier");
    return;
  }

  for(int i = parser.current; i <= parser.tokens.size(); i++) {
    Token curr_token = parser.tokens[i]; 
  
    // Find a '}'? Good! Just move on
    if(curr_token.type == TOKEN_RIGHT_CURLY_BRACKETS) {
      break;
    }

    // Otherwise, the end of the file was reached or another '{' was found which 
    // does not make sense since the previous scope was not terminated yet.
    if(curr_token.type == TOKEN_EOF || curr_token.type == TOKEN_LEFT_CURLY_BRACKETS) {
      log_error(curr_token, "Missing closing \'}\'");
      return;
    }
  }
    
  scope = token_peek_prev(parser).lexeme;
  entity_create(lvl, scope);
}

Parser parser_init(const std::vector<Token>& tokens) {
  Parser psr;
  psr.tokens = tokens;
  psr.current = 0;

  return psr;
}

void parser_parse(Parser& parser, Level& lvl) {
  bool has_begun_scope = false;
  std::string scope = "global";

  // Create a global entity. This MUST be created to keep all of the global variables
  entity_create(lvl, scope);

  while(!is_eof(parser)) {
    Token tkn = token_consume(parser);

    switch(tkn.type) {
      case TOKEN_LEFT_CURLY_BRACKETS:
        entity_expr(parser, lvl, tkn, scope);
        has_begun_scope = true;
        break;
      case TOKEN_RIGHT_CURLY_BRACKETS:
        // Go back to the global scope after the last scope was closed off
        scope = "global"; 
       
        // Why terminate a scope that was not declared in the first place?
        if(!has_begun_scope) {
          log_error(tkn, "Extra uneeded \'}\'");
        } 
        else {
          // Terminate the previous scope
          has_begun_scope = false;
        }
        break;
      case TOKEN_EQUAL:
        assign_expr(parser, tkn, scope, lvl);
        break;
      case TOKEN_NUMBER_LIT:
      case TOKEN_REAL_LIT:
      case TOKEN_STRING_LIT:
      case TOKEN_TRUE:
      case TOKEN_FALSE:
        if(token_peek_prev(parser).type != TOKEN_EQUAL) {
          log_error(tkn, "Hanging literal is not allowed. Please always use \'=\' with literals"); 
        }
        break;
      case TOKEN_IDEN_LIT:
        if(token_peek_next(parser).type != TOKEN_EQUAL && token_peek_next(parser).type != TOKEN_LEFT_CURLY_BRACKETS) {
          // I FUCKING HATE THIS SOOOOOO MUCH!!!!!!! FUCK YOU C++!!! FUCK YOU!
          std::string err = "Cannot determine the use of a hanging \'" + tkn.lexeme + "\' identifier";
          log_error(tkn, err); 
        }
        break;
      case TOKEN_EOF:
        break;
      default:
        log_error(tkn, "Unknown token detected");
        break;
    }
  }
}

}
