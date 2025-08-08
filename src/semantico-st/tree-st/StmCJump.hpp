#ifndef _STM_CJUMP_HPP_
#define _STM_CJUMP_HPP_

#include "Stm.hpp"
#include "Exp.hpp"

class StmCJump : public Stm {
public:
    Exp* expressao_booleana;  // Substitui relop + left + right
    std::string label_verdadeiro;
    std::string label_falso;
    
    StmCJump(Exp* expressao_booleana, const std::string& label_verdadeiro, const std::string& label_falso);
    virtual ~StmCJump();
    virtual std::string toString() const override;
    static StmCJump* criar(Exp* expressao_booleana, const std::string& label_verdadeiro, const std::string& label_falso);
};

#endif
