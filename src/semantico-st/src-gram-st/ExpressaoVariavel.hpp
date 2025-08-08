#ifndef _EXPRESSAO_VARIAVEL_HPP_
#define _EXPRESSAO_VARIAVEL_HPP_
#include "Expressao.hpp"
#include "ID.hpp"
using namespace std;

class ExpressaoVariavel : public Expressao {
public:
  ID* nome;
  ExpressaoVariavel();
  void debug_com_tab(int tab) override;
  Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
  double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
