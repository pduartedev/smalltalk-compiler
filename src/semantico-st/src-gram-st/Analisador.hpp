#ifndef _ANALISADOR_HPP_
#define _ANALISADOR_HPP_
#include "Funcao.hpp"
#include "Tipo.hpp"
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

class Analisador {
public:
  bool verificar_semantica(Funcao *f);
  double calcula_ultimo_valor(Funcao *f, const vector<double> &params);
  void imprimir_resultado(double valor, Tipo* tipo);
};
  
#endif
