#ifndef _TIPO_HPP_
#define _TIPO_HPP_

#include <string>
using namespace std;

enum class TipoSmallTalk {
    INTEGER,
    FLOAT,
    BOOLEAN,
    OBJECT,
    UNDEFINED
};

class Tipo {
public:
    TipoSmallTalk tipo;
    string nome;
    
    Tipo(TipoSmallTalk t, const string& n) : tipo(t), nome(n) {}
    
    static Tipo* INTEGER_TYPE() {
        static Tipo* instance = new Tipo(TipoSmallTalk::INTEGER, "Integer");
        return instance;
    }
    
    static Tipo* FLOAT_TYPE() {
        static Tipo* instance = new Tipo(TipoSmallTalk::FLOAT, "Float");
        return instance;
    }
    
    static Tipo* BOOLEAN_TYPE() {
        static Tipo* instance = new Tipo(TipoSmallTalk::BOOLEAN, "Boolean");
        return instance;
    }
    
    static Tipo* OBJECT_TYPE() {
        static Tipo* instance = new Tipo(TipoSmallTalk::OBJECT, "Object");
        return instance;
    }
    
    bool compativel_com(Tipo* outro);
    bool pode_fazer_operacao_aritmetica();
    bool pode_fazer_operacao_logica();
    string to_string();
};

#endif
