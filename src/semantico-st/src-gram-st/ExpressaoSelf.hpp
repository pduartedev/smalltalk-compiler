#ifndef EXPRESSAO_SELF_HPP
#define EXPRESSAO_SELF_HPP

#include "Expressao.hpp"
#include "Tipo.hpp"
#include <map>
#include <string>

class ExpressaoSelf : public Expressao {
public:
    ExpressaoSelf();
    void debug();
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
};

#endif
