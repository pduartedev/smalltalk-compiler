#ifndef _EXPRESSAO_BINARIA_HPP_
#define _EXPRESSAO_BINARIA_HPP_
#include "Expressao.hpp"
#include <string>
using namespace std;

class ExpressaoBinaria : public Expressao {
public:
  Expressao* esquerda;
  string operador;
  Expressao* direita;
  
  ExpressaoBinaria(Expressao* esq, const string& op, Expressao* dir);
  virtual ~ExpressaoBinaria();
  virtual void debug_com_tab(int tab);
  virtual Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros);
  virtual double calcular_valor(const map<string, double>& valores_variaveis);
};

#endif
