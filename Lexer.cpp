#include "Lexer.hpp"

std::string Token::tok_to_string(TOK tok) {
  std::string tok_string;
  switch (tok) {
  case TOK::LEFT_SQ:
    tok_string = "LEFT_SQ";
    break;
  case TOK::RIGHT_SQ:
    tok_string = "RIGHT_SQ";
    break;
  case TOK::LEFT_BR:
    tok_string = "LEFT_BR";
    break;
  case TOK::RIGHT_BR:
    tok_string = "RIGHT_BR";
    break;
  case TOK::COLON:
    tok_string = "COLON";
    break;
  case TOK::COMMA:
    tok_string = "COMMA";
    break;
  case TOK::TRUE:
    tok_string = "TRUE";
    break;
  case TOK::FALSE:
    tok_string = "FALSE";
    break;
  case TOK::_NULL:
    tok_string = "NULL";
    break;
  case TOK::NUMBER:
    tok_string = "NUMBER";
    break;
  case TOK::STRING:
    tok_string = "STRING";
    break;
  case TOK::_EOF:
    tok_string = "EOF";
    break;
  case TOK::UNDF:
    tok_string = "UNDF";
    break;
  case TOK::ERROR:
    tok_string = "ERROR";
    break;
  }
  return tok_string;
}
Tokens Lexer::lex() {
  Tokens tokens;
  char curr;
  Token::TOK tok = Token::TOK::UNDF;
  std::array<char, 5> temp;
  uint32_t start_offset = this->pos;
  while (this->pos < src.size()) {
    curr = this->src[this->pos];
    switch (curr) {
    // Structural
    case '[':
      tok = Token::TOK::LEFT_BR;
      break;
    case ']':
      tok = Token::TOK::RIGHT_BR;
      break;
    case '{':
      tok = Token::TOK::LEFT_SQ;
      break;
    case '}':
      tok = Token::TOK::RIGHT_SQ;
      break;
    case ':':
      tok = Token::TOK::COLON;
      break;
    case ',':
      tok = Token::TOK::COMMA;
      break;
      // Literal
    case 't': {
      for (int i = 0; i < 4; i++) {
        temp[i] = this->advance();
      }
      // Change out later
      auto res = std::string_view(temp.begin(), temp.begin() + 4);
      if (res != "true") {
        auto error =
            Token::make_error_token(this->line, {start_offset, this->pos});
        tokens.tokens.push_back(std::move(error));
        this->advance();
        continue;
      }
      tok = Token::TOK::TRUE;

      tokens.tokens.push_back(Token{
          .tok = tok, .line = this->line, .offset = {start_offset, this->pos}});
      continue;
    }
    case 'f': {
      for (int i = 0; i < 5; i++) {
        temp[i] = this->advance();
      }
      // Change out later
      auto res = std::string_view(temp.begin(), temp.begin() + 5);
      if (res != "false") {
        auto error =
            Token::make_error_token(this->line, {start_offset, this->pos});
        tokens.tokens.push_back(std::move(error));
        this->advance();
        continue;
      }
      tok = Token::TOK::FALSE;

      tokens.tokens.push_back(Token{
          .tok = tok, .line = this->line, .offset = {start_offset, this->pos}});
      continue;
    }
    case 'n': {
      for (int i = 0; i < 4; i++) {
        temp[i] = this->advance();
      }
      // Change out later
      auto res = std::string_view(temp.begin(), temp.begin() + 4);
      if (res != "null") {
        auto error =
            Token::make_error_token(this->line, {start_offset, this->pos});
        tokens.tokens.push_back(std::move(error));
        this->advance();
        continue;
      }
      tok = Token::TOK::_NULL;
      tokens.tokens.push_back(Token{
          .tok = tok, .line = this->line, .offset = {start_offset, this->pos}});
      continue;
    }
      // String
    case '"': {
      this->advance();
      uint32_t start_offset = this->pos;
      curr = this->src[this->pos];
      while (curr != '"' || (curr == '"' && this->prev() == '\\')) {
        this->advance();
        curr = this->src[this->pos];
      }
      tokens.tokens.push_back(Token{.tok = Token::TOK::STRING,
                                    .line = this->line,
                                    .offset = {start_offset, this->pos}});
      // std::string_view s(src.begin() + start_offset, src.begin() +
      // this->pos); fmt::println("STRING {}", s);
      this->advance();
      continue;
      break;
    }
    // Number
    case '-':
      //fallthrough
    default:
        // TODO: this is lazy, fix at some point cause its not right
      if (isdigit(curr) || curr == '-') {
        while (isdigit(curr) || curr == '.' || curr == 'e' || curr == 'E' ||
               curr == '+' || curr == '-') {
          this->advance();
          curr = this->src[this->pos];
        }
        tokens.tokens.push_back(Token{.tok = Token::TOK::NUMBER,
                                      .line = this->line,
                                      .offset = {start_offset, this->pos}});
        continue;
      }
      // Whitespace
      if (curr == 0x9 || curr == 0xD || curr == 0x20) {
        this->advance();
        continue;
      }
      if (curr == 0xA) {
        this->line++;
        this->advance();
        continue;
      }
      fmt::println(std::cerr, "Shouldn't be here");
      fmt::print(std::cerr, "");
      exit(1);
    }
    tokens.tokens.push_back(Token{
        .tok = tok, .line = this->line, .offset = {start_offset, this->pos}});
    this->advance();
  }
  tokens.tokens.push_back(Token{.tok = Token::TOK::_EOF,
                                .line = this->line,
                                .offset = {this->pos, this->pos}});
  return tokens;
}
