#include "ExpressaoNumero.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoNumero::ExpressaoNumero() { 
    tipo_resultado = Tipo::INTEGER_TYPE();
}

void ExpressaoNumero::debug_com_tab(int tab) {
  tab3(tab);
  cerr << numero->valor << " [Numero]"<< endl;
}

Tipo* ExpressaoNumero::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    return Tipo::INTEGER_TYPE();
}

double ExpressaoNumero::calcular_valor(const map<string, double>& valores_variaveis) {
    return static_cast<double>(numero->valor);
}
