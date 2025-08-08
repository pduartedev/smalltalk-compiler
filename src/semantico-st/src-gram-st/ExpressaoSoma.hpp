#ifndef _EXPRESSAO_SOMA_HPP_
#define _EXPRESSAO_SOMA_HPP_
#include "Expressao.hpp"
using namespace std;

class ExpressaoSoma : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  ExpressaoSoma();
  void debug_com_tab(int tab) override;
  Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
  double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
