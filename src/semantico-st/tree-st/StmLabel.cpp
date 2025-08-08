#include "StmLabel.hpp"

StmLabel::StmLabel(const std::string& nome) : nome(nome) {}

std::string StmLabel::toString() const {
    return "LABEL(" + nome + ")";
}

StmLabel* StmLabel::criar(const std::string& nome) {
    return new StmLabel(nome);
}
