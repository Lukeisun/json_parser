#pragma once

#include "Lexer.hpp"
#include "Parser.hpp"
#include <filesystem>

class JSON {
private:
public:
  std::string content;
  Tokens tokens;
  JSON(const std::filesystem::path &path) {
    std::ifstream s(path);
    if (!s) {
      fmt::println(std::cerr, "Couldn't open file");
      exit(EXIT_FAILURE);
    }
    std::ostringstream oss;
    oss << s.rdbuf();
    content = oss.str();
  }

  // For now just copy
  JSON(const std::string &content) : content(content) {}

  void lex() {
    Lexer lexer(content);
    this->tokens = lexer.lex();
  }
  Value parse() {
    if (tokens.tokens.empty()) {
      fmt::println(std::cerr, "Please Lex First");
      exit(EXIT_FAILURE);
    }
    Parser parser(this->tokens, this->content);
    auto v = parser.parse();
    return v;
  }
};
