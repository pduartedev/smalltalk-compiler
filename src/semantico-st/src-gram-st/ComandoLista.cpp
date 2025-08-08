#include "ComandoLista.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoLista::ComandoLista(){ }

void ComandoLista::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "{ INI Lista Comandos" << endl;
  for (int iv = 0; iv < lista_variaveis.size(); ++iv) {
    lista_variaveis[iv]->debug_com_tab(tab+1);
  }
  for (int ic = 0; ic < lista_comandos.size(); ++ic) {
    lista_comandos[ic]->debug_com_tab(tab+1);
  }
  tab3(tab);
  cerr << "} FIM Lista Comandos" << endl;
}

bool ComandoLista::verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    // Criar nova lista de variáveis que inclui as variáveis locais do bloco
    vector<Variavel*> variaveis_completas = variaveis;
    variaveis_completas.insert(variaveis_completas.end(), lista_variaveis.begin(), lista_variaveis.end());
    
    // Verificar todos os comandos
    for (auto cmd : lista_comandos) {
        if (!cmd->verificar_tipos_semanticos(variaveis_completas, parametros)) {
            return false;
        }
    }
    
    return true;
}

double ComandoLista::executar(map<string, double>& valores_variaveis) {
    // Inicializar variáveis locais com valores padrão
    for (auto var : lista_variaveis) {
        if (valores_variaveis.find(var->nome->nome) == valores_variaveis.end()) {
            valores_variaveis[var->nome->nome] = 0.0; // Valor padrão
        }
    }
    
    double ultimo_valor = 0.0;
    for (auto cmd : lista_comandos) {
        ultimo_valor = cmd->executar(valores_variaveis);
    }
    
    return ultimo_valor;
}
