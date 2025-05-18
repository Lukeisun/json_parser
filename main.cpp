#include "Lexer.hpp"
#include "Parser.hpp"
#include "fmt/base.h"
#include "nlohmann/json.hpp"
#include "fmt/ostream.h"
#include "json.hpp"
#include <chrono>
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
  auto start = std::chrono::high_resolution_clock::now();
  JSON json(path);
  json.lex();
  auto v = json.parse();
  std::cout <<"MY: " << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now() - start)
            << "\n";
  start = std::chrono::high_resolution_clock::now();
  std::ifstream ifs(argv[1]);
  nlohmann::json data = nlohmann::json::parse(ifs);
  std::cout <<"NLOHMANN: " << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now() - start)
            << "\n";
  // v["glossary"]["title"].print();
  // v[1].print();
}
