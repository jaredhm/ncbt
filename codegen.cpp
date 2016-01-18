#include "codegen.hpp"
#include "ast.hpp"

void GenComponent::EmitCode(class OpList n){
  for(std::list<class OpNode*>::const_iterator it = n.getOps()->begin();
  it != n.getOps()->end(); it++){
    (*it)->accept(*this);
  }
}

//Implmentation for C as the target language
void CGenComponent::Prologue(){
  _ss << "#include <stdio.h>" << std::endl;
  _ss << "#define VERY_LARGE_NUMBER " << _cells << std::endl;
  _ss << "int main(int argc, char** argv){" << std::endl;
  _ss << "char a[VERY_LARGE_NUMBER] = {0};" << std::endl;
  _ss << "char* p = &a[0];" << std::endl;
  _tabs++;  //Begin with a single tab
}

void CGenComponent::Epilogue(){
  _ss << std::string(_tabs,'\t') << "return 0;" << std::endl;
  _ss << "}" << std::endl;
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

//Java target implementation
void JavaGenComponent::Prologue(){
  //TODO: sort out main class name & target file name agreement (java is like really annoying)
  _ss << "public class Main{" << std::endl;
  _tabs++;
  //Now for the good ole' main() function declaration that will forever remain stuck in my head
  _ss << std::string(_tabs,'\t') << "public static void main(String[] args) throws java.io.IOException{" << std::endl;
  _ss << std::string(_tabs,'\t') << "char[] a = new char[" << _cells << "];" << std::endl;
  _ss << std::string(_tabs,'\t') << "int i = 0;" << std::endl; //Hopefully _cells isn't greater than 2^32 - 1, though you can't index an array with a long in java anywho
  _tabs++;
}

void JavaGenComponent::Epilogue(){
  _tabs--;
  _ss << std::string(_tabs,'\t') << "}" << std::endl;
  _tabs--;
  _ss << std::string(_tabs,'\t') << "}" << std::endl;
}

void JavaGenComponent::EmitCode(class IncPtrOp n){
  _ss << std::string(_tabs,'\t') << "i++;" << std::endl;
}

void JavaGenComponent::EmitCode(class DecPtrOp n){
  _ss << std::string(_tabs,'\t') << "i--;" << std::endl;
}

void JavaGenComponent::EmitCode(class IncCellOp n){
  _ss << std::string(_tabs,'\t') << "a[i]++;" << std::endl;
}

void JavaGenComponent::EmitCode(class DecCellOp n){
  _ss << std::string(_tabs,'\t') << "a[i]--;" << std::endl;
}

void JavaGenComponent::EmitCode(class PrintCellOp n){
  _ss << std::string(_tabs,'\t') << "System.out.print(a[i]);" << std::endl;
}

void JavaGenComponent::EmitCode(class EatByteOp n){
  _ss << std::string(_tabs,'\t') << "a[i] = (char) System.in.read();" << std::endl;
}

void JavaGenComponent::EmitCode(class LoopOp n){
  _ss << std::string(_tabs,'\t') << "while(a[i] != \'\\u0000\'){" << std::endl;
  _tabs++;
  n.getChild()->accept(*this);
  _tabs--;
  _ss << std::string(_tabs,'\t') << "}" << std::endl;
}
