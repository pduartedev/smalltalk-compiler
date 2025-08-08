#ifndef _EXP_TEMP_HPP_
#define _EXP_TEMP_HPP_

#include "Exp.hpp"

class ExpTemp : public Exp {
public:
    int id;
    std::string nome;
    
    ExpTemp(int id, const std::string& nome = "");
    virtual std::string toString() const override;
    static ExpTemp* criar(int id, const std::string& nome = "");
};

// FramePointer é um tipo especial de Temp
class ExpTempFramePointer : public ExpTemp {
public:
    ExpTempFramePointer();
    virtual std::string toString() const override;
    static ExpTempFramePointer* criar();
};

#endif
