#ifndef _EXP_HPP_
#define _EXP_HPP_

#include <iostream>
#include <string>

class Exp {
public:
    virtual ~Exp() = default;
    virtual std::string toString() const = 0;
    virtual void print() const;
};

#endif
