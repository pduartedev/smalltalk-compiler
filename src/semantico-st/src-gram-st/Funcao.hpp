#ifndef _FUNCAO_HPP_
#define _FUNCAO_HPP_
#include<vector>
#include<string>
#include "ID.hpp"
#include "Variavel.hpp"  
#include "Comando.hpp"
#include "../Arvore.hpp"
using namespace std;

class Funcao {
public:
  ID* tipo_retorno;
  ID* nome_funcao;
  vector<Variavel*> parametros;
  vector<Variavel*> variaveis;
  vector<Comando*> comandos;

  Funcao();
  static Funcao* extrai_funcao(No_arv_parse *arv);
  static vector<Funcao*> extrai_todas_funcoes(No_arv_parse *arv);
  static Funcao* extrai_funcao_recursivo(No_arv_parse *no);
  static void extrai_todas_funcoes_recursivo(No_arv_parse *no, vector<Funcao*>& funcoes);
  static Funcao* criar_funcao_implicita(No_arv_parse *no);
  static Funcao* criar_funcao_statement_simples(No_arv_parse *no);
  static void buscar_statements_nivel_superior(No_arv_parse *no, vector<Comando*> &comandos);
  static void extrai_message_pattern(No_arv_parse *no, Funcao* func);
  static void extrai_keyword_elements(No_arv_parse *no, Funcao* func);
  static void extrai_keyword_pattern_elementos(No_arv_parse *no, Funcao* func, string& nome_completo);
  static vector<Variavel*> extrai_temporaries(No_arv_parse *no);
  static vector<Variavel*> extrai_variable_list(No_arv_parse *no);
  static vector<Variavel*> extrai_variaveis_instancia(No_arv_parse *no_raiz);
  static No_arv_parse* buscar_no_por_simbolo(No_arv_parse *no, const string& simbolo);
  static vector<Comando*> extrai_statements(No_arv_parse *no);
  static Comando* extrai_statement(No_arv_parse *no);
  static Comando* extrai_return_statement(No_arv_parse *no);
  static Comando* extrai_expression_command(No_arv_parse *no);
  static vector<Comando*> extrai_statement_list(No_arv_parse *no);
  void debug();
};

#endif
