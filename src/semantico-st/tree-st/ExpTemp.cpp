#include "ExpTemp.hpp"
#include <sstream>

// ExpTemp
ExpTemp::ExpTemp(int id, const std::string& nome) : id(id), nome(nome) {}

std::string ExpTemp::toString() const {
    if (!nome.empty()) {
        return nome;
    }
    return "T" + std::to_string(id);
}

ExpTemp* ExpTemp::criar(int id, const std::string& nome) {
    return new ExpTemp(id, nome);
}

// ExpTempFramePointer
ExpTempFramePointer::ExpTempFramePointer() : ExpTemp(-1, "FP") {}

std::string ExpTempFramePointer::toString() const {
    return "FP";
}

ExpTempFramePointer* ExpTempFramePointer::criar() {
    return new ExpTempFramePointer();
}
