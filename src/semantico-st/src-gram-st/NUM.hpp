#ifndef _NUMERO_HPP_
#define _NUMERO_HPP_
#include "../Arvore.hpp"
using namespace std;

class NUM {
public:
  int valor;
  NUM();
  static NUM * extrai_NUM(No_arv_parse* no);
};

#endif
