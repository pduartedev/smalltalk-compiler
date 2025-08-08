#ifndef _STM_MOVE_HPP_
#define _STM_MOVE_HPP_

#include "Stm.hpp"
#include "Exp.hpp"

class StmMove : public Stm {
public:
    Exp* destino;
    Exp* origem;
    
    StmMove(Exp* destino, Exp* origem);
    virtual ~StmMove();
    virtual std::string toString() const override;
    static StmMove* criar(Exp* destino, Exp* origem);
};

#endif
