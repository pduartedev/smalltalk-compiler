#ifndef _STM_JUMP_HPP_
#define _STM_JUMP_HPP_

#include "Stm.hpp"
#include "Exp.hpp"

class StmJump : public Stm
{
public:
    Exp *destino;

    StmJump(Exp *destino);
    virtual ~StmJump();

    static StmJump *criar(Exp *destino);

    virtual std::string toString() const override;
};

#endif // _STM_JUMP_HPP_
