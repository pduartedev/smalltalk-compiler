#ifndef _EXPRESSAO_RELACIONAL_HPP_
#define _EXPRESSAO_RELACIONAL_HPP_

#include "Expressao.hpp"
#include <string>

class ExpressaoRelacional : public Expressao {
public:
    Expressao* esquerda;
    Expressao* direita;
    string operador;
    
    ExpressaoRelacional(Expressao* esq, Expressao* dir, const string& op);
    
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
