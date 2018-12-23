#ifndef NODE_H
#define NODE_H

#include <set>
#include <vector>
#include <string>

#include "token.h"

using namespace std;

struct node_t
{
 
  string label;
  int token_length; 
  int level; 

  vector<Token> tokens;

  struct node_t *child_1,
  *child_2,
  *child_3,
  *child_4;
};

#endif
 