#include "Lexer.hpp"
#include "fmt/base.h"
#include "fmt/ostream.h"
#include <cstdlib>
#include <filesystem>
#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

std::ostream &operator<<(std::ostream &out, Token const &data) {
  out << data.to_string();
  return out;
}
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

  JSON(const std::string &content) {}
  void lex() {
    Lexer lexer(content);
    this->tokens = lexer.lex();
    // for (const auto &token : this->tokens.tokens) {
    //   fmt::println("{} ", token.to_string());
    // }
  }
  void parse() {
    if (tokens.tokens.empty()) {
      fmt::println(std::cerr, "Please Lex First");
      exit(EXIT_FAILURE);
    }
    Parser parser(this->tokens, this->content);
    auto v = parser.parse();
    v.print();
  }
};

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::println(std::cerr, "Please provide file");
    exit(EXIT_FAILURE);
  }
  std::filesystem::path path(argv[1]);
  JSON json(path);
  json.lex();
  json.parse();
}
