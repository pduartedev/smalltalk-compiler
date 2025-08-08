#include "ExpressaoBinaria.hpp"
#include "Tipo.hpp"
#include "../debug-util.hpp"
#include <iostream>
#include <cmath>
using namespace std;

ExpressaoBinaria::ExpressaoBinaria(Expressao* esq, const string& op, Expressao* dir) 
  : esquerda(esq), operador(op), direita(dir) {
  tipo_resultado = nullptr;
}

ExpressaoBinaria::~ExpressaoBinaria() {
  // Não deletamos esquerda e direita aqui para evitar problemas de ownership
  // O sistema de garbage collection deve ser tratado de forma mais cuidadosa
}

void ExpressaoBinaria::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "ExpressaoBinaria: " << operador << endl;
  if (esquerda) {
    tab3(tab + 1);
    cerr << "Esquerda:" << endl;
    esquerda->debug_com_tab(tab + 2);
  }
  if (direita) {
    tab3(tab + 1);
    cerr << "Direita:" << endl;
    direita->debug_com_tab(tab + 2);
  }
}

Tipo* ExpressaoBinaria::verificar_tipos(const vector<Variavel*>& variaveis, const vector<Variavel*>& parametros) {
  if (!esquerda || !direita) {
    return nullptr;
  }
  
  Tipo* tipo_esq = esquerda->verificar_tipos(variaveis, parametros);
  Tipo* tipo_dir = direita->verificar_tipos(variaveis, parametros);
  
  // Regras de tipo para operações binárias
  if (operador == "+" || operador == "-" || operador == "*" || operador == "/" || operador == "%") {
    // Em Smalltalk, Object pode aceitar qualquer operação - inferência dinâmica
    if ((tipo_esq && tipo_esq->nome == "Object") || (tipo_dir && tipo_dir->nome == "Object")) {
      tipo_resultado = Tipo::OBJECT_TYPE();
    } else if ((tipo_esq && tipo_esq->nome == "Float") || (tipo_dir && tipo_dir->nome == "Float")) {
      tipo_resultado = Tipo::FLOAT_TYPE();
    } else if ((tipo_esq && tipo_esq->nome == "Integer") && (tipo_dir && tipo_dir->nome == "Integer")) {
      tipo_resultado = Tipo::INTEGER_TYPE();
    } else {
      // Para compatibilidade, assume Object como tipo de resultado
      tipo_resultado = Tipo::OBJECT_TYPE();
    }
  } else if (operador == "<" || operador == ">" || operador == "<=" || operador == ">=" || operador == "==" || operador == "~=") {
    // Operadores relacionais sempre retornam Boolean
    tipo_resultado = Tipo::BOOLEAN_TYPE();
  } else {
    return nullptr;
  }
  
  return tipo_resultado;
}

double ExpressaoBinaria::calcular_valor(const map<string, double>& valores_variaveis) {
  if (!esquerda || !direita) {
    return 0.0;
  }
  
  double val_esq = esquerda->calcular_valor(valores_variaveis);
  double val_dir = direita->calcular_valor(valores_variaveis);
  
  if (operador == "+") {
    return val_esq + val_dir;
  } else if (operador == "-") {
    return val_esq - val_dir;
  } else if (operador == "*") {
    return val_esq * val_dir;
  } else if (operador == "/") {
    if (val_dir != 0.0) {
      return val_esq / val_dir;
    } else {
      cerr << "Erro: Divisão por zero!" << endl;
      return 0.0;
    }
  } else if (operador == "%") {
    if (val_dir != 0.0) {
      return fmod(val_esq, val_dir);
    } else {
      cerr << "Erro: Módulo por zero!" << endl;
      return 0.0;
    }
  } else if (operador == "<") {
    return (val_esq < val_dir) ? 1.0 : 0.0;
  } else if (operador == ">") {
    return (val_esq > val_dir) ? 1.0 : 0.0;
  } else if (operador == "<=") {
    return (val_esq <= val_dir) ? 1.0 : 0.0;
  } else if (operador == ">=") {
    return (val_esq >= val_dir) ? 1.0 : 0.0;
  } else if (operador == "==") {
    return (abs(val_esq - val_dir) < 1e-10) ? 1.0 : 0.0;
  } else if (operador == "~=") {
    return (abs(val_esq - val_dir) >= 1e-10) ? 1.0 : 0.0;
  }
  
  return 0.0;
}
