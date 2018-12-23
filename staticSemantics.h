//staticSemantics.h

#ifndef STATICSEMANTICS_H
#define STATICSEMANTICS_H

#include <iostream>
#include <fstream>
#include <vector>
#include "token.h"
#include "node.h"

using namespace std;

extern ofstream target_file;

struct stack_t 
{  
  Token token;
  int scope;
  int line_number;
};

//vector stack
extern vector<stack_t> stack;


//traverse tree and parse tree functions
void staticSemantics(node_t*);
void check_var(stack_t);
int check_var_exists(stack_t);
int find_var(stack_t);
int find_on_stack(stack_t);
int compare_scope(stack_t);
void remove_local_vars(int);
void print_stack();

#endif
