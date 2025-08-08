#include "ComandoIf.hpp"
#include "Variavel.hpp"
#include "../debug-util.hpp"
#include <iostream>

ComandoIf::ComandoIf() : tem_else(false) {}

void ComandoIf::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "IF:" << endl;
    condicao->debug_com_tab(tab + 1);
    tab3(tab);
    cerr << "THEN:" << endl;
    for (auto cmd : comandos_then) {
        cmd->debug_com_tab(tab + 1);
    }
    if (tem_else) {
        tab3(tab);
        cerr << "ELSE:" << endl;
        for (auto cmd : comandos_else) {
            cmd->debug_com_tab(tab + 1);
        }
    }
}

bool ComandoIf::verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    Tipo* tipo_condicao = condicao->verificar_tipos(variaveis, parametros);
    
    if (tipo_condicao == nullptr) {
        return false;
    }
    
    if (!tipo_condicao->pode_fazer_operacao_logica()) {
        cerr << "Erro semântico: Condição do IF deve ser booleana" << endl;
        return false;
    }
    
    // Verificar comandos do THEN
    for (auto cmd : comandos_then) {
        if (!cmd->verificar_tipos_semanticos(variaveis, parametros)) {
            return false;
        }
    }
    
    // Verificar comandos do ELSE se existir
    if (tem_else) {
        for (auto cmd : comandos_else) {
            if (!cmd->verificar_tipos_semanticos(variaveis, parametros)) {
                return false;
            }
        }
    }
    
    return true;
}

double ComandoIf::executar(map<string, double>& valores_variaveis) {
    double valor_condicao = condicao->calcular_valor(valores_variaveis);
    double ultimo_valor = 0.0;
    
    if (valor_condicao != 0.0) {
        // Executa comandos do THEN
        for (auto cmd : comandos_then) {
            ultimo_valor = cmd->executar(valores_variaveis);
        }
    } else if (tem_else) {
        // Executa comandos do ELSE
        for (auto cmd : comandos_else) {
            ultimo_valor = cmd->executar(valores_variaveis);
        }
    }
    
    return ultimo_valor;
}
