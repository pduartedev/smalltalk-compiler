#ifndef _GRAMATICA_HPP_
#define _GRAMATICA_HPP_
#include<vector>
#include<string>
#include<iostream>
#include<fstream>   
#include<sstream>
using namespace std;

class Regra {
public:
  string esq;
  vector<string> dir;
  void debug();
};

class Gramatica {
public:
  vector<Regra> R;
  Gramatica(ifstream &arq_gramatica);
  void debug();
};

#endif
