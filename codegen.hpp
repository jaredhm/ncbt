#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <string>
#include <sstream>

class GenComponent{
protected:
  std::string _ex;
  std::stringstream& _ss;
  int _tabs = 0;  //Well-formatted code is important, even if it came from Brainfuck
  int _cells;
public:
  GenComponent(std::stringstream* ss,int c = 10000) : _ss(*ss), _cells(c) {}
  std::stringstream& getCodeAsStream() {return _ss;}
  std::string getExtension()  {return _ex;} //Handy way of having a generation component carry around the target file type
  void EmitCode(class OpList n);
  virtual void Prologue() = 0;  //Methods output includes, method and class declarations and end brackets
  virtual void Epilogue() = 0;
  virtual void EmitCode(class IncPtrOp n) = 0;
  virtual void EmitCode(class DecPtrOp n) = 0;
  virtual void EmitCode(class IncCellOp n) = 0;
  virtual void EmitCode(class DecCellOp n) = 0;
  virtual void EmitCode(class PrintCellOp n) = 0;
  virtual void EmitCode(class EatByteOp n) = 0;
  virtual void EmitCode(class LoopOp n) = 0;
};

class CGenComponent : public GenComponent{
  using GenComponent::getCodeAsStream;
  using GenComponent::getExtension;
public:
  CGenComponent(std::stringstream* ss,int c) : GenComponent(ss,c) {_ex = ".c";}
  void Prologue();
  void Epilogue();
  void EmitCode(class IncPtrOp n);
  void EmitCode(class DecPtrOp n);
  void EmitCode(class IncCellOp n);
  void EmitCode(class DecCellOp n);
  void EmitCode(class PrintCellOp n);
  void EmitCode(class EatByteOp n);
  void EmitCode(class LoopOp n);
};

class JavaGenComponent : public GenComponent{
  using GenComponent::getCodeAsStream;
  using GenComponent::getExtension;
public:
  JavaGenComponent(std::stringstream* ss, int c) : GenComponent(ss,c) {_ex = ".java";}
  void Prologue();
  void Epilogue();
  void EmitCode(class IncPtrOp n);
  void EmitCode(class DecPtrOp n);
  void EmitCode(class IncCellOp n);
  void EmitCode(class DecCellOp n);
  void EmitCode(class PrintCellOp n);
  void EmitCode(class EatByteOp n);
  void EmitCode(class LoopOp n);
};
#endif
