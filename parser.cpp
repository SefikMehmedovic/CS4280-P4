#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "node.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

Token tk;
static string expected_token;
static Token EMPTY_TOKEN;

node_t *parser()
{
  EMPTY_TOKEN.desc = "EMPTY";
  scanner(tk);
  
	//root node
  node_t *root = NULL;

  root = program();

	//check if EOF
  if (tk.ID != EOFtk)
  {
    expected_token.assign("EOF");
    parser_error();
  }
  else
  {
    return root;
  }
}

//<program>->void <vars> <block>
node_t *program()
{ 
  node_t *node = create_node("<program>");

  if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "VOID_tk"))
  {
    scanner(tk);
    node->child_1 = vars();
    node->child_2 = block();
    return node;
  }
  else
  {
    expected_token.assign("void");
    parser_error();
  }
}

//<vars> -> empty | let Identifier = Integer <vars> 
node_t *vars()
{
  node_t *node = create_node("<vars>");

  if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "LET_tk"))
  {
    scanner(tk);
    if (tk.ID == ID_tk)
    {
      node->tokens.push_back(tk);
      scanner(tk);

      if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
      {
        scanner(tk);

        if (tk.ID == INT_tk)
        {
          node->tokens.push_back(tk);
          scanner(tk);
          node->child_1 = vars();

          return node;
        }
        else
        {
          expected_token.assign("Integer");
          parser_error();
        }
      }
      else
      {
        expected_token.assign("=");
        parser_error();
      }
    }
    else
    {
      expected_token.assign("Identifier");
      parser_error();
    }
  }
  else
  {
    node->tokens.push_back(EMPTY_TOKEN);
    return node;
  }
}

//------------------
//<block> -> begin <vars> <stats> end 
node_t *block()
{
  node_t *node = create_node("<block>");
  
  if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "BEGIN_tk"))
  {
    scanner(tk);
    node->child_1 = vars();
    node->child_2 = stats();

    if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "END_tk"))
    {
      scanner(tk);
      return node;
    }
    else
    {
      expected_token.assign("end");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("begin");
    parser_error();
  }
}

//<expr> -> <A> / <expr> | <A> * <expr> | <A> 
node_t *expr()
{
  node_t *node = create_node("<expr>");
  node->child_1 = A();

  if (tk.ID == OPtk)
  {
    if (operator_map[tk.desc] == "SLASH_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);
      node->child_2 = expr();
    }
    else if (operator_map[tk.desc] == "ASTERIK_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);
      node->child_2 = expr();
    }
  }

  return node; 
}

//A -> <M> + <A> | <M> - <A> | <M> 
node_t *A()
{
  node_t *node = create_node("<A>");
  node->child_1 = M();

  if (tk.ID == OPtk)
  {
    if (operator_map[tk.desc] == "PLUS_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);
      node->child_2 = A();
    }
    else if (operator_map[tk.desc] == "MINUS_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);
      node->child_2 = A();
    }
  }
  
  return node;
}

//<M> -> -<M> | <R> 
node_t *M()
{
  node_t *node = create_node("<M>");
  
  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "MINUS_tk"))
  {
    node->tokens.push_back(tk);
    scanner(tk);
    node->child_1 = M();
    return node;
  }

  node->child_1 = R();
  return node;
}

//<R> -> (<expr>) | Identifier | Integer 
node_t *R()
{
  node_t *node = create_node("<R>");

  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
  {
    scanner(tk);
    node->child_1 = expr();

    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
    {
      scanner(tk);
      return node;
    }
    else
    {
      expected_token.assign(")");
      parser_error();
    }
  }
  else if (tk.ID == ID_tk)
  {
    node->tokens.push_back(tk);
    scanner(tk);
    return node;
  }
  else if (tk.ID == INT_tk)
  {
    node->tokens.push_back(tk);
    scanner(tk);
    return node;
  }
  else
  {
    expected_token.assign("( or Identifier or Integer");
    parser_error();
  }
}

//<stats> -> <stat> <mStat> 
node_t *stats()
{
  node_t *node = create_node("<stats>");
  node->child_1 = stat();
  node->child_2 = mStat(); 

  return node;
}

//<mStat> -> empty | <stat> <mStat> 
node_t *mStat()
{
  node_t *node = create_node("<mStat>");

  if ( ((tk.ID == KEYWORDtk)&& 
    ( (keyword_map[tk.desc] == "PRINT_tk") || (keyword_map[tk.desc] == "READ_tk")
      || (keyword_map[tk.desc] == "BEGIN_tk") 
      || (keyword_map[tk.desc] == "COND_tk") 
      || (keyword_map[tk.desc] == "ITER_tk"))) || (tk.ID == ID_tk))
  {
    node->child_1 = stat();
    node->child_2 = mStat();
    return node;
  }
  else
  {
    node->tokens.push_back(EMPTY_TOKEN);
    return node;
  }
}


