#ifndef _PARSER_HPP_
#define _PARSER_HPP_
#include<vector>
#include<string>
#include<stack>
#include<algorithm>
#include<iostream>
#include <fstream>   
#include<sstream>
using namespace std;

#include "Gramatica.hpp"
#include "TabelaLR1.hpp"
#include "Arvore.hpp"

class Gramatica;
class Tabela_LR1;

class Parser {
public:
  Gramatica gram;
  Tabela_LR1 tabela;
  stack<pair<No_arv_parse *,int> > pilha;
  Parser(ifstream &arq_gramatica, ifstream &arq_tabela_lr1);
  string to_upper(string str);
  Arvore_parse executa_parse(istream &input);
};

#endif
