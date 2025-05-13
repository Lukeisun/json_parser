
#include "Parser.hpp"
#include <cstddef>
#include <string_view>
#include <variant>

void Value::print(int indent) const {
  fmt::print("{}", std::string(indent, '\t'));
  std::visit(overloads{
                 [](float v) { fmt::print("{}", v); },
                 [](std::string_view v) { fmt::print("\"{}\"", v); },
                 [](std::vector<Value> v) {
                   fmt::print("[");
                   for (const auto &ele : v) {
                     ele.print();
                     fmt::print(" ,");
                   }
                   fmt::print("]");
                 },
                 [](bool v) { fmt::print("{}", v ? "true" : "false"); },
                 [](std::nullptr_t v) { fmt::print("null"); },
                 [](Error v) { fmt::print("ERROR"); },
                 [](object_t v) {
                   fmt::print("{{");
                   for (const auto &ele : v) {
                     fmt::print("\"{}\" : ", ele.first);
                     ele.second.print();
                     fmt::print(", ");
                   }
                   fmt::print("}}");
                 },
             },
             this->val);
}
Value Parser::parse() { return this->elements(); }
Value Parser::elements() {
  Value v = this->element();
  std::vector<Value> maybe_v{v};
  while (this->peek().tok == Token::TOK::COMMA) {
    this->advance();
    Value rest = this->element();
    maybe_v.push_back(rest);
  }
  return maybe_v.size() == 1 ? v : Value(maybe_v);
}
Value Parser::element() { return this->value(); }
Value Parser::value() {
  auto next = this->advance();
  fmt::println("Processsing {}", next.to_string());
  switch (next.tok) {
  case Token::TOK::LEFT_SQ:
    return this->object();
  case Token::TOK::LEFT_BR:
    return this->array();
  case Token::TOK::TRUE:
    return Value(true);
  case Token::TOK::FALSE:
    return Value(false);
  case Token::TOK::_NULL:
    return Value(nullptr);
  case Token::TOK::NUMBER:
    // Handle as number rn
    return Value(extract_str(next));
  case Token::TOK::STRING: {
    auto sv = extract_str(next);
    return Value(sv);
  }
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
Value Parser::object() {
  if (this->peek().tok == Token::TOK::RIGHT_SQ) {
    return Value(Value::object_t{});
  }
  auto x = this->members();
  return x;
}
Value Parser::members(Value::object_t *root_obj) {
  Value::object_t object;
  auto str = this->advance();
  if (str.tok != Token::TOK::STRING || !this->match(Token::TOK::COLON)) {
    fmt::println(std::cerr, "ERROR PARSING OBJECT", this->peek().to_string());
    return Value(Error{});
  }
  auto v = this->element();
  std::string_view name(this->src.begin() + str.offset.first,
                        this->src.begin() + str.offset.second);
  auto obj = root_obj ? root_obj : &object;
  obj->insert({name, v});
  if (this->peek().tok == Token::TOK::COMMA) {
    this->advance();
    return this->members(obj);
  }
  if (this->match(Token::TOK::RIGHT_SQ)) {
    return Value(*obj);
  }
  return Value(Error{});
}
Value Parser::array() {
  if (this->peek().tok == (Token::TOK::RIGHT_BR)) {
    this->advance();
    return Value(std::vector<Value>{});
  }
  auto v = this->elements();
  if (!std::holds_alternative<std::vector<Value>>(v.val)) {
    v = Value(std::vector<Value>{v});
  }
  if (!this->match(Token::TOK::RIGHT_BR)) {
    fmt::println(std::cerr, "ERROR PARSING ARRAY {}", this->peek().to_string());
    return Value(Error{});
  }
  return v;
}

bool Parser::match(Token::TOK tok) { return this->advance().tok == tok; }
Token Parser::peek() { return this->tokens[this->pos]; }
Token Parser::advance() { return this->tokens[this->pos++]; }
std::string_view Parser::extract_str(Token tok) {
  return std::string_view(this->src.begin() + tok.offset.first,
                          this->src.begin() + tok.offset.second);
}
