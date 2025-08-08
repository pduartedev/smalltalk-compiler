#include "Analisador.hpp"
#include <iostream>
#include <iomanip>

bool Analisador::verificar_semantica(Funcao *f) {
    if (f == nullptr) {
        cerr << "Erro: Função é nula" << endl;
        return false;
    }
    
    // Verificar tipos de todos os comandos
    for (auto cmd : f->comandos) {
        if (!cmd->verificar_tipos_semanticos(f->variaveis, f->parametros)) {
            return false;
        }
    }
    
    return true;
}

double Analisador::calcula_ultimo_valor(Funcao *f, const vector<double> &params) {
    if (f == nullptr) {
        cerr << "Erro: Função é nula" << endl;
        return 0.0;
    }
    
    // Verificar se o número de parâmetros está correto
    if (params.size() != f->parametros.size()) {
        cerr << "Erro: Número de parâmetros incorreto. Esperado: " 
             << f->parametros.size() << ", Fornecido: " << params.size() << endl;
        return 0.0;
    }
    
    // Inicializar mapa de valores das variáveis
    map<string, double> valores_variaveis;
    
    // Inicializar parâmetros
    for (size_t i = 0; i < f->parametros.size(); i++) {
        valores_variaveis[f->parametros[i]->nome->nome] = params[i];
    }
    
    // Inicializar variáveis locais com valores padrão
    for (auto var : f->variaveis) {
        valores_variaveis[var->nome->nome] = 0.0;
    }
    
    // Executar comandos e retornar o último valor atribuído
    double ultimo_valor = 0.0;
    for (auto cmd : f->comandos) {
        ultimo_valor = cmd->executar(valores_variaveis);
    }
    
    return ultimo_valor;
}

void Analisador::imprimir_resultado(double valor, Tipo* tipo) {
    cerr << "DEBUG imprimir_resultado: valor=" << valor << ", tipo=" << (tipo ? (int)tipo->tipo : -1) << endl;
    cerr << "DEBUG: INTEGER=" << (int)TipoSmallTalk::INTEGER << ", FLOAT=" << (int)TipoSmallTalk::FLOAT << ", BOOLEAN=" << (int)TipoSmallTalk::BOOLEAN << endl;
    if (tipo == nullptr || tipo->tipo == TipoSmallTalk::INTEGER) {
        cerr << "DEBUG: Entrou na condição INTEGER" << endl;
        cout << static_cast<int>(valor) << endl;
    } else if (tipo->tipo == TipoSmallTalk::FLOAT) {
        cerr << "DEBUG: Entrou na condição FLOAT" << endl;
        cout << fixed << setprecision(2) << valor << endl;
    } else if (tipo->tipo == TipoSmallTalk::BOOLEAN) {
        cerr << "DEBUG: Entrou na condição BOOLEAN" << endl;
        cout << (valor != 0.0 ? "true" : "false") << endl;
    } else {
        cerr << "DEBUG: Entrou na condição ELSE" << endl;
        cout << valor << endl;
    }
}
