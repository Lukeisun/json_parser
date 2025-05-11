
#include "Parser.hpp"
#include <cstddef>
#include <string_view>
#include <variant>
Value Parser::parse() { return this->elements(); }
Value Parser::elements() {
  fmt::println("B {}", this->peek().to_string());
  Value v = this->element();
  fmt::println("A {}", this->peek().to_string());
  if (this->peek().tok == Token::TOK::COMMA) {
    fmt::println("{}", this->advance().to_string());
    Value rest = this->elements();
    std::vector<Value> vals{v, rest};
    return Value(vals);
  } else {
    return v;
  }
}
Value Parser::element() { return this->value(); }
Value Parser::value() {
  auto next = this->advance();
  fmt::println("Processsing {}", next.to_string());
  switch (next.tok) {
  case Token::TOK::LEFT_SQ:
    this->object();
    break;
  case Token::TOK::LEFT_BR:
    this->array();
    break;
  case Token::TOK::TRUE:
    return Value(true);
  case Token::TOK::FALSE:
    return Value(false);
  case Token::TOK::_NULL:
    return Value(nullptr);
  case Token::TOK::NUMBER:
    // Handle as number rn
    return Value(extract_str(next));
    break;
  case Token::TOK::STRING:
    return Value(extract_str(next));
    break;
  case Token::TOK::_EOF:
    break;
  case Token::TOK::UNDF:
  case Token::TOK::ERROR:
  case Token::TOK::RIGHT_SQ:
  case Token::TOK::RIGHT_BR:
  case Token::TOK::COLON:
  case Token::TOK::COMMA:
    return Value(Error{});
  }
  return Value(Error{});
}
Value Parser::object() { return Value(Error{}); }
Value Parser::array() {
  fmt::println("IN ARRAY");
  if (this->peek().tok == Token::TOK::RIGHT_BR) {
    return Value(std::vector<Value>{});
  }
  auto v = this->elements();
  if (!std::holds_alternative<std::vector<Value>>(v.val)) {
    v = Value(std::vector<Value>{v});
  }
  if (this->peek().tok != Token::TOK::RIGHT_BR) {
    fmt::println(std::cerr, "ERROR PARSING ARRAY {}", this->peek().to_string());
    return Value(Error{});
  }
  return v;
}

Token Parser::peek() { return this->tokens[this->pos]; }
Token Parser::advance() { return this->tokens[this->pos++]; }
std::string_view Parser::extract_str(Token tok) {
  return std::string_view(this->src.begin() + tok.offset.first,
                          this->src.begin() + tok.offset.second);
}
