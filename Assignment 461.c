#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define LTD "285" // Replace this with your actual last three digits

typedef enum {
 TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE,
 TOKEN_LBRACE, TOKEN_RBRACE,
 TOKEN_LPAREN, TOKEN_RPAREN,
 TOKEN_SEMICOLON,
 TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV,
 TOKEN_EQ, TOKEN_NEQ, TOKEN_LT, TOKEN_GT, TOKEN_LEQ,
 TOKEN_GEQ,
 TOKEN_NUMBER, TOKEN_IDENTIFIER, TOKEN_LTD,
 TOKEN_END, TOKEN_INVALID
} TokenType;

typedef struct {
 TokenType type;
 char lexeme[MAX_TOKEN_LENGTH];
} Token;

char *input;
int pos = 0;
Token current_token;

// === Forward Declarations ===
void next_token();
void parse_program();
void parse_block();
void parse_statement();
void parse_if();
void parse_while();
void parse_condition();
void parse_expression();
void parse_term();
void parse_factor();
void error(const char *message);

// === Lexer ===
int is_letter(char c) { return isalpha(c); }
int is_digit(char c) { return isdigit(c); }

void skip_whitespace() {
 while (isspace(input[pos])) pos++;
}

Token get_token() {
 skip_whitespace();
 Token token = { TOKEN_INVALID, "" };
 char c = input[pos];
 if (c == '\0') {
  token.type = TOKEN_END;
  return token;
 }

 if (is_letter(c)) {
  int start = pos;
  while (is_letter(input[pos]) || is_digit(input[pos])) pos++;
  int len = pos - start;
  strncpy(token.lexeme, &input[start], len);
  token.lexeme[len] = '\0';
  if (strcmp(token.lexeme, "if") == 0) token.type = TOKEN_IF;
  else if (strcmp(token.lexeme, "else") == 0) token.type = TOKEN_ELSE;
  else if (strcmp(token.lexeme, "while") == 0) token.type = TOKEN_WHILE;
  else if (strcmp(token.lexeme, "LTD") == 0) token.type = TOKEN_LTD;
  else token.type = TOKEN_IDENTIFIER;
  return token;
 }

 if (is_digit(c)) {
  int start = pos;
  while (is_digit(input[pos])) pos++;
  int len = pos - start;
  strncpy(token.lexeme, &input[start], len);
  token.lexeme[len] = '\0';
  token.type = TOKEN_NUMBER;
  return token;
 }

 pos++;
 switch (c) {
  case '{': token.type = TOKEN_LBRACE; strcpy(token.lexeme, "{"); break;
  case '}': token.type = TOKEN_RBRACE; strcpy(token.lexeme, "}"); break;
  case '(': token.type = TOKEN_LPAREN; strcpy(token.lexeme, "("); break;
  case ')': token.type = TOKEN_RPAREN; strcpy(token.lexeme, ")"); break;
  case ';': token.type = TOKEN_SEMICOLON; strcpy(token.lexeme, ";"); break;
  case '+': token.type = TOKEN_PLUS; strcpy(token.lexeme, "+"); break;
  case '-': token.type = TOKEN_MINUS; strcpy(token.lexeme, "-"); break;
  case '*': token.type = TOKEN_MULT; strcpy(token.lexeme, "*"); break;
  case '/': token.type = TOKEN_DIV; strcpy(token.lexeme, "/"); break;
  case '=':
   if (input[pos] == '=') {
    pos++;
    token.type = TOKEN_EQ;
    strcpy(token.lexeme, "==");
   }
   break;
  case '!':
   if (input[pos] == '=') {
    pos++;
    token.type = TOKEN_NEQ;
    strcpy(token.lexeme, "!=");
   }
   break;
  case '<':
   if (input[pos] == '=') {
    pos++;
    token.type = TOKEN_LEQ;
    strcpy(token.lexeme, "<=");
   } else {
    token.type = TOKEN_LT;
    strcpy(token.lexeme, "<");
   }
   break;
  case '>':
   if (input[pos] == '=') {
    pos++;
    token.type = TOKEN_GEQ;
    strcpy(token.lexeme, ">=");
   } else {
    token.type = TOKEN_GT;
    strcpy(token.lexeme, ">");
   }
   break;
  default:
   token.type = TOKEN_INVALID;
 }
 return token;
}

void next_token() {
 current_token = get_token();
}

// === Parser Implementation ===
void match(TokenType expected) {
 if (current_token.type == expected)
  next_token();
 else
  error("Unexpected token");
}

void error(const char *message) {
 printf("Syntax Error: %s at '%s'\n", message, current_token.lexeme);
 exit(1);
}

void parse_program() {
 parse_block();
 if (current_token.type != TOKEN_END)
  error("Extra input after program end");
}

void parse_block() {
 match(TOKEN_LBRACE);
 while (current_token.type != TOKEN_RBRACE && current_token.type != TOKEN_END) {
  parse_statement();
 }
 match(TOKEN_RBRACE);
}

void parse_statement() {
 if (current_token.type == TOKEN_IF) parse_if();
 else if (current_token.type == TOKEN_WHILE) parse_while();
 else {
  parse_expression();
  match(TOKEN_SEMICOLON);
 }
}

void parse_if() {
 match(TOKEN_IF);
 match(TOKEN_LPAREN);
 parse_condition();
 match(TOKEN_RPAREN);
 parse_block();
 if (current_token.type == TOKEN_ELSE) {
  match(TOKEN_ELSE);
  parse_block();
 }
}

void parse_while() {
 match(TOKEN_WHILE);
 match(TOKEN_LPAREN);
 parse_condition();
 match(TOKEN_RPAREN);
 parse_block();
}

void parse_condition() {
 parse_expression();
 switch (current_token.type) {
  case TOKEN_EQ: case TOKEN_NEQ:
  case TOKEN_LT: case TOKEN_GT:
  case TOKEN_LEQ: case TOKEN_GEQ:
   next_token(); break;
  default:
   error("Expected relational operator");
 }
 parse_expression();
}

void parse_expression() {
 parse_term();
 while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
  next_token();
  parse_term();
 }
}

void parse_term() {
 parse_factor();
 while (current_token.type == TOKEN_MULT || current_token.type == TOKEN_DIV) {
  next_token();
  parse_factor();
 }
}

void parse_factor() {
 if (current_token.type == TOKEN_NUMBER || current_token.type == TOKEN_IDENTIFIER || current_token.type == TOKEN_LTD) {
  next_token();
 } else if (current_token.type == TOKEN_LPAREN) {
  match(TOKEN_LPAREN);
  parse_expression();
  match(TOKEN_RPAREN);
 } else {
  error("Invalid factor");
 }
}

// === Main Function ===
int main() {
 input = "{ if (a == LTD) { while (b < 100) { (a + b) * (b - LTD); } } else { (x + y) * (a - b); } }";
 next_token(); // Initialize first token
 parse_program();
 printf(" Program parsed successfully.\n");
 return 0;
}
