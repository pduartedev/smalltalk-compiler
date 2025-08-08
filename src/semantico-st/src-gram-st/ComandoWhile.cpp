#include "ComandoWhile.hpp"
#include "Variavel.hpp"
#include "../debug-util.hpp"
#include <iostream>

ComandoWhile::ComandoWhile() {}

void ComandoWhile::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "WHILE:" << endl;
    condicao->debug_com_tab(tab + 1);
    tab3(tab);
    cerr << "DO:" << endl;
    for (auto cmd : comandos_loop) {
        cmd->debug_com_tab(tab + 1);
    }
}

bool ComandoWhile::verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    Tipo* tipo_condicao = condicao->verificar_tipos(variaveis, parametros);
    
    if (tipo_condicao == nullptr) {
        return false;
    }
    
    if (!tipo_condicao->pode_fazer_operacao_logica()) {
        cerr << "Erro semântico: Condição do WHILE deve ser booleana" << endl;
        return false;
    }
    
    // Verificar comandos do loop
    for (auto cmd : comandos_loop) {
        if (!cmd->verificar_tipos_semanticos(variaveis, parametros)) {
            return false;
        }
    }
    
    return true;
}

double ComandoWhile::executar(map<string, double>& valores_variaveis) {
    double ultimo_valor = 0.0;
    int contador_seguranca = 0;
    const int MAX_ITERACOES = 10000; // Evitar loops infinitos
    
    while (condicao->calcular_valor(valores_variaveis) != 0.0 && contador_seguranca < MAX_ITERACOES) {
        for (auto cmd : comandos_loop) {
            ultimo_valor = cmd->executar(valores_variaveis);
        }
        contador_seguranca++;
    }
    
    if (contador_seguranca >= MAX_ITERACOES) {
        cerr << "Aviso: Loop WHILE interrompido após " << MAX_ITERACOES << " iterações" << endl;
    }
    
    return ultimo_valor;
}
