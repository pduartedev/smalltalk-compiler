#include "ExpressaoCondicional.hpp"
#include "Variavel.hpp"
#include "Funcao.hpp"
#include "../debug-util.hpp"
#include <iostream>

ExpressaoCondicional::ExpressaoCondicional(Expressao* cond, const vector<Comando*>& cmd_then, const vector<Comando*>& cmd_else) 
    : condicao(cond), comandos_then(cmd_then), comandos_else(cmd_else) {
    tem_else = !cmd_else.empty();
    operador = tem_else ? "ifTrue:ifFalse:" : "ifTrue:";
    tipo_resultado = Tipo::BOOLEAN_TYPE();
}

void ExpressaoCondicional::debug_com_tab(int tab) {
    tab3(tab);
    cerr << "ExpressaoCondicional [" << operador << "]:" << endl;
    tab3(tab + 1);
    cerr << "CONDICAO:" << endl;
    condicao->debug_com_tab(tab + 2);
    tab3(tab + 1);
    cerr << "THEN:" << endl;
    for (auto cmd : comandos_then) {
        cmd->debug_com_tab(tab + 2);
    }
    if (tem_else) {
        tab3(tab + 1);
        cerr << "ELSE:" << endl;
        for (auto cmd : comandos_else) {
            cmd->debug_com_tab(tab + 2);
        }
    }
}

Tipo* ExpressaoCondicional::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
    Tipo* tipo_condicao = condicao->verificar_tipos(variaveis, parametros);
    
    if (tipo_condicao == nullptr) {
        return nullptr;
    }
    
    if (!tipo_condicao->pode_fazer_operacao_logica()) {
        cerr << "Erro semântico: Condição do IF deve ser booleana" << endl;
        return nullptr;
    }
    
    // Verificar comandos do THEN
    for (auto cmd : comandos_then) {
        if (!cmd->verificar_tipos_semanticos(const_cast<vector<Variavel*>&>(variaveis), parametros)) {
            return nullptr;
        }
    }
    
    // Verificar comandos do ELSE se existir
    if (tem_else) {
        for (auto cmd : comandos_else) {
            if (!cmd->verificar_tipos_semanticos(const_cast<vector<Variavel*>&>(variaveis), parametros)) {
                return nullptr;
            }
        }
    }
    
    return tipo_resultado;
}

double ExpressaoCondicional::calcular_valor(const map<string, double>& valores_variaveis) {
    double valor_condicao = condicao->calcular_valor(valores_variaveis);
    double ultimo_valor = 0.0;
    
    // Criar uma cópia não-const para executar comandos
    map<string, double> valores_mutaveis = valores_variaveis;
    
    if (valor_condicao != 0.0) {
        // Executa comandos do THEN
        for (auto cmd : comandos_then) {
            ultimo_valor = cmd->executar(valores_mutaveis);
        }
    } else if (tem_else) {
        // Executa comandos do ELSE
        for (auto cmd : comandos_else) {
            ultimo_valor = cmd->executar(valores_mutaveis);
        }
    }
    
    return ultimo_valor;
}

