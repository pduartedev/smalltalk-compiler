#ifndef _STM_HPP_
#define _STM_HPP_

#include <iostream>
#include <string>

class Stm {
public:
    virtual ~Stm() = default;
    virtual std::string toString() const = 0;
    virtual void print() const;
};

#endif
