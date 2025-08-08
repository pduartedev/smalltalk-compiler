#ifndef _COMANDO_IF_HPP_
#define _COMANDO_IF_HPP_

#include "Comando.hpp"
#include "Expressao.hpp"
#include <vector>

class ComandoIf : public Comando {
public:
    Expressao* condicao;
    vector<Comando*> comandos_then;
    vector<Comando*> comandos_else;
    bool tem_else;
    
    ComandoIf();
    
    void debug_com_tab(int tab) override;
    bool verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double executar(map<string, double>& valores_variaveis) override;
};

#endif
