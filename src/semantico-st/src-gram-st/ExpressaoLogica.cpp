#include "ExpressaoLogica.hpp"
#include "Variavel.hpp"
#include "../debug-util.hpp"
#include <iostream>

ExpressaoLogica::ExpressaoLogica(Expressao* esq, Expressao* dir, const string& op) 
    : esquerda(esq), direita(dir), operador(op) {
    tipo_resultado = Tipo::BOOLEAN_TYPE();
}

ExpressaoLogica::ExpressaoLogica(Expressao* operando, const string& op) 
    : esquerda(operando), direita(nullptr), operador(op) {
    tipo_resultado = Tipo::BOOLEAN_TYPE();
}

void ExpressaoLogica::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "ExpressaoLogica [" << operador << "]" << endl;
    esquerda->debug_com_tab(tab + 1);
    if (direita != nullptr) {
        direita->debug_com_tab(tab + 1);
    }
}

Tipo* ExpressaoLogica::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    Tipo* tipo_esq = esquerda->verificar_tipos(variaveis, parametros);
    
    if (operador == "!" || operador == "not") {
        // Operador unário de negação
        if (!tipo_esq->pode_fazer_operacao_logica()) {
            cerr << "Erro semântico: Operador " << operador << " não pode ser aplicado ao tipo " 
                 << tipo_esq->to_string() << endl;
            return nullptr;
        }
        return Tipo::BOOLEAN_TYPE();
    }
    
    if (direita == nullptr) {
        cerr << "Erro semântico: Operador binário " << operador << " requer dois operandos" << endl;
        return nullptr;
    }
    
    Tipo* tipo_dir = direita->verificar_tipos(variaveis, parametros);
    
    // Operadores lógicos binários requerem tipos booleanos
    if (!tipo_esq->pode_fazer_operacao_logica() || !tipo_dir->pode_fazer_operacao_logica()) {
        cerr << "Erro semântico: Operadores lógicos " << operador 
             << " requerem operandos booleanos" << endl;
        return nullptr;
    }
    
    return Tipo::BOOLEAN_TYPE();
}

double ExpressaoLogica::calcular_valor(const map<string, double>& valores_variaveis) {
    double val_esq = esquerda->calcular_valor(valores_variaveis);
    
    if (operador == "!" || operador == "not") {
        return (val_esq == 0.0) ? 1.0 : 0.0;
    }
    
    if (direita == nullptr) {
        return 0.0; // Erro
    }
    
    double val_dir = direita->calcular_valor(valores_variaveis);
    
    if (operador == "&&") {
        return (val_esq != 0.0 && val_dir != 0.0) ? 1.0 : 0.0;
    } else if (operador == "||") {
        return (val_esq != 0.0 || val_dir != 0.0) ? 1.0 : 0.0;
    }
    
    return 0.0;
}
