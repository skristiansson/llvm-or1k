#ifndef OR1KDISASSEMBLER_H
#define OR1KDISASSEMBLER_H

#include "llvm/MC/MCDisassembler.h"

namespace llvm {

class MCInst;
class MemoryObject;
class raw_ostream;

struct EDInstInfo;

class OR1KDisassembler : public MCDisassembler {
public:
  /// Constructor     - Initializes the disassembler.
  ///
  OR1KDisassembler(const MCSubtargetInfo &STI, const MCRegisterInfo *Info) :
    MCDisassembler(STI), RegInfo(Info) {
  }

  ~OR1KDisassembler() {
  }

  /// getInstruction - See MCDisassembler.
  MCDisassembler::DecodeStatus getInstruction(MCInst &instr,
                      uint64_t &size,
                      const MemoryObject &region,
                      uint64_t address,
                      raw_ostream &vStream,
                      raw_ostream &cStream) const;

  /// getEDInfo - See MCDisassembler.
  const EDInstInfo *getEDInfo() const;

  const MCRegisterInfo *getRegInfo() const { return RegInfo; }

private:
  const MCRegisterInfo *RegInfo;
};

} // namespace llvm

#endif
