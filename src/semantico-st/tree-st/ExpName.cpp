#include "ExpName.hpp"

ExpName::ExpName(const std::string &label) : label(label)
{
}

ExpName::~ExpName()
{
}

std::string ExpName::toString() const
{
    return "NAME(" + label + ")";
}

ExpName *ExpName::criar(const std::string &label)
{
    return new ExpName(label);
}
