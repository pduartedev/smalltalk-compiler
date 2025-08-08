#ifndef _EXPRESSAO_BOOLEAN_HPP_
#define _EXPRESSAO_BOOLEAN_HPP_

#include "Expressao.hpp"

class ExpressaoBoolean : public Expressao {
public:
    bool valor;
    
    ExpressaoBoolean(bool val);
    
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
    
    static ExpressaoBoolean* extrai_boolean(No_arv_parse* no);
};

#endif
