#ifndef _EXPRESSAO_CONDICIONAL_HPP_
#define _EXPRESSAO_CONDICIONAL_HPP_

#include "Expressao.hpp"
#include "Comando.hpp"
#include <vector>
#include <string>

class ExpressaoCondicional : public Expressao {
public:
    Expressao* condicao;
    vector<Comando*> comandos_then;
    vector<Comando*> comandos_else;
    bool tem_else;
    string operador; // "ifTrue:" ou "ifTrue:ifFalse:"
    
    ExpressaoCondicional(Expressao* cond, const vector<Comando*>& cmd_then, const vector<Comando*>& cmd_else = vector<Comando*>());
    
    void debug_com_tab(int tab) override;
    Tipo* verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) override;
    double calcular_valor(const map<string, double>& valores_variaveis) override;
    
    static ExpressaoCondicional* extrai_condicional(Expressao* primary, No_arv_parse* no);
    static vector<Comando*> extrair_comandos_do_bloco(No_arv_parse* no);
};

#endif
