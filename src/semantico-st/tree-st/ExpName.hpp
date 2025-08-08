#ifndef _EXP_NAME_HPP_
#define _EXP_NAME_HPP_

#include "Exp.hpp"

class ExpName : public Exp
{
public:
    std::string label;

    ExpName(const std::string &label);
    virtual ~ExpName();

    static ExpName *criar(const std::string &label);

    virtual std::string toString() const override;
};

#endif // _EXP_NAME_HPP_
