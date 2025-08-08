#include <algorithm>
#include<vector>
#include<string>
#include<iostream>
#include <fstream>   
#include<sstream>
#include<map>
#include<stack>
using namespace std;

#include "Gramatica.hpp"
#include "TabelaLR1.hpp"
#include "Arvore.hpp"
#include "Parser.hpp"
#include "src-gram-st/Funcao.hpp"
#include "src-gram-st/ComandoAtribuicao.hpp"
#include "src-gram-st/Analisador.hpp"
#include "frame-st/FrameFuncao.hpp"
#include "frame-st/FrameAcessoNoFrame.hpp"
#include "frame-st/FrameAcessoTemp.hpp"
#include "GeradorRI.hpp"


int main(int argc, char * argv[]) {
  if (argc != 3 && argc != 1) {
    cerr << "Parametros nomes dos arquivos: 1) csv com gramática e 2) csv com tabela LR1" << endl;
    return 1;
  }
  string nome_gramatica, nome_tab_lr1;
  if (argc == 1) {
    //cerr << "Valores padrao utilizados: gramatica.conf e tabela_lr1.conf" << endl;
    nome_gramatica = string("gramatica.conf");
    nome_tab_lr1 = string("tabela_lr1.conf");
  } else {
    nome_gramatica = string(argv[1]);
    nome_tab_lr1 = string(argv[2]);
  }

  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  if (arq_tabela_lr1.fail() || arq_gramatica.fail()) {
    cerr << "Falha ao abrir arquivos: " << 
      ((arq_gramatica.fail()) ? nome_gramatica : "") << ", " << 
      ((arq_tabela_lr1.fail()) ? nome_tab_lr1 : "") << endl;
    return 1;    
  }
  Parser parser(arq_gramatica, arq_tabela_lr1);
  parser.tabela.debug();
  parser.gram.debug();

  
  Arvore_parse arv = parser.executa_parse(cin);
  cerr << "Parse executado" << endl;
  arv.debug();
  
  // Extrair todas as funções da classe
  vector<Funcao*> funcoes = Funcao::extrai_todas_funcoes(arv.raiz);
  
  if (funcoes.empty()) {
    cerr << "Erro: Não foi possível extrair nenhuma função da árvore de parse" << endl;
    return 1;
  }
  
  cout << "=== ENCONTRADAS " << funcoes.size() << " FUNÇÕES ===" << endl;
  
  // Processar cada função encontrada
  for (size_t i = 0; i < funcoes.size(); i++) {
    Funcao* func = funcoes[i];
    
    cout << "\n=== PROCESSANDO FUNÇÃO " << (i+1) << " ===" << endl;
    func->debug();
    
    // Gerar descrição do frame para a função
    FrameFuncao* frame = FrameFuncao::gera_frame_de_funcao(func);
    
    if (frame == nullptr) {
      cerr << "Erro: Não foi possível gerar o frame da função " << (i+1) << endl;
      continue;
    }
    
    // Exibir informações do frame
    cout << "=== INFORMAÇÕES DO FRAME ===" << endl;
    cout << "Tamanho total do frame: " << frame->tamanho_frame << " bytes" << endl;
    cout << "Número de parâmetros de entrada: " << frame->n_param_entrada << endl;
    cout << "Número máximo de parâmetros de saída: " << frame->n_maximo_param_saida << endl;
    cout << "Número de pseudo-registradores: " << frame->n_pseudo_registradores << endl;
    cout << "Número de variáveis no frame: " << frame->n_variaveis_no_frame << endl;
    cout << endl;
    
    // Exibir mapeamento de variáveis
    cout << "=== MAPEAMENTO DE VARIÁVEIS ===" << endl;
    cout << "Parâmetros de entrada:" << endl;
    for (size_t j = 0; j < func->parametros.size(); j++) {
      Variavel* param = func->parametros[j];
      if (param != nullptr && param->nome != nullptr && param->acesso_frame != nullptr) {
        FrameAcessoNoFrame* acesso = dynamic_cast<FrameAcessoNoFrame*>(param->acesso_frame);
        if (acesso != nullptr) {
          cout << "  " << param->nome->nome << ": FP+" << acesso->posicao_no_frame << endl;
        }
      }
    }
    
    cout << "Variáveis locais:" << endl;
    for (Variavel* var : func->variaveis) {
      if (var != nullptr && var->nome != nullptr && var->acesso_frame != nullptr) {
        FrameAcessoNoFrame* acesso_frame = dynamic_cast<FrameAcessoNoFrame*>(var->acesso_frame);
        FrameAcessoTemp* acesso_temp = dynamic_cast<FrameAcessoTemp*>(var->acesso_frame);
        
        if (acesso_frame != nullptr) {
          cout << "  " << var->nome->nome << ": FP" << acesso_frame->posicao_no_frame << " (no frame)" << endl;
        } else if (acesso_temp != nullptr) {
          cout << "  " << var->nome->nome << ": T" << acesso_temp->id << " (pseudo-registrador)" << endl;
        }
      }
    }
    cout << endl;
    
    // Executar análise semântica
    Analisador ana;
    if (!ana.verificar_semantica(func)) {
      cerr << "Erro na análise semântica da função " << (i+1) << ". Pulando..." << endl;
      continue;
    }
    
    cerr << "Análise semântica da função " << (i+1) << " concluída com sucesso!" << endl;
    
    // Exemplo de chamada do analisador semantico com parâmetros de exemplo
    vector<double> parametros_passados;
    
    // Usar valores padrão para parâmetros (se existirem)
    for (size_t j = 0; j < func->parametros.size(); j++) {
      parametros_passados.push_back(j + 1); // Valor padrão
    }
    
    double resultado = ana.calcula_ultimo_valor(func, parametros_passados);
    
    // Determinar o tipo do resultado baseado no último comando
    Tipo* tipo_resultado = Tipo::INTEGER_TYPE(); // Padrão
    if (!func->comandos.empty()) {
      // Tentar obter o tipo do último comando
      Comando* ultimo_cmd = func->comandos.back();
      cerr << "DEBUG: Tipo do último comando: " << typeid(*ultimo_cmd).name() << endl;
      
      // Se for uma atribuição, obter o tipo da expressão à direita
      ComandoAtribuicao* atribuicao = dynamic_cast<ComandoAtribuicao*>(ultimo_cmd);
      if (atribuicao != nullptr) {
        cerr << "DEBUG: É uma atribuição para variável: " << (atribuicao->esquerda ? atribuicao->esquerda->nome : "NULL") << endl;
        if (atribuicao->direita != nullptr && atribuicao->direita->tipo_resultado != nullptr) {
          cerr << "DEBUG: Tipo da expressão à direita: " << (int)atribuicao->direita->tipo_resultado->tipo << endl;
          tipo_resultado = atribuicao->direita->tipo_resultado;
          
          // Verificar se é especificamente o comando de return (atribuição à variável __return__)
          if (atribuicao->esquerda != nullptr && atribuicao->esquerda->nome == "__return__") {
            cerr << "DEBUG: Detectado comando de return!" << endl;
            tipo_resultado = atribuicao->direita->tipo_resultado;
          }
        }
      }
    }
    cerr << "DEBUG: Tipo final determinado: " << (int)tipo_resultado->tipo << endl;
    
    cout << "Ultimo valor calculado: ";
    ana.imprimir_resultado(resultado, tipo_resultado);
    cout << endl;
  }
  
  // =================== FASE DE REPRESENTAÇÃO INTERMEDIÁRIA ===================
  cout << "\n\n========== INICIANDO GERAÇÃO DE REPRESENTAÇÃO INTERMEDIÁRIA ==========\n";
  
  // Inicializar gerador de RI
  Analisador analisador_global;
  GeradorRI geradorRI(&analisador_global);
  
  // Processar cada função para gerar RI
  for (size_t i = 0; i < funcoes.size(); i++) {
    Funcao* func = funcoes[i];
    
    cout << "\n---------- FUNÇÃO " << (i+1) << ": " << (func->nome ? func->nome->nome : "ANÔNIMA") << " ----------\n";
    
    try {
      // Gerar representação intermediária
      shared_ptr<Stm> ri = geradorRI.converterFuncao(func);
      
      if (ri != nullptr) {
        cout << "Representação Intermediária gerada:\n";
        ri->imprimir(0);
      } else {
        cout << "Erro: Não foi possível gerar RI para esta função.\n";
      }
    } catch (const exception& e) {
      cout << "Erro na geração de RI: " << e.what() << "\n";
    }
    
    cout << "\n";
  }
  
  cout << "========== GERAÇÃO DE REPRESENTAÇÃO INTERMEDIÁRIA CONCLUÍDA ==========\n\n";
  
  return 0;
}
