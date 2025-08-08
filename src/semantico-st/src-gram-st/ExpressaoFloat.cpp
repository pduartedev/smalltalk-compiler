#include "ExpressaoFloat.hpp"
#include "../debug-util.hpp"
#include <iostream>
#include <sstream>

ExpressaoFloat::ExpressaoFloat(double val) : valor(val) {
    tipo_resultado = Tipo::FLOAT_TYPE();
}

void ExpressaoFloat::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "ExpressaoFloat [" << valor << "]" << endl;
}

Tipo* ExpressaoFloat::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    return Tipo::FLOAT_TYPE();
}

double ExpressaoFloat::calcular_valor(const map<string, double>& valores_variaveis) {
    return valor;
}

ExpressaoFloat* ExpressaoFloat::extrai_float(No_arv_parse* no) {
    if (no->simb == "TOKEN_float") {
        istringstream iss(no->dado_extra);
        double val;
        iss >> val;
        return new ExpressaoFloat(val);
    }
    return nullptr;
}
