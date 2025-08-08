#include "ExpBinop.hpp"

ExpBinop::ExpBinop(Operador* operador, Exp* esquerda, Exp* direita) 
    : operador(operador), esquerda(esquerda), direita(direita) {}

ExpBinop::~ExpBinop() {
    delete operador;
    delete esquerda;
    delete direita;
}

std::string ExpBinop::toString() const {
    return "BINOP(" + operador->toString() + ", " + 
           esquerda->toString() + ", " + 
           direita->toString() + ")";
}

ExpBinop* ExpBinop::criar(Operador* operador, Exp* esquerda, Exp* direita) {
    return new ExpBinop(operador, esquerda, direita);
}
