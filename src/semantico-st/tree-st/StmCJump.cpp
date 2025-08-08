#include "StmCJump.hpp"

StmCJump::StmCJump(const std::string &operador, Exp *esquerda, Exp *direita, const std::string &label_verdadeiro, const std::string &label_falso)
    : operador(operador), esquerda(esquerda), direita(direita), label_verdadeiro(label_verdadeiro), label_falso(label_falso) {}

StmCJump::~StmCJump()
{
    delete esquerda;
    delete direita;
}

std::string StmCJump::toString() const
{
    return "CJUMP(" + operador + ", " + esquerda->toString() + ", " + direita->toString() + ", " +
           label_verdadeiro + ", " + label_falso + ")";
}

StmCJump *StmCJump::criar(const std::string &operador, Exp *esquerda, Exp *direita, const std::string &label_verdadeiro, const std::string &label_falso)
{
    return new StmCJump(operador, esquerda, direita, label_verdadeiro, label_falso);
}
