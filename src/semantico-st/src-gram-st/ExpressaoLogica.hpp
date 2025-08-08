#ifndef _EXPRESSAO_LOGICA_HPP_
#define _EXPRESSAO_LOGICA_HPP_

#include "Expressao.hpp"
#include <string>

class ExpressaoLogica : public Expressao {
public:
    Expressao* esquerda;
    Expressao* direita;
    string operador;
    
    ExpressaoLogica(Expressao* esq, Expressao* dir, const string& op);
    ExpressaoLogica(Expressao* operando, const string& op); // Para operador unário !
    
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
