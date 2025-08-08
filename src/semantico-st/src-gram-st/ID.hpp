#ifndef _ID_HPP_
#define _ID_HPP_
#include<string>
#include "../Arvore.hpp"
using namespace std;

class ID {
public:
  string nome;
  static ID* extrai_ID(No_arv_parse* no);
};

#endif
