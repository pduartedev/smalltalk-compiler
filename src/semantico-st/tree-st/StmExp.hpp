#ifndef _STM_EXP_HPP_
#define _STM_EXP_HPP_

#include "Stm.hpp"
#include "Exp.hpp"

class StmExp : public Stm {
public:
    Exp* expressao;
    
    StmExp(Exp* expressao);
    virtual ~StmExp();
    virtual std::string toString() const override;
    static StmExp* criar(Exp* expressao);
};

#endif
