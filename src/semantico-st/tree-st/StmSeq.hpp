#ifndef _STM_SEQ_HPP_
#define _STM_SEQ_HPP_

#include "Stm.hpp"
#include <vector>

class StmSeq : public Stm {
public:
    std::vector<Stm*> statements;
    
    StmSeq(const std::vector<Stm*>& statements = {});
    virtual ~StmSeq();
    virtual std::string toString() const override;
    void adicionar(Stm* stm);
    static StmSeq* criar(const std::vector<Stm*>& statements = {});
};

#endif
