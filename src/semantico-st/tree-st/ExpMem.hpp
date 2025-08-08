#ifndef _EXP_MEM_HPP_
#define _EXP_MEM_HPP_

#include "Exp.hpp"

class ExpMem : public Exp {
public:
    Exp* endereco;
    
    ExpMem(Exp* endereco);
    virtual ~ExpMem();
    virtual std::string toString() const override;
    static ExpMem* criar(Exp* endereco);
};

#endif
