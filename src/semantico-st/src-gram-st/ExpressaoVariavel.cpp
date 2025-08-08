#include "ExpressaoVariavel.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;
ExpressaoVariavel::ExpressaoVariavel() { }

void ExpressaoVariavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "VAR[" << nome->nome << "]" << endl;
}

Tipo* ExpressaoVariavel::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    // Procurar nas variáveis locais
    for (const auto& var : variaveis) {
        if (var->nome->nome == nome->nome) {
            tipo_resultado = var->tipo_semantico;
            return var->tipo_semantico;
        }
    }
    
    // Procurar nos parâmetros
    for (const auto& param : parametros) {
        if (param->nome->nome == nome->nome) {
            tipo_resultado = param->tipo_semantico;
            return param->tipo_semantico;
        }
    }
    
    cerr << "Erro semântico: Variável '" << nome->nome << "' não declarada" << endl;
    return nullptr;
}

double ExpressaoVariavel::calcular_valor(const map<string, double>& valores_variaveis) {
    auto it = valores_variaveis.find(nome->nome);
    if (it != valores_variaveis.end()) {
        return it->second;
    }
    return 0.0; // Valor padrão se não encontrar
}
