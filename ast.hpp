#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <iomanip>
#include <list>

  class OpNode{
  public:
    OpNode() {}
    virtual void accept(class GenComponent& gc) = 0;
    virtual std::ostream& print(std::ostream& os, int tabs) const = 0;
  };

  class OpList : public OpNode{
  private:
    std::list<OpNode*>* _ops;
  public:
    void accept(class GenComponent& gc);
    OpList() {_ops = new std::list<OpNode*>;}
    std::ostream& print(std::ostream& os, int tabs) const;
    std::list<OpNode*>* getOps() const {return _ops;}
  };

  class IncPtrOp : public OpNode{
  public:
    void accept(class GenComponent& gc);
    IncPtrOp() : OpNode() {}
    std::ostream& print(std::ostream& os, int tabs) const;
  };

  class DecPtrOp : public OpNode {
  public:
    void accept(class GenComponent& gc);
    DecPtrOp() : OpNode() {}
    std::ostream& print(std::ostream& os, int tabs) const;
  };

  class IncCellOp : public OpNode {
  public:
    void accept(class GenComponent& gc);
    IncCellOp() : OpNode() {}
    std::ostream& print(std::ostream& os, int tabs) const;
  };

  class DecCellOp : public OpNode {
  public:
    void accept(class GenComponent& gc);
    DecCellOp() : OpNode() {}
    std::ostream& print(std::ostream& os, int tabs) const;
  };

  class PrintCellOp : public OpNode {
  public:
    void accept(class GenComponent& gc);
    PrintCellOp() : OpNode() {}
    std::ostream& print(std::ostream& os, int tabs) const;
  };

  class EatByteOp : public OpNode {
  public:
    void accept(class GenComponent& gc);
    EatByteOp() : OpNode() {}
    std::ostream& print(std::ostream& os, int tabs) const;
  };

  class LoopOp : public OpNode{
  private:
    OpNode* _child;
  public:
    void accept(class GenComponent& gc);
    LoopOp(OpNode* c) : _child(c) {}
    OpNode* getChild(){return _child;}
    std::ostream& print(std::ostream& os, int tabs) const;
  };
#endif
