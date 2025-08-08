#include "ExpressaoChamadaFuncao.hpp"
#include "../debug-util.hpp"
#include <iostream>

using namespace std;

ExpressaoChamadaFuncao::ExpressaoChamadaFuncao() {
  nome_funcao = nullptr;
  tipo_resultado = Tipo::OBJECT_TYPE();
}

ExpressaoChamadaFuncao::ExpressaoChamadaFuncao(ID* nome, vector<Expressao*> params) {
  nome_funcao = nome;
  parametros = params;
  tipo_resultado = Tipo::OBJECT_TYPE();
}

ExpressaoChamadaFuncao* ExpressaoChamadaFuncao::extrai_chamada_funcao(No_arv_parse* no) {
  if (no == nullptr) return nullptr;
  
  // Verifica se é uma Keyword_Message que pode representar uma chamada de função
  if (no->simb == "Keyword_Message" && no->filhos.size() >= 2) {
    // Keyword_Message -> Keyword_Message_Element Keyword_Message_Element_List
    
    // Primeiro, constrói o nome completo da função a partir dos keywords
    string nome_completo = "";
    vector<Expressao*> parametros_extraidos;
    
    // Processa o primeiro elemento
    if (no->filhos[0]->simb == "Keyword_Message_Element") {
      No_arv_parse* primeiro_elemento = no->filhos[0];
      if (primeiro_elemento->filhos.size() >= 2) {
        // Keyword_Message_Element -> TOKEN_keyword Keyword_Argument
        if (primeiro_elemento->filhos[0]->simb == "TOKEN_keyword") {
          nome_completo += primeiro_elemento->filhos[0]->dado_extra;
        }
        
        // Extrai o argumento (parâmetro)
        Expressao* param = nullptr;
        
        // Keyword_Argument -> Primary
        if (primeiro_elemento->filhos[1]->simb == "Keyword_Argument" && 
            primeiro_elemento->filhos[1]->filhos.size() > 0) {
          // Extrai a expressão do Primary dentro do Keyword_Argument
          param = Expressao::extrai_expressao(primeiro_elemento->filhos[1]->filhos[0]);
        } else {
          param = Expressao::extrai_expressao(primeiro_elemento->filhos[1]);
        }
        
        if (param != nullptr) {
          parametros_extraidos.push_back(param);
        }
      }
    }
    
    // Processa elementos adicionais da lista
    if (no->filhos.size() > 1 && no->filhos[1]->simb == "Keyword_Message_Element_List") {
      No_arv_parse* lista = no->filhos[1];
      cout << "DEBUG: Lista inicial tem " << lista->filhos.size() << " filhos" << endl;
      
      while (lista != nullptr && lista->filhos.size() > 0) {
        cout << "DEBUG: Processando lista com simbolo: " << lista->simb << ", filhos: " << lista->filhos.size() << endl;
        
        if (lista->filhos[0]->simb == "Keyword_Message_Element") {
          No_arv_parse* elemento = lista->filhos[0];
          cout << "DEBUG: Elemento tem " << elemento->filhos.size() << " filhos" << endl;
          
          if (elemento->filhos.size() >= 2) {
            // Adiciona ao nome da função
            if (elemento->filhos[0]->simb == "TOKEN_keyword") {
              nome_completo += elemento->filhos[0]->dado_extra;
              cout << "DEBUG: Adicionando keyword: " << elemento->filhos[0]->dado_extra << endl;
            }
            
            // Extrai o argumento
            cout << "DEBUG: Extraindo parametro do tipo: " << elemento->filhos[1]->simb << endl;
            Expressao* param = nullptr;
            
            // Keyword_Argument -> Primary
            if (elemento->filhos[1]->simb == "Keyword_Argument" && 
                elemento->filhos[1]->filhos.size() > 0) {
              // Extrai a expressão do Primary dentro do Keyword_Argument
              param = Expressao::extrai_expressao(elemento->filhos[1]->filhos[0]);
            } else {
              param = Expressao::extrai_expressao(elemento->filhos[1]);
            }
            
            if (param != nullptr) {
              parametros_extraidos.push_back(param);
              cout << "DEBUG: Parametro extraido com sucesso" << endl;
            } else {
              cout << "DEBUG: Falha ao extrair parametro" << endl;
            }
          }
        }
        
        // Avança para o próximo elemento da lista
        if (lista->filhos.size() > 1) {
          cout << "DEBUG: Avancando para proximo elemento da lista" << endl;
          lista = lista->filhos[1];
        } else {
          cout << "DEBUG: Fim da lista" << endl;
          break;
        }
      }
    }
    
    // Cria a chamada de função se temos nome e parâmetros
    if (!nome_completo.empty()) {
      ID* nome_id = new ID();
      nome_id->nome = nome_completo;
      return new ExpressaoChamadaFuncao(nome_id, parametros_extraidos);
    }
  }
  
  return nullptr;
}

vector<Expressao*> ExpressaoChamadaFuncao::extrai_lista_parametros(No_arv_parse* no) {
  vector<Expressao*> resultado;
  
  // Esta função pode ser usada para extrair listas de parâmetros de outros contextos
  // Por enquanto, a extração é feita na função extrai_chamada_funcao
  
  return resultado;
}

void ExpressaoChamadaFuncao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Chamada de funcao: " << (nome_funcao ? nome_funcao->nome : "NULL") << "(";
  for (size_t i = 0; i < parametros.size(); i++) {
    if (i > 0) cerr << ", ";
    cerr << "param" << i;
  }
  cerr << ")" << endl;
  
  for (Expressao* param : parametros) {
    if (param != nullptr) {
      param->debug_com_tab(tab + 1);
    }
  }
}

Tipo* ExpressaoChamadaFuncao::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros_func) {
  // Verifica os tipos dos parâmetros
  for (Expressao* param : parametros) {
    if (param != nullptr) {
      param->verificar_tipos(variaveis, parametros_func);
    }
  }
  
  // Por padrão, assume que a função retorna OBJECT_TYPE
  tipo_resultado = Tipo::OBJECT_TYPE();
  return tipo_resultado;
}

double ExpressaoChamadaFuncao::calcular_valor(const map<string, double>& valores_variaveis) {
  // Para análise semântica, retorna um valor padrão
  // Em uma implementação real, isso executaria a função
  return 0.0;
}
