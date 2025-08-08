#ifndef _OPERADOR_HPP_
#define _OPERADOR_HPP_

#include <string>

class Operador {
public:
    virtual ~Operador() = default;
    virtual std::string toString() const = 0;
};

class OperadorSoma : public Operador {
public:
    virtual std::string toString() const override { return "+"; }
    static OperadorSoma* criar() { return new OperadorSoma(); }
};

class OperadorSubtracao : public Operador {
public:
    virtual std::string toString() const override { return "-"; }
    static OperadorSubtracao* criar() { return new OperadorSubtracao(); }
};

class OperadorMultiplicacao : public Operador {
public:
    virtual std::string toString() const override { return "*"; }
    static OperadorMultiplicacao* criar() { return new OperadorMultiplicacao(); }
};

class OperadorDivisao : public Operador {
public:
    virtual std::string toString() const override { return "/"; }
    static OperadorDivisao* criar() { return new OperadorDivisao(); }
};

// Operadores de comparação (substituem relop)
class OperadorMenorIgual : public Operador {
public:
    virtual std::string toString() const override { return "<="; }
    static OperadorMenorIgual* criar() { return new OperadorMenorIgual(); }
};

class OperadorMaior : public Operador {
public:
    virtual std::string toString() const override { return ">"; }
    static OperadorMaior* criar() { return new OperadorMaior(); }
};

class OperadorMenor : public Operador {
public:
    virtual std::string toString() const override { return "<"; }
    static OperadorMenor* criar() { return new OperadorMenor(); }
};

class OperadorIgual : public Operador {
public:
    virtual std::string toString() const override { return "=="; }
    static OperadorIgual* criar() { return new OperadorIgual(); }
};

#endif
