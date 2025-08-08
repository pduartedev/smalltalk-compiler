#ifndef _EXP_BINOP_HPP_
#define _EXP_BINOP_HPP_

#include "Exp.hpp"
#include "Operador.hpp"

class ExpBinop : public Exp {
public:
    Operador* operador;
    Exp* esquerda;
    Exp* direita;
    
    ExpBinop(Operador* operador, Exp* esquerda, Exp* direita);
    virtual ~ExpBinop();
    virtual std::string toString() const override;
    static ExpBinop* criar(Operador* operador, Exp* esquerda, Exp* direita);
};

#endif
