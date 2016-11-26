#ifndef EVAL_H
#define EVAL_H
#include <string.h>
#include <string>

//@author Timothy Bartolo
//811847393

using namespace std;

class eval{

 public:

  eval(int the_argc, const char *the_argv[]); //constructor calls initialize_vars function
  ~eval(); //deconstructor
  int get_pipes(); //return total pipes
  int get_procs(); //return total processes
  int get_argc(); //return the total number of arguments
  string *get_argv(); //return the array with user inputs
  string get_process_output(); //return string representation of information
  string get_std_in(); //return standard in file
  string get_std_out(); //return standard out file
  string get_std_err(); //return standard error file
  bool is_background(); //returns true if & is included at end of user input
  bool get_out_trunc();
  bool get_err_trunc();
  void initialize_vars(int argc, const char *argv[]); //initializes all variables
  string *get_process(int element); //return specific process
  int get_process_args(int element); //return args for a specific process
};

#endif
