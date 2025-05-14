#pragma once

#include "Lexer.hpp"
#include "map"
#include <variant>
struct Error {};

template <class... Ts> struct overloads : Ts... {
  using Ts::operator()...;
};

class Value {
private:
public:
  static Value _error;
  typedef std::map<std::string_view, Value> object_t;
  typedef std::vector<Value> array_t;
  typedef std::variant<float, std::string_view, array_t, bool,
                       std::nullptr_t, Error, object_t>
      value_t;
  value_t val;
  Value(value_t val) : val(val) {}
  void print(int indent = 0) const;
  Value &operator[](std::size_t idx);
  const Value &operator[](std::size_t idx) const;
  Value &operator[](std::string_view key);
  const Value &operator[](std::string_view key) const;
};

class Parser {
private:
  const std::vector<Token> &tokens;
  const std::string &src;
  uint32_t pos = 0;
  Token peek();
  Token advance();
  bool match(Token::TOK tok);
  std::string_view extract_str(Token tok);

public:
  Parser(const Tokens &tokens, const std::string &src)
      : tokens(tokens.tokens), src(src) {}
  Value parse();
  Value elements();
  Value element();
  Value value();
  Value object();
  Value members(Value::object_t *root_obj = nullptr);
  Value array();
};
