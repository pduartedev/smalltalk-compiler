#ifndef _EXPRESSAO_NUMERO_HPP_
#define _EXPRESSAO_NUMERO_HPP_
#include "Expressao.hpp"
#include "NUM.hpp"
using namespace std;

class ExpressaoNumero : public Expressao {
public:
  NUM* numero;
  ExpressaoNumero();
  void debug_com_tab(int tab) override;
  Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
  double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
