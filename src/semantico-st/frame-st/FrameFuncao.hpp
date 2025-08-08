#ifndef _FRAME_FUNCAO_HPP_
#define _FRAME_FUNCAO_HPP_
#include "../src-gram-st/Funcao.hpp"
#include "../src-gram-st/Comando.hpp"
#include "../src-gram-st/Expressao.hpp"
#include <set>
using namespace std;

class FrameFuncao {
public:
  int tamanho_frame;
  int n_param_entrada;
  int n_maximo_param_saida;
  int n_pseudo_registradores;
  int n_variaveis_no_frame;

  FrameFuncao();
  static FrameFuncao* gera_frame_de_funcao(Funcao* fun);
  
private:
  static void alocar_variaveis_internas(Funcao* fun, FrameFuncao* frame);
  static void configurar_parametros_entrada(Funcao* fun, FrameFuncao* frame);
  static int encontrar_maximo_parametros_saida(Funcao* fun);
  static set<string> encontrar_variaveis_em_parametros_de_chamadas(Funcao* fun);
  static void analisar_comando_para_chamadas(Comando* cmd, int& max_params, set<string>& vars_em_params);
  static void analisar_expressao_para_chamadas(Expressao* expr, int& max_params, set<string>& vars_em_params);
  static void coletar_variaveis_em_expressao(Expressao* expr, set<string>& vars);
};

#endif
