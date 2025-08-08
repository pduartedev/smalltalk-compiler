#include "FrameFuncao.hpp"
#include "FrameAcessoNoFrame.hpp"
#include "FrameAcessoTemp.hpp"
#include "../src-gram-st/Variavel.hpp"
#include "../src-gram-st/Comando.hpp"
#include "../src-gram-st/ComandoAtribuicao.hpp"
#include "../src-gram-st/ComandoIf.hpp"
#include "../src-gram-st/ComandoWhile.hpp"
#include "../src-gram-st/ComandoLista.hpp"
#include "../src-gram-st/Expressao.hpp"
#include "../src-gram-st/ExpressaoVariavel.hpp"
#include "../src-gram-st/ExpressaoChamadaFuncao.hpp"
#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

FrameFuncao::FrameFuncao() { 
  tamanho_frame = 0;
  n_param_entrada = 0;
  n_maximo_param_saida = 0;
  n_pseudo_registradores = 0;
  n_variaveis_no_frame = 0;
}

FrameFuncao* FrameFuncao::gera_frame_de_funcao(Funcao* fun) {
  if (fun == nullptr) return nullptr;
  
  string nome_funcao = (fun->nome_funcao && fun->nome_funcao->nome.length() > 0) ? 
                       fun->nome_funcao->nome : "funcao_anonima";
  cout << "Gerando frame para funcao: " << nome_funcao << endl;
  
  FrameFuncao* frame = new FrameFuncao();
  
  // 1. Configurar parâmetros de entrada
  configurar_parametros_entrada(fun, frame);
  
  // 2. Encontrar o máximo de parâmetros de saída
  frame->n_maximo_param_saida = encontrar_maximo_parametros_saida(fun);
  
  // 3. Encontrar variáveis que aparecem em parâmetros de chamadas de função
  set<string> variaveis_em_parametros = encontrar_variaveis_em_parametros_de_chamadas(fun);
  
  // 4. Alocar variáveis considerando se aparecem em parâmetros
  alocar_variaveis_internas(fun, frame);
  
  // 5. Calcular tamanho total do frame
  frame->tamanho_frame = 40 + (frame->n_variaveis_no_frame * 8); // 40 bytes iniciais + variáveis
  
  return frame;
}

void FrameFuncao::configurar_parametros_entrada(Funcao* fun, FrameFuncao* frame) {
  frame->n_param_entrada = fun->parametros.size();
  cout << "Configurando " << frame->n_param_entrada << " parametros de entrada" << endl;
  
  int posicao_param = 8; // Primeira posição de parâmetro: FP+8
  
  // Configura posições dos parâmetros no frame anterior
  for (Variavel* param : fun->parametros) {
    if (param != nullptr) {
      param->acesso_frame = new FrameAcessoNoFrame(posicao_param);
      posicao_param += 8; // Próximo parâmetro: +8, +16, +24, etc.
    }
  }
}

int FrameFuncao::encontrar_maximo_parametros_saida(Funcao* fun) {
  int max_params = 0;
  set<string> vars_dummy;
  
  for (Comando* cmd : fun->comandos) {
    analisar_comando_para_chamadas(cmd, max_params, vars_dummy);
  }
  
  cout << "Maximo de parametros de saida encontrado: " << max_params << endl;
  return max_params;
}

set<string> FrameFuncao::encontrar_variaveis_em_parametros_de_chamadas(Funcao* fun) {
  set<string> variaveis_em_parametros;
  int max_dummy = 0;
  
  for (Comando* cmd : fun->comandos) {
    analisar_comando_para_chamadas(cmd, max_dummy, variaveis_em_parametros);
  }
  
  cout << "Variaveis encontradas em parametros de chamadas:" << endl;
  for (const string& var : variaveis_em_parametros) {
    cout << "  - " << var << endl;
  }
  
  return variaveis_em_parametros;
}

