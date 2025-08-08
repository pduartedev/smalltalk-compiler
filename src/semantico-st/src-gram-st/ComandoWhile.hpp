#ifndef _COMANDO_WHILE_HPP_
#define _COMANDO_WHILE_HPP_

#include "Comando.hpp"
#include "Expressao.hpp"
#include <vector>

class ComandoWhile : public Comando {
public:
    Expressao* condicao;
    vector<Comando*> comandos_loop;
    
    ComandoWhile();
    
    void debug_com_tab(int tab) override;
    bool verificar_tipos_semanticos(vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double executar(map<string, double>& valores_variaveis) override;
};

#endif
