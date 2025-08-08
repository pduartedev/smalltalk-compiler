#ifndef _EXPRESSAO_HPP_
#define _EXPRESSAO_HPP_
#include "../Arvore.hpp"
#include "Tipo.hpp"
#include <vector>
#include <map>
using namespace std;

class Variavel;

class Expressao {
public:
  Tipo* tipo_resultado;
  
  static Expressao* extrai_expressao(No_arv_parse* no);
  static Expressao* extrai_basic_expression(No_arv_parse* no);
  static Expressao* extrai_primary(No_arv_parse* no);
  static Expressao* extrai_literal(No_arv_parse* no);
  static Expressao* extrai_number_literal(No_arv_parse* no);
  static Expressao* extrai_message_sequence(Expressao* primary, No_arv_parse* no);
  static Expressao* extrai_message_chain(Expressao* primary, No_arv_parse* no);
  static Expressao* extrai_unary_message_chain(Expressao* primary, No_arv_parse* no);
  static Expressao* extrai_unary_message_list(Expressao* primary, No_arv_parse* no);
  static string extrai_unary_message(No_arv_parse* no);
  static Expressao* extrai_binary_message_chain(Expressao* primary, No_arv_parse* no);
  static Expressao* extrai_binary_message_list(Expressao* primary, No_arv_parse* no);
  static Expressao* extrai_binary_message(Expressao* primary, No_arv_parse* no);
  static Expressao* extrai_keyword_message(Expressao* primary, No_arv_parse* no);
  static string extrai_binary_selector(No_arv_parse* no);
  static Expressao* extrai_binary_argument(No_arv_parse* no);
  static string extrai_binary_selector_from_message(No_arv_parse* no);
  static Expressao* extrai_binary_argument_from_message(No_arv_parse* no);
  static Expressao* aplica_precedencia_operadores(Expressao* primary, const vector<pair<string, Expressao*>>& operacoes);
  virtual void debug_com_tab(int tab);
  virtual Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros);
  virtual double calcular_valor(const map<string, double>& valores_variaveis) = 0;
};

#endif