void FrameFuncao::analisar_comando_para_chamadas(Comando* cmd, int& max_params, set<string>& vars_em_params) {
  if (!cmd) return;
  
  ComandoAtribuicao* cmdAtrib = dynamic_cast<ComandoAtribuicao*>(cmd);
  if (cmdAtrib) {
    analisar_expressao_para_chamadas(cmdAtrib->direita, max_params, vars_em_params);
    return;
  }
  
  ComandoLista* cmdLista = dynamic_cast<ComandoLista*>(cmd);
  if (cmdLista) {
    for (Comando* subcmd : cmdLista->lista_comandos) {
      analisar_comando_para_chamadas(subcmd, max_params, vars_em_params);
    }
    return;
  }
  
  ComandoIf* cmdIf = dynamic_cast<ComandoIf*>(cmd);
  if (cmdIf) {
    analisar_expressao_para_chamadas(cmdIf->condicao, max_params, vars_em_params);
    for (Comando* subcmd : cmdIf->comandos_then) {
      analisar_comando_para_chamadas(subcmd, max_params, vars_em_params);
    }
    if (cmdIf->tem_else) {
      for (Comando* subcmd : cmdIf->comandos_else) {
        analisar_comando_para_chamadas(subcmd, max_params, vars_em_params);
      }
    }
    return;
  }
  
  ComandoWhile* cmdWhile = dynamic_cast<ComandoWhile*>(cmd);
  if (cmdWhile) {
    // Assumindo que ComandoWhile tem campos similares
    // Implementar quando necessário
    return;
  }
}

void FrameFuncao::analisar_expressao_para_chamadas(Expressao* expr, int& max_params, set<string>& vars_em_params) {
  if (!expr) return;
  
  ExpressaoChamadaFuncao* chamada = dynamic_cast<ExpressaoChamadaFuncao*>(expr);
  if (chamada) {
    string nome_chamada = (chamada->nome_funcao && chamada->nome_funcao->nome.length() > 0) ? 
                          chamada->nome_funcao->nome : "funcao_anonima";
    cout << "Analisando chamada de funcao: " << nome_chamada << endl;
    cout << "  DEBUG: Chamada tem " << chamada->parametros.size() << " parametros" << endl;
    
    // Contar parâmetros desta chamada
    int num_params = chamada->parametros.size();
    max_params = max(max_params, num_params);
    
    // Coletar variáveis nos parâmetros
    for (size_t i = 0; i < chamada->parametros.size(); i++) {
      cout << "  DEBUG: Analisando parametro " << i << endl;
      coletar_variaveis_em_expressao(chamada->parametros[i], vars_em_params);
    }
    
    // Analisar recursivamente os parâmetros para chamadas aninhadas
    for (Expressao* param : chamada->parametros) {
      analisar_expressao_para_chamadas(param, max_params, vars_em_params);
    }
  }
}

void FrameFuncao::coletar_variaveis_em_expressao(Expressao* expr, set<string>& vars) {
  if (!expr) {
    cout << "  DEBUG: Expressao nula" << endl;
    return;
  }
  
  cout << "  DEBUG: Analisando tipo de expressao: " << typeid(*expr).name() << endl;
  
  ExpressaoVariavel* exprVar = dynamic_cast<ExpressaoVariavel*>(expr);
  if (exprVar && exprVar->nome && exprVar->nome->nome.length() > 0) {
    vars.insert(exprVar->nome->nome);
    cout << "  Coletada variavel em parametro: " << exprVar->nome->nome << endl;
    return;
  }
  
  // Para outras expressões compostas, analisar recursivamente
  // (implementar conforme necessário para expressões binárias, etc.)
  cout << "  DEBUG: Nao eh ExpressaoVariavel" << endl;
}

void FrameFuncao::alocar_variaveis_internas(Funcao* fun, FrameFuncao* frame) {
  cout << "Analisando " << fun->variaveis.size() << " variaveis internas" << endl;
  
  // Encontrar variáveis que aparecem em parâmetros de chamadas
  set<string> variaveis_em_parametros = encontrar_variaveis_em_parametros_de_chamadas(fun);
  
  int posicao_frame_atual = -40; // Primeira posição no frame para variáveis
  int id_pseudo_reg_atual = 1;   // ID do próximo pseudo-registrador
  
  // Processa variáveis locais (não parâmetros)
  for (Variavel* var : fun->variaveis) {
    if (var == nullptr || var->nome == nullptr) continue;
    
    cout << "  Analisando variavel: " << var->nome->nome << endl;
    
    // Verificar se a variável aparece em parâmetros de chamadas
    bool aparece_em_parametros = variaveis_em_parametros.find(var->nome->nome) != variaveis_em_parametros.end();
    
    if (aparece_em_parametros) {
      // Deve ir para o frame
      var->acesso_frame = new FrameAcessoNoFrame(posicao_frame_atual);
      posicao_frame_atual -= 8; // Próxima posição no frame
      frame->n_variaveis_no_frame++;
      cout << "    -> Alocada no frame (aparece em parametros)" << endl;
    } else {
      // Pode ir para pseudo-registrador
      var->acesso_frame = new FrameAcessoTemp(id_pseudo_reg_atual);
      id_pseudo_reg_atual++;
      frame->n_pseudo_registradores++;
      cout << "    -> Alocada em pseudo-registrador" << endl;
    }
  }
}
