#include "NUM.hpp"
#include <stdlib.h>

NUM::NUM() { }

NUM* NUM::extrai_NUM(No_arv_parse* no){
  // 23) F -> NUM
  NUM* res = new NUM();
  res->valor = atoi(no->dado_extra.c_str());
  return res;
}
