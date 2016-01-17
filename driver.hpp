#ifndef PARSE_DRIVER_HPP
#define PARSE_DRIVER_HPP
#include "ast.hpp"

class Parser{
private:
    const char* _target;
    OpNode* _root;
    bool _accept;
public:
    Parser(const char* path = 0){
      _target = path;
      _accept = false;
    }
    bool run();
    OpNode* getAST() const {return _root;}
    void setAST(OpNode* r) {_root = r;}
    void printPrettySrc() {_root->print(std::cout,0);
                                 std::cout << "\n";}
    void accept() {_accept = true;}
    bool isAccepted() const {return _accept;}
};
#endif
