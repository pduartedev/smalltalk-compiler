#include "ExpressaoSelf.hpp"
#include <iostream>
using namespace std;

ExpressaoSelf::ExpressaoSelf() {
}

void ExpressaoSelf::debug() {
    debug_com_tab(0);
}

void ExpressaoSelf::debug_com_tab(int tab) {
    for (int i = 0; i < tab; i++) cerr << "   ";
    cerr << "SELF";
}

Tipo* ExpressaoSelf::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    (void)variaveis;  // Evitar warning
    (void)parametros; // Evitar warning
    
    // Self é sempre do tipo Object
    Tipo* tipo = new Tipo(TipoSmallTalk::OBJECT, "Object");
    return tipo;
}

double ExpressaoSelf::calcular_valor(const map<string, double>& valores_variaveis) {
    // Para funções binárias matemáticas, usar o valor de self se estiver disponível
    auto it = valores_variaveis.find("self");
    if (it != valores_variaveis.end()) {
        return it->second;
    }
    // Valor padrão para self
    return 0.0;
}
