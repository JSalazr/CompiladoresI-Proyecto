#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include "expr_lexer.h"
#include "expr_parser.h"

using namespace std;

int main(int argc, char* argv[]){
  string line;
  ifstream myfile (argv[1]);
 
  Lexer lexer (myfile);
  Parser parser (lexer);

  parser.parse();

  myfile.close();
}
