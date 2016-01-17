#include <iostream>
#include <iomanip>
#include <list>
#include "ast.hpp"
#include "codegen.hpp"

void OpList::accept(class GenComponent& gc){gc.EmitCode(*this);}
void IncPtrOp::accept(class GenComponent& gc){gc.EmitCode(*this);}
void DecPtrOp::accept(class GenComponent& gc){gc.EmitCode(*this);}
void IncCellOp::accept(class GenComponent& gc){gc.EmitCode(*this);}
void DecCellOp::accept(class GenComponent& gc){gc.EmitCode(*this);}
void PrintCellOp::accept(class GenComponent& gc){gc.EmitCode(*this);}
void EatByteOp::accept(class GenComponent& gc){gc.EmitCode(*this);}
void LoopOp::accept(class GenComponent& gc){gc.EmitCode(*this);}


std::ostream& OpList::print(std::ostream& os, int tabs) const {
  char c;
  os << std::string(3*tabs,0x20);
  for(std::list<OpNode*>::const_iterator it = _ops->begin();
      it != _ops->end(); it++){
        (*it)->print(os,tabs);
      }
  return os;
}

std::ostream& IncPtrOp::print(std::ostream& os, int tabs) const {
  return os << ">";
}

std::ostream& DecPtrOp::print(std::ostream& os, int tabs) const {
  return os << "<";
}

std::ostream& IncCellOp::print(std::ostream& os, int tabs) const {
  return os << "+";
}

std::ostream& DecCellOp::print(std::ostream& os, int tabs) const {
  return os << "-";
}

std::ostream& PrintCellOp::print(std::ostream& os, int tabs) const {
  return os << ".";
}

std::ostream& EatByteOp::print(std::ostream& os, int tabs) const {
  return os << ",";
}

std::ostream& LoopOp::print(std::ostream& os, int tabs) const {
  os << std::endl;
  os << std::string(3*tabs,0x20) << "[" << std::endl;

  _child->print(os,tabs + 1);
  os << std::endl;

  os << std::string(3*tabs,0x20) << "]" << std::endl;
  return os << std::string(3*tabs,0x20);
}
