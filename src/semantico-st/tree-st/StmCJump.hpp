#ifndef _STM_CJUMP_HPP_
#define _STM_CJUMP_HPP_

#include "Stm.hpp"
#include "Exp.hpp"

class StmCJump : public Stm
{
public:
    // Versão com operador e operandos separados (padrão Appel)
    std::string operador; // GT, LT, EQ, etc.
    Exp *esquerda;
    Exp *direita;
    std::string label_verdadeiro;
    std::string label_falso;

    StmCJump(const std::string &operador, Exp *esquerda, Exp *direita, const std::string &label_verdadeiro, const std::string &label_falso);
    virtual ~StmCJump();
    virtual std::string toString() const override;
    static StmCJump *criar(const std::string &operador, Exp *esquerda, Exp *direita, const std::string &label_verdadeiro, const std::string &label_falso);
};

#endif
