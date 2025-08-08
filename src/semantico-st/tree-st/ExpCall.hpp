#ifndef _EXP_CALL_HPP_
#define _EXP_CALL_HPP_

#include "Exp.hpp"
#include <vector>

// Forward declaration
class ExpList;

class ExpCall : public Exp
{
public:
    Exp *funcao;
    ExpList *argumentos;

    ExpCall(Exp *funcao, ExpList *argumentos = nullptr);
    virtual ~ExpCall();
    virtual std::string toString() const override;
    static ExpCall *criar(Exp *funcao, ExpList *argumentos = nullptr);

    // Método auxiliar para converter vector para ExpList
    static ExpList *criarExpList(const std::vector<Exp *> &args);
};

#endif
