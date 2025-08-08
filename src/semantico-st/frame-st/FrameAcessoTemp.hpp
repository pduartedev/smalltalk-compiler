#ifndef _FRAME_ACESSO_TEMP_HPP_
#define _FRAME_ACESSO_TEMP_HPP_

#include "FrameAcesso.hpp"

class FrameAcessoTemp : public FrameAcesso {
public:
  int id; // id unico do pseudo-registrador/temporario.
  FrameAcessoTemp();
  FrameAcessoTemp(int id_temp);
  
  virtual bool eh_no_frame() const override { return false; }
};

#endif
