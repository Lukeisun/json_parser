#pragma once

#include "Lexer.hpp"
#include <variant>
struct Error {};

class Value {
public:
  typedef std::variant<float, std::string_view, std::vector<Value>, bool,
                       std::nullptr_t, Error>
      value_t;

  value_t val;
  Value(value_t val) : val(val) {}
};

class Parser {
private:
  const std::vector<Token> &tokens;
  const std::string &src;
  uint32_t pos = 0;
  Token peek();
  Token advance();
  std::string_view extract_str(Token tok);

public:
  Parser(const Tokens &tokens, const std::string &src)
      : tokens(tokens.tokens), src(src) {}
  Value parse();
  Value elements();
  Value element();
  Value value();
  Value object();
  Value array();
};
