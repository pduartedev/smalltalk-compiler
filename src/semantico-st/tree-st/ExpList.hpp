#ifndef EXPLIST_HPP
#define EXPLIST_HPP

#include "Exp.hpp"

class ExpList : public Exp
{
private:
    Exp *head;
    ExpList *tail;

public:
    ExpList(Exp *head, ExpList *tail);
    ~ExpList();

    std::string toString() const override;
    Exp *getHead() const;
    ExpList *getTail() const;
};

#endif
