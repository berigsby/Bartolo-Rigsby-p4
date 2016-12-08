#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "eval.h"

using namespace std;

const char *procs[] = {"cat", "grep", "less", "echo"};
const char *symbols[] = {"<", ">>", ">", "e>", "e>>"}; 

int total_pipes = 0;
int total_procs = 0;
int total_args = 0;
int process_args = 0;

string *arg_v; //the array of inputs which will be returned to shell
string *the_proc;

string std_in;
string std_out;
string std_err;

bool out_trunc;
bool err_trunc;

bool background = false;

/* constructor calls the function
 * which initializes all vars
 */
eval::eval(int the_argc, const char *the_argv[]){
  out_trunc = false;
  err_trunc = false;
  std_in = "STDIN_FILENO";
  std_out = "STDOUT_FILENO";
  std_err = "STDERR_FILENO";

  arg_v = new string[the_argc];
  the_proc = new string[100];
  initialize_vars(the_argc, the_argv);
    
} //eval

/* deconstructor to delete
 * items on the heap and reset
 * variables
 */
eval::~eval(){
  total_pipes = 0;
  total_procs = 0;
  total_args = 0;
  process_args = 0;
  std_in = "";
  std_out = "";
  std_err = "";
  out_trunc = false;
  err_trunc = false;
  background = false;
  delete [] arg_v;
  delete [] the_proc;
} //eval

/* returns the total number
 * of pipelines
 */
int eval::get_pipes(){
  return total_pipes;
} //get_pipes

/* returns the total number
 * of processes
 */
int eval::get_procs(){
  return total_procs;
} //get_procs

/* returns the string representation
 * of the standard in file
 */
string eval::get_std_in(){
  return std_in;
} //get_std_in

/* returns the string representation
 * of the standard out file
 */
string eval::get_std_out(){
  return std_out;
} //get_std_out

/* returns the string representation
 * of the standard error file
 */
string eval::get_std_err(){
  return std_err;
} //get_std_err

/* return the total 
 * number of arguments
 */
int eval::get_argc(){
  return total_args;
} //get_argc

/* returns the user input 
 * without the added quotations
 * if added in shell arguments
 */
string *eval::get_argv(){
  return arg_v;
} //get_argv

/* returns true if & was the last
 * char in user input or false
 * otherwise
 */
bool eval::is_background(){
  return background;
} //is_background

/**
 *
 *
 */
bool eval::get_out_trunc(){
  return out_trunc;
}//get_out_trunc

/**
 *
 *
 */
bool eval::get_err_trunc(){
  return err_trunc;
}//get_out_trunc

/* takes in the user arguments and
 * determines the number of pipes
 * and processes. Determines which
 * files are standard out, standard
 * in, and standard error. 
 */
