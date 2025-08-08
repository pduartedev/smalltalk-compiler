#include "ComandoAtribuicao.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoAtribuicao::ComandoAtribuicao() {
  
}

void ComandoAtribuicao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "INI ATRIB: " << esquerda->nome << " = " << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "FIM ATRIB" << endl;
  cerr << endl;
}

bool ComandoAtribuicao::verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    // Primeiro verificar o tipo da expressão do lado direito
    Tipo* tipo_expressao = direita->verificar_tipos(variaveis, parametros);
    if (tipo_expressao == nullptr) {
        return false;
    }
    
    // Procurar se a variável já existe
    Tipo* tipo_variavel = nullptr;
    
    for (const auto& var : variaveis) {
        if (var->nome->nome == esquerda->nome) {
            tipo_variavel = var->tipo_semantico;
            break;
        }
    }
    
    if (tipo_variavel == nullptr) {
        for (const auto& param : parametros) {
            if (param->nome->nome == esquerda->nome) {
                tipo_variavel = param->tipo_semantico;
                break;
            }
        }
    }
    
    // Se a variável não existe, criar uma nova com o tipo da expressão (declaração implícita)
    if (tipo_variavel == nullptr) {
        cerr << "Declaração implícita da variável '" << esquerda->nome << "' com tipo " << tipo_expressao->to_string() << endl;
        
        // Criar nova variável
        Variavel* nova_variavel = new Variavel();
        nova_variavel->nome = esquerda;
        nova_variavel->tipo_semantico = tipo_expressao;
        
        // Adicionar à lista de variáveis (cast para remover const)
        const_cast<vector<Variavel*>&>(variaveis).push_back(nova_variavel);
        
        return true;
    }
    
    // Se a variável já existe, atualizar seu tipo (inferência de tipos)
    // Em Smalltalk, podemos atribuir qualquer tipo a uma variável Object
    if (tipo_variavel->tipo == TipoSmallTalk::OBJECT) {
        // Encontrar a variável e atualizar seu tipo
        for (auto& var : variaveis) {
            if (var->nome->nome == esquerda->nome) {
                var->tipo_semantico = tipo_expressao;
                cerr << "Inferência de tipos: Variável '" << esquerda->nome << "' atualizada de Object para " << tipo_expressao->to_string() << endl;
                break;
            }
        }
        
        // Também atualizar parâmetros se necessário
        for (auto& param : const_cast<vector<Variavel*>&>(parametros)) {
            if (param->nome->nome == esquerda->nome) {
                param->tipo_semantico = tipo_expressao;
                cerr << "Inferência de tipos: Parâmetro '" << esquerda->nome << "' atualizado de Object para " << tipo_expressao->to_string() << endl;
                break;
            }
        }
    } else if (!tipo_variavel->compativel_com(tipo_expressao)) {
        cerr << "Erro semântico: Tipos incompatíveis na atribuição (" 
             << tipo_variavel->to_string() << " := " << tipo_expressao->to_string() << ")" << endl;
        return false;
    }
    
    return true;
}

double ComandoAtribuicao::executar(map<string, double>& valores_variaveis) {
    double valor = direita->calcular_valor(valores_variaveis);
    valores_variaveis[esquerda->nome] = valor;
    return valor;
}