ExpressaoCondicional* ExpressaoCondicional::extrai_condicional(Expressao* primary, No_arv_parse* no) {
    if (no == nullptr || primary == nullptr) return nullptr;
    
    // Keyword_Message -> Keyword_Message_Element Keyword_Message_Element_List
    if (no->simb == "Keyword_Message" && no->filhos.size() >= 1) {
        No_arv_parse* primeiro_elemento = no->filhos[0];
        
        // Verifica se é ifTrue:
        if (primeiro_elemento->simb == "Keyword_Message_Element" && primeiro_elemento->filhos.size() >= 2) {
            string keyword = primeiro_elemento->filhos[0]->dado_extra;
            
            if (keyword == "ifTrue:") {
                // Extrai o bloco THEN
                vector<Comando*> comandos_then = extrair_comandos_do_bloco(primeiro_elemento->filhos[1]);
                
                // Verifica se há ifFalse:
                vector<Comando*> comandos_else;
                if (no->filhos.size() >= 2) {
                    cerr << "DEBUG: Verificando ifFalse: - no tem " << no->filhos.size() << " filhos" << endl;
                    No_arv_parse* lista_elementos = no->filhos[1];
                    cerr << "DEBUG: lista_elementos->simb = " << lista_elementos->simb << endl;
                    if (lista_elementos->simb == "Keyword_Message_Element_List" && lista_elementos->filhos.size() >= 1) {
                        cerr << "DEBUG: lista_elementos tem " << lista_elementos->filhos.size() << " filhos" << endl;
                        No_arv_parse* segundo_elemento = lista_elementos->filhos[0];
                        cerr << "DEBUG: segundo_elemento->simb = " << segundo_elemento->simb << endl;
                        if (segundo_elemento->simb == "Keyword_Message_Element" && segundo_elemento->filhos.size() >= 2) {
                            string segunda_keyword = segundo_elemento->filhos[0]->dado_extra;
                            cerr << "DEBUG: segunda_keyword = " << segunda_keyword << endl;
                            if (segunda_keyword == "ifFalse:") {
                                cerr << "DEBUG: Encontrou ifFalse:, extraindo comandos..." << endl;
                                comandos_else = extrair_comandos_do_bloco(segundo_elemento->filhos[1]);
                                cerr << "DEBUG: Extraiu " << comandos_else.size() << " comandos do bloco else" << endl;
                            }
                        }
                    }
                }
                
                return new ExpressaoCondicional(primary, comandos_then, comandos_else);
            }
        }
    }
    
    return nullptr;
}

vector<Comando*> ExpressaoCondicional::extrair_comandos_do_bloco(No_arv_parse* no) {
    vector<Comando*> comandos;
    
    if (no == nullptr) {
        cerr << "DEBUG: extrair_comandos_do_bloco - no é nullptr" << endl;
        return comandos;
    }
    
    cerr << "DEBUG: extrair_comandos_do_bloco - processando nó: " << no->simb << endl;
    
    // Keyword_Argument pode ser apenas Primary (Block_Constructor)
    if (no->simb == "Keyword_Argument" && no->filhos.size() >= 1) {
        cerr << "DEBUG: Keyword_Argument encontrado com " << no->filhos.size() << " filhos" << endl;
        No_arv_parse* primary = no->filhos[0];
        cerr << "DEBUG: primary->simb = " << primary->simb << endl;
        if (primary->simb == "Primary" && primary->filhos.size() >= 1) {
            No_arv_parse* block_constructor = primary->filhos[0];
            cerr << "DEBUG: block_constructor->simb = " << block_constructor->simb << endl;
            if (block_constructor->simb == "Block_Constructor") {
                cerr << "DEBUG: Block_Constructor encontrado!" << endl;
                // Block_Constructor -> TOKEN_left_bracket Block_Body_Content TOKEN_right_bracket
                if (block_constructor->filhos.size() >= 2) {
                    No_arv_parse* block_body = block_constructor->filhos[1];
                    cerr << "DEBUG: block_body->simb = " << block_body->simb << endl;
                    if (block_body->simb == "Block_Body_Content") {
                        // Block_Body_Content -> Temporaries_Opt Block_Statements_Opt
                        if (block_body->filhos.size() >= 2) {
                            No_arv_parse* statements_opt = block_body->filhos[1];
                            cerr << "DEBUG: statements_opt->simb = " << statements_opt->simb << " com " << statements_opt->filhos.size() << " filhos" << endl;
                            if (statements_opt->simb == "Block_Statements_Opt" && statements_opt->filhos.size() >= 1) {
                                No_arv_parse* statements = statements_opt->filhos[0];
                                cerr << "DEBUG: Extraindo comandos com Comando::extrai_block_statements" << endl;
                                comandos = Comando::extrai_block_statements(statements);
                                cerr << "DEBUG: Extraiu " << comandos.size() << " comandos" << endl;
                            } else {
                                cerr << "DEBUG: Block_Statements_Opt não tem filhos suficientes" << endl;
                            }
                        }
                    }
                }
            }
        }
    } else {
        cerr << "DEBUG: Nó não é Keyword_Argument ou não tem filhos suficientes" << endl;
    }
    
    return comandos;
}
