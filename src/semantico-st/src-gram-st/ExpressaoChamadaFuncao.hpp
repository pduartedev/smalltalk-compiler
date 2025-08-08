#ifndef _EXPRESSAO_CHAMADA_FUNCAO_HPP_
#define _EXPRESSAO_CHAMADA_FUNCAO_HPP_

#include "Expressao.hpp"
#include "ID.hpp"
#include <vector>

class ExpressaoChamadaFuncao : public Expressao {
public:
  ID* nome_funcao;
  vector<Expressao*> parametros;
  
  ExpressaoChamadaFuncao();
  ExpressaoChamadaFuncao(ID* nome, vector<Expressao*> params);
  
  static ExpressaoChamadaFuncao* extrai_chamada_funcao(No_arv_parse* no);
  static vector<Expressao*> extrai_lista_parametros(No_arv_parse* no);
  
  virtual void debug_com_tab(int tab) override;
  virtual Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
  virtual double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
