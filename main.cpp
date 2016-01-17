#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "driver.hpp"
#include "ast.hpp"
#include "codegen.hpp"


void croak(const char* s);
void croak(std::string& s);
void LowerCase(char* s, int sz);

const char* helpStr =
"ncbt, or, the Needlessly Complex Brainfuck Translater. Usage:\n\n"
    "\tncbt [input_file_name.(b|bf)] [-t target_language] [-o output_file_name] [-p] [-n #####] [-h]\n\n"
 "There\'s really no wrong way to use ncbt. All of the above flags are optional, and can be provided in any order:\n\n"
    "\t \'input_file_name.(b|bf)\' is the Brainfuck source. \n\tYou can tell ncbt to eat source this way, or redirect to stdin from a file using the \'<\' operation.\n\n"
    "\t \'target_language\' is a string representing the target language. \n\tDefault is c. Current options are: [\'c\', \'java\'].\n\n"
    "\t \'output_file_name\' will be the name of the output file. \n\tAlso, look, don't put a file extension at the end of this, the compiler will do it for you.\n\n"
    "\t \'-p\' prints a nice, pretty version of your ugly-ass source code to the console.\n\n"
    "\t \'-n #####\' allocates ##### cells (bytes) for the BF-machine head to read over and write to.\n\n"
    "\t \'-h\' or \'-help\' print the message that you're currently reading. But you probably already knew that. \n\tOh, it also prevents any translation.\n\n";

int main(int argc, char* argv[]){
  //This will much more complex when deployed as a true-to-form console application
  Parser* p = 0;
  GenComponent* gc;
  std::string flags[] = {"-t", //(-t)arget, the target language
                        "-o",  //(-o)utput, the output destination
                        "-p",  //(-p)rint a nice, pretty version of the source code as dictated by the AST
                        "-n",  //(-n)umber of cells, allows the coder to specify how many bytes the pointer head can move around in
                        "-h", "-help"}; //(-h)elp (duh)
  int n = argc;
  int cells = 10000;  //Default
  std::ofstream outputFile;
  std::string targetLang;
  std::string outputName = "alan";
  std::string fileName = "";
  std::string pathStr;
  std::string errStr;

  while(n){
    if(flags[4].compare(argv[argc-n]) == 0 ||
       flags[5].compare(argv[argc-n]) == 0){
         std::cout << helpStr;
         return 1;
    }
    n--;
  }

  n = argc;
  while(n){
    if(flags[3].compare(argv[argc-n]) == 0){
      if(n <= 1)
        croak("Use of \'-n\' without specifing a number of cells to allocate");
      try{
        cells = std::stoi(std::string(argv[1+argc-n]));
      }
      catch(const std::invalid_argument& ex){
        errStr = "Unable to convert the following to an integer - ";
        errStr += argv[1+argc-n];
        croak(errStr); //Catch and die;  I'd prefer the process end on my terms
      }
    }
    n--;
  }

  n = argc;
  //grab the target lang, or default to c, don't matter to ncbt
  while(n){
    if(flags[0].compare(argv[argc-n]) == 0){
      if(n <= 1)  //Whoops, user typed something like './ncbt blah blah -t'
        croak("Use of \'-t\' flag without specifing a target language");
      LowerCase(argv[1+argc-n], strlen(argv[1+argc-n]));  //Convert to lowercase
      targetLang = argv[1+argc-n];
      break;
    }
    n--;
  }


  if(!n)
    gc = new CGenComponent(new std::stringstream(),cells);
  else{
    if(targetLang.compare("java") == 0){
      //TODO : gc = new JavaGenComponent(new std::stringstream(),cells);
    }
    else if (targetLang.compare("c") == 0) {
      gc = new CGenComponent(new std::stringstream(),cells);
    }
    else{
      errStr = "unrecognzed target language - ";
      errStr += targetLang;
      croak(errStr);
    }
  }

  //Now we grab the target file path, if the user specified one

  if(argc >= 2)
    pathStr = argv[1];  //Will be the second argument
  if(access(pathStr.c_str(),F_OK) != -1){ //Verify path exists
    if(pathStr.substr(pathStr.find_last_of(".") + 1) == "bf" ||
       pathStr.substr(pathStr.find_last_of(".") + 1) == "b")
         p = new Parser(pathStr.c_str());
  }

  if(p == 0)
    p = new Parser(); //read from stdin

  //Run first two compilation phases, lexing and parsing
  bool noErr = p->run();
  if(!(p->isAccepted() && noErr)){
    std::cout << "An error occured while parsing, bummer!" << std::endl;
    return 1;
  }

  n = argc;
  while(n){
    if(flags[2].compare(argv[argc - n]) == 0)
      p->printPrettySrc();  //Print that sh*t
    n--;
  }

  gc->Prologue();
  p->getAST()->accept(*gc);
  gc->Epilogue();

  //Time to write to a file. Which file? Well, let's look at the '-o' argument (if it exists)
  n = argc;
  while(n){
    if(flags[1].compare(argv[argc-n]) == 0){
      if(n <= 1)
        croak("Use of \'-o\' flag without providing any output file name");
      outputName = argv[1+argc-n];
    }
    n--;
  }

  outputName += gc->getExtension();

  outputFile.open(outputName); //Begin writing output;
  if(outputFile.is_open()){
    outputFile << gc->getCodeAsStream().str();  //Whew! All done; that was a workout
    outputFile.close();
  }
  else{
    errStr = "Something went wrong writing to file ";
    errStr += outputName;
    croak(errStr);
  }

  return 1;
}


void croak(const char* s){
  std::string str (s);
  croak(str);
}

//Dies, and prints a (hopefully) meaniningful message to stderr
void croak(std::string& s){
  std::cerr << "Fatal error, dying now:" << std::endl << "\t" << s << "\n";
  std::exit(EXIT_FAILURE);
}

//Returns a (approximate, symbols are going to mess with things) lowercase version of the input string
void LowerCase(char* s, int len){
  int i = 0;
  while(len-i){
    if (s[i] < 'a')
      s[i] = s[i] - 'A' + 'a';
    i++;
  }
}
