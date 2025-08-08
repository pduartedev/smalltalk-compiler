#ifndef _GERADOR_RI_HPP_
#define _GERADOR_RI_HPP_

#include "tree-st/Stm.hpp"
#include "tree-st/Exp.hpp"
#include "tree-st/StmSeq.hpp"
#include "tree-st/StmMove.hpp"
#include "tree-st/StmExp.hpp"
#include "tree-st/StmCJump.hpp"
#include "tree-st/StmLabel.hpp"
#include "tree-st/ExpTemp.hpp"
#include "tree-st/ExpConst.hpp"
#include "tree-st/ExpMem.hpp"
#include "tree-st/ExpBinop.hpp"
#include "tree-st/ExpCall.hpp"
#include "tree-st/Operador.hpp"
#include "src-gram-st/Funcao.hpp"
#include "src-gram-st/Comando.hpp"
#include "src-gram-st/Expressao.hpp"
#include "frame-st/FrameFuncao.hpp"
#include "frame-st/FrameAcesso.hpp"
#include "frame-st/FrameAcessoNoFrame.hpp"
#include "frame-st/FrameAcessoTemp.hpp"
#include <map>
#include <string>

class GeradorRI {
private:
    int contador_temp;
    int contador_label;
    ExpTemp* frame_pointer;
    FrameFuncao* frame_atual;
    
public:
    GeradorRI();
    ~GeradorRI();
    
    // Método principal de conversão
    StmSeq* converterFuncao(Funcao* funcao, FrameFuncao* frame);
    
    // Métodos auxiliares
    Stm* converterComando(Comando* comando);
    Exp* converterExpressao(Expressao* expressao);
    Exp* gerarAcessoVariavel(const std::string& nome);
    
    // Métodos para geração de temporários e labels
    std::string novoLabel();
    ExpTemp* novoTemp();
    
    // Método para converter operadores
    Operador* converterOperador(const std::string& op);
    
    // Método para imprimir RI
    void imprimirRI(StmSeq* programa);
};

#endif
