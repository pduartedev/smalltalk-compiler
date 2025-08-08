#ifndef _STM_LABEL_HPP_
#define _STM_LABEL_HPP_

#include "Stm.hpp"

class StmLabel : public Stm {
public:
    std::string nome;
    
    StmLabel(const std::string& nome);
    virtual std::string toString() const override;
    static StmLabel* criar(const std::string& nome);
};

#endif
