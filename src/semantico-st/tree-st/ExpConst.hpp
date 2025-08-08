#ifndef _EXP_CONST_HPP_
#define _EXP_CONST_HPP_

#include "Exp.hpp"

class ExpConst : public Exp {
public:
    int valor;
    
    ExpConst(int valor);
    virtual std::string toString() const override;
    static ExpConst* criar(int valor);
};

#endif
