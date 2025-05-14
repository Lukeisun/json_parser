#include "Lexer.hpp"
#include "json.hpp"
#include "Parser.hpp"
#include "fmt/base.h"
#include "fmt/ostream.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>

std::ostream &operator<<(std::ostream &out, Token const &data) {
  out << data.to_string();
  return out;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fmt::println(std::cerr, "Please provide file");
    exit(EXIT_FAILURE);
  }
  std::filesystem::path path(argv[1]);
  JSON json(path);
  json.lex();
  auto v = json.parse();
  v["glossary"]["title"].print();
}
