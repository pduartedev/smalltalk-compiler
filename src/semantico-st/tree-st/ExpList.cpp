#include "ExpList.hpp"

ExpList::ExpList(Exp *head, ExpList *tail) : head(head), tail(tail) {}

ExpList::~ExpList()
{
    delete head;
    delete tail;
}

std::string ExpList::toString() const
{
    if (head == nullptr)
    {
        return "null";
    }

    std::string result = "ExpList(\n                " + head->toString();
    if (tail != nullptr)
    {
        result += ",\n                " + tail->toString();
    }
    else
    {
        result += ",\n                null";
    }
    result += "\n            )";
    return result;
}

Exp *ExpList::getHead() const
{
    return head;
}

ExpList *ExpList::getTail() const
{
    return tail;
}
