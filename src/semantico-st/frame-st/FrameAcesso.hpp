#ifndef _FRAME_ACESSO_HPP_
#define _FRAME_ACESSO_HPP_
using namespace std;

class FrameAcesso {
public:
  virtual ~FrameAcesso() = default; // Torna a classe polimórfica
  virtual bool eh_no_frame() const = 0; // Método para identificar o tipo
};

#endif
