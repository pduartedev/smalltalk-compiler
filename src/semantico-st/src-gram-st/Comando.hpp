#ifndef _COMANDO_HPP_
#define _COMANDO_HPP_
#include<vector>
#include<map>
#include "../Arvore.hpp"
using namespace std;

class Variavel;
class ComandoLista; // Forward declaration
class ComandoWhile; // Forward declaration
class Expressao; // Forward declaration

class Comando {
public:
  static vector<Comando*> extrai_lista_comandos(No_arv_parse* no);
  static Comando* extrai_comando(No_arv_parse* no);
  static Comando* extrai_comando_de_expression(No_arv_parse* no);
  static ComandoWhile* extrai_comando_while(No_arv_parse* no);
  static Expressao* extrair_expressao_do_bloco(No_arv_parse* block_constructor);
  static vector<Comando*> extrair_comandos_do_bloco_while(No_arv_parse* no);
  static void extrai_block_content(No_arv_parse* no, ComandoLista* cmd);
  static vector<Comando*> extrai_block_statements(No_arv_parse* no);
  static vector<Comando*> extrai_block_statement_list(No_arv_parse* no);
  virtual void debug_com_tab(int tab);
  virtual bool verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros);
  virtual double executar(map<string, double>& valores_variaveis) = 0;
};

#endif
