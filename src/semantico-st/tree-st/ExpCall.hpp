#ifndef _EXP_CALL_HPP_
#define _EXP_CALL_HPP_

#include "Exp.hpp"
#include <vector>

class ExpCall : public Exp {
public:
    Exp* funcao;
    std::vector<Exp*> argumentos;
    
    ExpCall(Exp* funcao, const std::vector<Exp*>& argumentos = {});
    virtual ~ExpCall();
    virtual std::string toString() const override;
    static ExpCall* criar(Exp* funcao, const std::vector<Exp*>& argumentos = {});
};

#endif
