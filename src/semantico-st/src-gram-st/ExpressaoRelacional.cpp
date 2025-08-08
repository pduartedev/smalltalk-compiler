#include "ExpressaoRelacional.hpp"
#include "Variavel.hpp"
#include "../debug-util.hpp"
#include <iostream>

ExpressaoRelacional::ExpressaoRelacional(Expressao* esq, Expressao* dir, const string& op) 
    : esquerda(esq), direita(dir), operador(op) {
    tipo_resultado = Tipo::BOOLEAN_TYPE();
}

void ExpressaoRelacional::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "ExpressaoRelacional [" << operador << "]" << endl;
    esquerda->debug_com_tab(tab + 1);
    direita->debug_com_tab(tab + 1);
}

Tipo* ExpressaoRelacional::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    Tipo* tipo_esq = esquerda->verificar_tipos(variaveis, parametros);
    Tipo* tipo_dir = direita->verificar_tipos(variaveis, parametros);
    
    // Operadores relacionais requerem tipos compatíveis
    if (!tipo_esq->compativel_com(tipo_dir)) {
        cerr << "Erro semântico: Tipos incompatíveis na expressão relacional " << operador 
             << " (" << tipo_esq->to_string() << " " << operador << " " << tipo_dir->to_string() << ")" << endl;
        return nullptr;
    }
    
    // Operadores relacionais só funcionam com números
    if (!tipo_esq->pode_fazer_operacao_aritmetica()) {
        cerr << "Erro semântico: Operador relacional " << operador 
             << " não pode ser aplicado ao tipo " << tipo_esq->to_string() << endl;
        return nullptr;
    }
    
    return Tipo::BOOLEAN_TYPE();
}

double ExpressaoRelacional::calcular_valor(const map<string, double>& valores_variaveis) {
    double val_esq = esquerda->calcular_valor(valores_variaveis);
    double val_dir = direita->calcular_valor(valores_variaveis);
    
    if (operador == "<") {
        return val_esq < val_dir ? 1.0 : 0.0;
    } else if (operador == "==") {
        return val_esq == val_dir ? 1.0 : 0.0;
    } else if (operador == ">") {
        return val_esq > val_dir ? 1.0 : 0.0;
    } else if (operador == "<=") {
        return val_esq <= val_dir ? 1.0 : 0.0;
    } else if (operador == ">=") {
        return val_esq >= val_dir ? 1.0 : 0.0;
    } else if (operador == "!=") {
        return val_esq != val_dir ? 1.0 : 0.0;
    }
    
    return 0.0;
}
