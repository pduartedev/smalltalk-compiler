#ifndef _COMANDO_ATRIBUI_HPP_
#define _COMANDO_ATRIBUI_HPP_
#include "Comando.hpp"
#include "ID.hpp"
#include "Expressao.hpp"
using namespace std;

class ComandoAtribuicao : public Comando {
public:
  ID * esquerda;
  Expressao * direita;
  ComandoAtribuicao();
  void debug_com_tab(int tab) override;
  bool verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
  double executar(map<string, double>& valores_variaveis) override;
};

#endif