void eval::initialize_vars(int argc, const char *argv[]){
  
  bool cont_loop = true;
  int array_pos = 0;

  if((strcmp(argv[1], "cat") != 0) && (strcmp(argv[1], "grep") != 0) &&  //if input is cd exit help etc...
     (strcmp(argv[1], "less") != 0) && (strcmp(argv[1], "echo") != 0)){
    for(int m = 1; m < argc; m++){
      arg_v[m-1] = argv[m];
      total_args++;
    } //for
    return;
  } //if

  for(int i = 1; i < argc; i++){
    if(strcmp(argv[i], "|") == 0){
      total_pipes++;
      arg_v[array_pos] = argv[i]; //update array
      array_pos++;
      total_args++;
    } //if

    for(int w = 0; w < 5; w++){ //search for symbols
      if(strcmp(argv[i], symbols[w]) == 0){
	string file(argv[i+1]);
	arg_v[array_pos] = argv[i]; //update array
	array_pos++;
	total_args++;
	arg_v[array_pos] = file; //update array
	array_pos++;
	total_args++;

	switch(w){
	case 0: std_in = file;      break;  
	case 1: std_out = file;     break;
	case 2: std_out = file; out_trunc = true;     break; 
	case 3: std_err = file; err_trunc = true;     break;
	case 4: std_err = file;     break;
	default:                    break;
	} //switch
      } //if
    } //for
    for(int j = 0; j < 4; j++){
      if(strcmp(argv[i], procs[j]) == 0){ //if argv[i] = cat, grep, less, echo
	total_args++;
	total_procs++;
	arg_v[array_pos] = argv[i]; //update array
	array_pos++;
       
	for(int x = i+1; x < argc; x++){ //go through elements of the current process
	  if((strcmp(argv[x], "|") == 0) || (strcmp(argv[x], "<") == 0) || //loop until | or std_fileno
	     (strcmp(argv[x], ">") == 0) || (strcmp(argv[x], ">>") == 0) ||
	     (strcmp(argv[x], "e>") == 0) || (strcmp(argv[x], "e>>") == 0)){
	    break;
	  } //if
	  
	  if(((strcmp(argv[x], "&") == 0)) && (x == argc - 1)){ //if & is the last char of user input
	    background = true;
	    return;
	  } //if

	  if(argv[x][0] == '"'){ //if user encloses a string with quotes
	    cont_loop = true;
	    string quote_content;

	    for(int g = x; cont_loop; g++){ //loop until the closing quote is found
	      for(unsigned int v = 0; v < strlen(argv[g]); v++){ //loop through each item's char
		if((g == x) && (v == 0)) continue; //bypass argv[x][0] since its an opening quote

		if((argv[g][v] == '\\') && (argv[g][v+1] == '\\')){ // two consecutive backslash procude a singe one 
		  quote_content += argv[g][v];
		  v++;
		} //if

		if(argv[g][v] == '"'){ //on read next "
		  if(v != 0){
		    if(argv[g][v-1] == '\\'){ //if \"
		      quote_content += argv[g][v];
		    } //if
		    else{ //if we found closing quote
		      cont_loop = false;
		    } //else
		  } //if
		} //if
		else{
		  if(argv[g][v] == '\\'){ //if \" do not include the blackslash
		    continue;
		  } //if
		  quote_content += argv[g][v]; //update the output string
		} //else
	      } //for
	      quote_content += " "; //space between each word
	      i++; //update i to avoid repetition of words
	    } //for
	    
	    string quote_element(quote_content);
	    arg_v[array_pos] = quote_element; //update array
	    array_pos++;
	    total_args++;
	    x = i; //update x to avoid repetition of words
	  } //if
	  else{
	    arg_v[array_pos] = argv[x]; //update array
	    array_pos++;
	    total_args++;
	  } //else
	} //for
      } //if
    } //for
  } //for
} //initialize_vars

/* returns a specific process
 * depending on the element
 * passed
 */
string *eval::get_process(int element){

  int iteration = 0;
  int x = 0;

  for(int i = 0; i < get_argc(); i++){
    if(arg_v[i] == "|"){
      iteration++;
    } //if
    
    if(iteration == element){
      if((arg_v[i] == "<") || (arg_v[i] == ">") || (arg_v[i] == ">>") 
	 || (arg_v[i] == "e>>") || (arg_v[i] == "e>")){
	break;
      } //if
      else if(arg_v[i] != "|"){
	the_proc[x] = arg_v[i];
	x++;
      } //else if
    } //if
  } //for

  return the_proc;
} //set_procs

/* returns the total arguments
 * of a specific process 
 * depending on element passed
 */
int eval::get_process_args(int element){

  int iteration = 0;
  int x = 0;

  for(int i = 0; i < get_argc(); i++){
    if(arg_v[i] == "|"){
      iteration++;
    } //if
    
    if(iteration == element){
      if((arg_v[i] == "<") || (arg_v[i] == ">") || (arg_v[i] == ">>") 
	 || (arg_v[i] == "e>>") || (arg_v[i] == "e>")){
	break;
      } //if
      else if(arg_v[i] != "|"){
	x++;
      } //else if
    } //if
  } //for

  return x;
} //get_process_args
