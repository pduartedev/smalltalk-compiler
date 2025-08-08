#include "ExpressaoInt.hpp"
#include "../debug-util.hpp"
#include <iostream>
#include <sstream>

ExpressaoInt::ExpressaoInt(int val) : valor(val) {
    tipo_resultado = Tipo::INTEGER_TYPE();
}

void ExpressaoInt::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "ExpressaoInt [" << valor << "]" << endl;
}

Tipo* ExpressaoInt::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    return Tipo::INTEGER_TYPE();
}

double ExpressaoInt::calcular_valor(const map<string, double>& valores_variaveis) {
    return static_cast<double>(valor);
}

ExpressaoInt* ExpressaoInt::extrai_int(No_arv_parse* no) {
    if (no->simb == "TOKEN_decimal_integer") {
        istringstream iss(no->dado_extra);
        int val;
        iss >> val;
        return new ExpressaoInt(val);
    }
    return nullptr;
}
