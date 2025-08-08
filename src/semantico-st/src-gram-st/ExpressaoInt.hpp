#ifndef _EXPRESSAO_INT_HPP_
#define _EXPRESSAO_INT_HPP_

#include "Expressao.hpp"

class ExpressaoInt : public Expressao {
public:
    int valor;
    
    ExpressaoInt(int val);
    
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
    
    static ExpressaoInt* extrai_int(No_arv_parse* no);
};

#endif
