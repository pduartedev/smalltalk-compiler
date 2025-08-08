#include "StmMove.hpp"

StmMove::StmMove(Exp* destino, Exp* origem) : destino(destino), origem(origem) {}

StmMove::~StmMove() {
    delete destino;
    delete origem;
}

std::string StmMove::toString() const {
    return "MOVE(" + destino->toString() + ", " + origem->toString() + ")";
}

StmMove* StmMove::criar(Exp* destino, Exp* origem) {
    return new StmMove(destino, origem);
}
