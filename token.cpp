//token.cpp
#include "token.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

//Extern map from token.h
map<string, string> operator_map;

//Extern var from token.h
map<string, string> keyword_map;

void populate_operator_map()
{
  operator_map.insert(make_pair("=", "EQUALS_tk"));
  operator_map.insert(make_pair("<", "LESS_tk"));
  operator_map.insert(make_pair(":", "COLON_tk"));
  operator_map.insert(make_pair(">", "GREAT_tk"));
  operator_map.insert(make_pair("+", "PLUS_tk"));
  operator_map.insert(make_pair("-", "MINUS_tk"));
  operator_map.insert(make_pair("*", "ASTERIK_tk"));
  operator_map.insert(make_pair("/", "SLASH_tk"));
  operator_map.insert(make_pair("%", "PCT_tk"));
  operator_map.insert(make_pair(".", "PERIOD_tk"));
  operator_map.insert(make_pair("(", "OPAR_tk"));
  operator_map.insert(make_pair(")", "CPAR_tk"));
  operator_map.insert(make_pair(",", "COMMA_tk"));
  operator_map.insert(make_pair("{", "OBRACE_tk"));
  operator_map.insert(make_pair("}", "CBRACE_tk"));
  operator_map.insert(make_pair(";", "SCOLON_tk"));
  operator_map.insert(make_pair("[", "OBRACKET_tk"));
  operator_map.insert(make_pair("]", "CBRACKET_tk"));
}

void populate_keyword_map()
{
  keyword_map.insert(make_pair("begin", "BEGIN_tk"));
  keyword_map.insert(make_pair("end", "END_tk"));
  keyword_map.insert(make_pair("iter", "ITER_tk"));
  keyword_map.insert(make_pair("void", "VOID_tk"));
  keyword_map.insert(make_pair("var", "VAR_tk"));
  keyword_map.insert(make_pair("return", "RETURN_tk"));
  keyword_map.insert(make_pair("read", "READ_tk"));
  keyword_map.insert(make_pair("print", "PRINT_tk"));
  keyword_map.insert(make_pair("program", "PROGRAM_tk"));
  keyword_map.insert(make_pair("cond", "COND_tk"));
  keyword_map.insert(make_pair("then", "THEN_tk"));
  keyword_map.insert(make_pair("let", "LET_tk"));
}

//line number token error 
void display_token(Token token)
{
  cout << "Line number: " << token.line_number << " " << token_names[token.ID] 
    << " " << token.desc << "\n";
}


void get_operator(Token &token)
{
  token.desc.assign(operator_map[token.desc]);
}

int is_operator(char ch)
{
  for (int counter = 0; counter < NUM_OPERATORS; counter++)
  {
	//get operators 
    if (ch == operators[counter])
      return 1;
  }
  return 0;
}

//check for keyword function
int is_keyword(Token &token)
{
	
  for (int counter = 0; counter < NUM_KEYWORDS; counter++)
  {
    if (token.desc == keywords[counter])
    {
      token.desc = keyword_map[token.desc];
      return counter;
    }
  } 
  return -1;
}

//get token ID
string get_token_desc(Token token)
{
  size_t found = token.desc.find("ID_tk");
  string desc = token.desc;

  if (found != string::npos)
    desc.erase(found, 5);
  
  return desc;
}

//get token INT
string get_token_int(Token token)
{
  size_t found = token.desc.find("INT_tk");
  string desc = token.desc;

  if (found != string::npos)
    desc.erase(found, 6);

  return desc;
}