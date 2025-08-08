#include "ExpressaoBoolean.hpp"
#include "../debug-util.hpp"
#include <iostream>

ExpressaoBoolean::ExpressaoBoolean(bool val) : valor(val) {
    tipo_resultado = Tipo::BOOLEAN_TYPE();
}

void ExpressaoBoolean::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "ExpressaoBoolean [" << (valor ? "true" : "false") << "]" << endl;
}

Tipo* ExpressaoBoolean::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    return Tipo::BOOLEAN_TYPE();
}

double ExpressaoBoolean::calcular_valor(const map<string, double>& valores_variaveis) {
    return valor ? 1.0 : 0.0;
}

ExpressaoBoolean* ExpressaoBoolean::extrai_boolean(No_arv_parse* no) {
    if (no->simb == "TOKEN_true") {
        return new ExpressaoBoolean(true);
    } else if (no->simb == "TOKEN_false") {
        return new ExpressaoBoolean(false);
    }
    return nullptr;
}
