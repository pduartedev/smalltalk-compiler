#include "ExpressaoDivisao.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoDivisao::ExpressaoDivisao() { }

void ExpressaoDivisao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "/ [ INI Divisao"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "/" << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << "/ ] FIM Divisao"<< endl;
}

Tipo* ExpressaoDivisao::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    Tipo* tipo_esq = esquerda->verificar_tipos(variaveis, parametros);
    Tipo* tipo_dir = direita->verificar_tipos(variaveis, parametros);
    
    if (tipo_esq == nullptr || tipo_dir == nullptr) {
        return nullptr;
    }
    
    if (!tipo_esq->pode_fazer_operacao_aritmetica() || !tipo_dir->pode_fazer_operacao_aritmetica()) {
        cerr << "Erro semântico: Operação de divisão requer operandos numéricos" << endl;
        return nullptr;
    }
    
    // Divisão sempre retorna float
    tipo_resultado = Tipo::FLOAT_TYPE();
    return Tipo::FLOAT_TYPE();
}

double ExpressaoDivisao::calcular_valor(const map<string, double>& valores_variaveis) {
    double divisor = direita->calcular_valor(valores_variaveis);
    if (divisor == 0.0) {
        cerr << "Erro de execução: Divisão por zero" << endl;
        return 0.0;
    }
    return esquerda->calcular_valor(valores_variaveis) / divisor;
}
