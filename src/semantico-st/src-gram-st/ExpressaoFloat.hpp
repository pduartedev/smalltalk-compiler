#ifndef _EXPRESSAO_FLOAT_HPP_
#define _EXPRESSAO_FLOAT_HPP_

#include "Expressao.hpp"

class ExpressaoFloat : public Expressao {
public:
    double valor;
    
    ExpressaoFloat(double val);
    
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
    
    static ExpressaoFloat* extrai_float(No_arv_parse* no);
};

#endif
