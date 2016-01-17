#include "codegen.hpp"
#include "ast.hpp"

void CGenComponent::Prologue(){
  _ss << "#include <stdio.h>" << std::endl;
  _ss << "#define VERY_LARGE_NUMBER " << _cells << std::endl;
  _ss << "int main(int argc, char** argv){" << std::endl;
  _ss << "char a[VERY_LARGE_NUMBER] = {0};" << std::endl;
  _ss << "char* p = &a[0];" << std::endl;
  _tabs = 1;  //Begin with a single tab
}

void CGenComponent::Epilogue(){
  _ss << "\treturn 0;" << std::endl << "}";
}

void CGenComponent::EmitCode(class OpList n){
  for(std::list<class OpNode*>::const_iterator it = n.getOps()->begin();
        it != n.getOps()->end(); it++){
      (*it)->accept(*this);
  }
}
void CGenComponent::EmitCode(class IncPtrOp n){
  _ss << std::string(_tabs,'\t') << "p++;" << std::endl;
}

void CGenComponent::EmitCode(class DecPtrOp n){
  _ss << std::string(_tabs,'\t') << "p--;" << std::endl;
}

void CGenComponent::EmitCode(class IncCellOp n){
  _ss << std::string(_tabs,'\t') << "(*p)++;" << std::endl;
}

void CGenComponent::EmitCode(class DecCellOp n){
  _ss << std::string(_tabs,'\t') << "(*p)--;" << std::endl;
}

void CGenComponent::EmitCode(class PrintCellOp n){
  _ss << std::string(_tabs,'\t') << "putchar(*p);" << std::endl;
}

void CGenComponent::EmitCode(class EatByteOp n){
  _ss << std::string(_tabs,'\t') << "*p = getchar();" << std::endl;
}

void CGenComponent::EmitCode(class LoopOp n){
  _ss << std::string(_tabs,'\t') << "while(*p){" << std::endl;
  _tabs++;  //Increment tabs for child op
  n.getChild()->accept(*this);
  _tabs--;  //Then bump back down
  _ss << std::string(_tabs,'\t') << "}" << std::endl;
}