//| <if> | <loop> | <assign> 
node_t *stat()
{
  node_t *node = create_node("<stat>");

  if (tk.ID == KEYWORDtk)
  {
    if (keyword_map[tk.desc] == "PRINT_tk")
    {
      scanner(tk);
      node->child_1 = out();

      return node;
    }
    else if (keyword_map[tk.desc] == "READ_tk")
    {
      scanner(tk);
      node->child_1 = in();
      return node;
    }
    else if (keyword_map[tk.desc] == "BEGIN_tk")
    {
      node->child_1 = block();
      return node;
    }
    else if (keyword_map[tk.desc] == "COND_tk")
    {
      scanner(tk);
      node->child_1 = cond();
      return node;
    }
    else if (keyword_map[tk.desc] == "ITER_tk")
    {
      scanner(tk);
      node->child_1 = loop();
      return node;
    }
  }
  else if (tk.ID == ID_tk)
  {
    Token temp_token = tk;
    scanner(tk);
    node->child_1 = assign();
    node->child_1->tokens.push_back(temp_token);
    return node;
  }
  else
  {
    expected_token.assign("read or print or cond or iter or Identifier");
    parser_error();
  }
}

//<in> -> read (Identifier): 
node_t *in()
{
  node_t *node = create_node("<in>");
  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
  {
    scanner(tk);

    if (tk.ID == ID_tk)
    {
      node->tokens.push_back(tk);
      scanner(tk);

      if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
      {
        scanner(tk);

        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "COLON_tk"))
        {
          scanner(tk);
          return node;
        }
        else
        {
          expected_token.assign(":");
          parser_error();
        }
      }
      else
      {
        expected_token.assign("(");
        parser_error();
      }
    }
    else
    {
      expected_token.assign("Identifier");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("(");
    parser_error();
  }
}

//<out> -> print(<expr>): 
node_t *out()
{
  node_t *node = create_node("<out>");
  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
  {
    scanner(tk);
    node->child_1 = expr();

    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
    {
      scanner(tk);
      if ((tk.ID == OPtk) && (operator_map[tk.desc] == "COLON_tk"))
      {
        scanner(tk);
        return node;
      }
      else
      {
        expected_token.assign(":");
        parser_error();
      }
    }
    else
    {
      expected_token.assign(")");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("(");
    parser_error();
  }
}

//<if> -> cond(<expr> <RO> <expr>) <stat> 
node_t *cond()
{
  node_t *node = create_node("<if>");
  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
  {
    scanner(tk);

    node->child_1 = expr();
    node->child_2 = RO();
    node->child_3 = expr();

    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
    {
      scanner(tk);
      node->child_4 = stat();

      return node;
    }
    else
    {
      expected_token.assign(")");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("(");
    parser_error();
  }
}

//<loop> -> iter (<expr> <RO> <expr>) <stat>
node_t *loop()
{
  node_t *node = create_node("<loop>");

  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
  {
    scanner(tk);
	
    node->child_1 = expr();
    node->child_2 = RO();
    node->child_3 = expr();

    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
    {
      scanner(tk);
      node->child_4 = stat();
      
      return node;
    }
    else
    {
      expected_token.assign(")");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("(");
    parser_error();
  }
}

//------------------------------------------
//<assign> -> Identifier = <expr>: 
node_t *assign()
{
  node_t *node = create_node("<assign>");

  if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
  {
    scanner(tk);
    node->child_1 = expr();

    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "COLON_tk"))
    {
      scanner(tk);
      return node;
    }
    else
    {
      expected_token.assign(":");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("=");
    parser_error();
  }
}

//<RO> -> < | < = | > | > = | = = | = 
node_t *RO()
{
  node_t *node = create_node("<RO>");
	//check for operator tokens
  if (tk.ID == OPtk)
  {
    if (operator_map[tk.desc] == "LESS_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);

      if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
      {
        node->tokens.push_back(tk);
        scanner(tk);
        return node;
      }
      else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "EQUALS_tk")
        && (operator_map[tk.desc] != "OPAR_tk"))
      {
        expected_token.assign("< =");
        parser_error();
      }
      else
        return node;
    }
    else if (operator_map[tk.desc] == "GREAT_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);

      if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
      {
        node->tokens.push_back(tk);
        scanner(tk);
        return node;
      }
      else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "EQUALS_tk")
        && (operator_map[tk.desc] != "OPAR_tk"))
      {
        expected_token.assign("> = ");
        parser_error();
      }
      else
        return node;
    }
    else if (operator_map[tk.desc] == "EQUALS_tk")
    {
      node->tokens.push_back(tk);
      scanner(tk);

      if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
      {
        node->tokens.push_back(tk);
        scanner(tk);
        return node;
      }
      else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "EQUALS_tk")
        && (operator_map[tk.desc] != "OPAR_tk"))
      {
        expected_token.assign("= =");
        parser_error();
      }
      else
        return node;
    }
    else
    {
      expected_token.assign("< or < = or > or > = or = or = = ");
      parser_error();
    }
  }
  else
  {
    expected_token.assign("< or < = or > or > = or = or = =");
    parser_error();
  }
}
//------------------
void parser_error()
{
  cout << "Error: parser - line number " << tk.line_number << 
    ": expected '" << expected_token << "' but received '" << tk.desc << "'\n";
  exit(EXIT_FAILURE);
}

//-----------------------------------------------
//create node function
node_t *create_node(string production_name)
{
  node_t *node = new node_t();
  node->child_1 = NULL;
  node->child_2 = NULL;
  node->child_3 = NULL;
  node->child_4 = NULL;

  node->label = production_name;

  return node;
}
