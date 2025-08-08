#include "StmJump.hpp"

StmJump::StmJump(Exp *destino) : destino(destino)
{
}

StmJump::~StmJump()
{
    delete destino;
}

std::string StmJump::toString() const
{
    return "JUMP(" + destino->toString() + ")";
}

StmJump *StmJump::criar(Exp *destino)
{
    return new StmJump(destino);
}
