#pragma once

#include "array"
#include "fmt/ostream.h"
#include <cstdint>
#include <fmt/base.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
class Token {
public:
  enum class TOK {
    // Structural
    LEFT_SQ,
    RIGHT_SQ,
    LEFT_BR,
    RIGHT_BR,
    COLON,
    COMMA,
    // LITERAL
    TRUE,
    FALSE,
    _NULL,

    NUMBER,
    STRING,

    _EOF,
    UNDF,
    ERROR,
  };
  TOK tok;
  uint32_t line;
  std::pair<uint32_t, uint32_t> offset;

  static std::string tok_to_string(TOK tok);
  static Token make_error_token(uint32_t line,
                                std::pair<uint32_t, uint32_t> offset) {
    return {.tok = TOK::ERROR, .line = line, .offset = offset};
  }
  std::string to_string() const {
    return Token::tok_to_string(tok) + ", " + std::to_string(line) + ", (" +
           std::to_string(offset.first) + ", " + std::to_string(offset.second) +
           ")";
  }
};
class Tokens {
public:
  std::vector<Token> tokens;
};

class Lexer {
private:
  const std::string &src;
  uint32_t line = 0;
  uint32_t pos = 0;
  bool had_error = false;

public:
  Lexer(const std::string &src) : src(src) {}
  Tokens lex();
  char advance() { return this->src[this->pos++]; }
  char prev() { return this->src[this->pos - 1]; }
};
