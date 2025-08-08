#ifndef _FRAME_ACESSO_NO_FRAME_HPP_
#define _FRAME_ACESSO_NO_FRAME_HPP_

#include "FrameAcesso.hpp"

class FrameAcessoNoFrame : public FrameAcesso {
public:
  int posicao_no_frame;
  // Acesso sera FP+posicao_no_frame. Exemplos
  // posicao_no_frame=-40, -48, -56, etc.
  FrameAcessoNoFrame();
  FrameAcessoNoFrame(int posicao);
  
  virtual bool eh_no_frame() const override { return true; }
};

#endif
