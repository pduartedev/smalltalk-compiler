#include "GeradorRI.hpp"
#include "src-gram-st/ComandoAtribuicao.hpp"
#include "src-gram-st/ExpressaoCondicional.hpp"
#include "src-gram-st/ExpressaoBinaria.hpp"
#include "src-gram-st/ExpressaoInt.hpp"
#include "src-gram-st/ExpressaoVariavel.hpp"
#include "src-gram-st/ExpressaoChamadaFuncao.hpp"
#include "src-gram-st/ExpressaoRelacional.hpp"
#include <iostream>
#include <sstream>

GeradorRI::GeradorRI() : contador_temp(1), contador_label(1), frame_pointer(nullptr), frame_atual(nullptr) {
    frame_pointer = ExpTemp::criar(0); // FP é temp 0
}

GeradorRI::~GeradorRI() {
    delete frame_pointer;
}

std::string GeradorRI::novoLabel() {
    return "L" + std::to_string(contador_label++);
}

ExpTemp* GeradorRI::novoTemp() {
    return ExpTemp::criar(contador_temp++);
}

StmSeq* GeradorRI::converterFuncao(Funcao* funcao, FrameFuncao* frame) {
    frame_atual = frame;
    StmSeq* programa = StmSeq::criar();
    
    std::cout << "=== GERAÇÃO DE REPRESENTAÇÃO INTERMEDIÁRIA ===" << std::endl;
    std::cout << "Função: " << (funcao->nome_funcao ? funcao->nome_funcao->nome : "ANÔNIMA") << std::endl;
    
    // Converter cada comando da função
    for (Comando* comando : funcao->comandos) {
        Stm* stm_ri = converterComando(comando);
        if (stm_ri) {
            programa->adicionar(stm_ri);
        }
    }
    
    return programa;
}

Stm* GeradorRI::converterComando(Comando* comando) {
    if (!comando) return nullptr;
    
    // Verificar tipo do comando
    ComandoAtribuicao* atrib = dynamic_cast<ComandoAtribuicao*>(comando);
    if (atrib) {
        std::cout << "Convertendo atribuição: " << (atrib->esquerda ? atrib->esquerda->nome : "NULL") << std::endl;
        
        Exp* destino = gerarAcessoVariavel(atrib->esquerda ? atrib->esquerda->nome : "");
        Exp* origem = converterExpressao(atrib->direita);
        
        return StmMove::criar(destino, origem);
    }
    
    // Outros tipos de comando podem ser adicionados aqui
    std::cout << "Tipo de comando não implementado ainda" << std::endl;
    return nullptr;
}

Exp* GeradorRI::converterExpressao(Expressao* expressao) {
    if (!expressao) return nullptr;
    
    // ExpressaoInt (constante)
    ExpressaoInt* exp_int = dynamic_cast<ExpressaoInt*>(expressao);
    if (exp_int) {
        return ExpConst::criar(exp_int->valor);
    }
    
    // ExpressaoVariavel
    ExpressaoVariavel* exp_var = dynamic_cast<ExpressaoVariavel*>(expressao);
    if (exp_var) {
        return gerarAcessoVariavel(exp_var->nome ? exp_var->nome->nome : "");
    }
    
    // ExpressaoBinaria
    ExpressaoBinaria* exp_bin = dynamic_cast<ExpressaoBinaria*>(expressao);
    if (exp_bin) {
        Exp* esquerda = converterExpressao(exp_bin->esquerda);
        Exp* direita = converterExpressao(exp_bin->direita);
        Operador* op = converterOperador(exp_bin->operador);
        
        return ExpBinop::criar(op, esquerda, direita);
    }
    
    // ExpressaoRelacional
    ExpressaoRelacional* exp_rel = dynamic_cast<ExpressaoRelacional*>(expressao);
    if (exp_rel) {
        Exp* esquerda = converterExpressao(exp_rel->esquerda);
        Exp* direita = converterExpressao(exp_rel->direita);
        Operador* op = converterOperador(exp_rel->operador);
        
        return ExpBinop::criar(op, esquerda, direita);
    }
    
    // ExpressaoCondicional (ifTrue:)
    ExpressaoCondicional* exp_cond = dynamic_cast<ExpressaoCondicional*>(expressao);
    if (exp_cond) {
        std::cout << "Convertendo expressão condicional ifTrue:" << std::endl;
        
        // Para ifTrue:, geramos uma sequência com CJUMP
        std::string label_true = novoLabel();
        std::string label_false = novoLabel();
        std::string label_fim = novoLabel();
        
        // Condição
        Exp* condicao = converterExpressao(exp_cond->condicao);
        
        // Para simplificar, retornamos apenas a condição por enquanto
        // A implementação completa requer StmSeq com múltiplos statements
        return condicao;
    }
    
    // ExpressaoChamadaFuncao
    ExpressaoChamadaFuncao* exp_call = dynamic_cast<ExpressaoChamadaFuncao*>(expressao);
    if (exp_call) {
        std::string nome_func = exp_call->nome_funcao ? exp_call->nome_funcao->nome : "DESCONHECIDA";
        std::cout << "Convertendo chamada de função: " << nome_func << std::endl;
        
        // Converter argumentos
        std::vector<Exp*> argumentos_ri;
        for (Expressao* arg : exp_call->parametros) {
            argumentos_ri.push_back(converterExpressao(arg));
        }
        
        // Função é um nome (poderia ser ExpName, mas usamos ExpTemp por simplicidade)
        ExpTemp* nome_funcao = ExpTemp::criar(-1, nome_func);
        
        return ExpCall::criar(nome_funcao, argumentos_ri);
    }
    
    std::cout << "Tipo de expressão não implementado ainda" << std::endl;
    return nullptr;
}

Exp* GeradorRI::gerarAcessoVariavel(const std::string& nome) {
    // Versão simplificada - retorna um temporary com o nome da variável
    // Em uma implementação completa, consultaria o frame para determinar
    // se a variável está na memória (usar ExpMem) ou em registrador (usar ExpTemp)
    
    std::cout << "Gerando acesso para variável: " << nome << std::endl;
    
    // Por simplicidade, assumindo que todas as variáveis são temporárias
    return ExpTemp::criar(0, nome);
}

Operador* GeradorRI::converterOperador(const std::string& op) {
    if (op == "+") return OperadorSoma::criar();
    if (op == "-") return OperadorSubtracao::criar();
    if (op == "*") return OperadorMultiplicacao::criar();
    if (op == "/") return OperadorDivisao::criar();
    if (op == "<=") return OperadorMenorIgual::criar();
    if (op == ">") return OperadorMaior::criar();
    if (op == "<") return OperadorMenor::criar();
    if (op == "==") return OperadorIgual::criar();
    
    std::cout << "ERRO: Operador " << op << " não reconhecido" << std::endl;
    return OperadorSoma::criar(); // fallback
}

void GeradorRI::imprimirRI(StmSeq* programa) {
    std::cout << "\n=== REPRESENTAÇÃO INTERMEDIÁRIA GERADA ===" << std::endl;
    if (programa) {
        programa->print();
    } else {
        std::cout << "ERRO: Programa RI é nulo" << std::endl;
    }
    std::cout << "===========================================" << std::endl;
}
