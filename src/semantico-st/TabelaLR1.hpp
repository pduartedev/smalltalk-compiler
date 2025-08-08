#ifndef _TABELALR1_HPP_
#define _TABELALR1_HPP_
#include<vector>
#include<string>
#include<iostream>
#include<fstream>   
#include<sstream>
#include<map>
using namespace std;

class Transicao {
public:
  int tipo; // 0-vazio, 1 - shift, 2 - goto, 3-reduz, 4-aceita, 5-especial-LR2
  int reducao; // qual regra no vetor de gramatica
  int prox_estado;
  map<string,Transicao> mapa_LR2;
  string impressao();
  Transicao(string tok); 
  Transicao();
private:
  void inicializa(string tok);
};

class Tabela_LR1 {
public:
  vector<map<string,Transicao> > Tab;
  void debug();
  Tabela_LR1(ifstream &arq_tabela_lr1);
};

#endif
