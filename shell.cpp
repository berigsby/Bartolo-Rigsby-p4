#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "eval.h"
#include <signal.h>
#include <sys/wait.h>

//@author Timothy Bartol
//811847393

/* program simulates the input
 * screen of the UNIX terminal.
 * processes, pipelines, and
 * files are read accordingly
 */
using namespace std;

eval get_input_info();
string get_prompt();

/* main takes in user input and
 * passes it to eval class. Then
 * calls getter functions to display
 * all information regarding commands
 */
int main(int argc, const char *argv[]){

  cout.setf(std::ios_base::unitbuf); //turn off buffering for cout
  pid_t pid, wpid; //various PIDs
  int pstatus; //process pstatus

  bool dead = false;
    
  while(!dead){
    eval *ev = new eval(get_input_info()); //get user input information
    string *arg_v = ev -> get_argv();

    //    for(int i = 0; i < ev -> get_argc(); i++){
    //  cout << arg_v[i] << "\n";
    //} //for

    if(ev -> get_procs() == 0){ //check for other commands other than processes
      if(arg_v[0] == "exit"){ //exit
	if(ev -> get_argc() == 2){ //exit (int)
	  exit(stoi(arg_v[1]));
	} //if
	else{ //exit (no int)
	  exit(pstatus);
	} //else
      } //if
      else if(arg_v[0] == "cd"){ //cd
	if(ev -> get_argc() > 2){
	  printf("%s: invalid argument length\n", arg_v[0].c_str());
	} //if
	else if(ev -> get_argc() == 1){
	  chdir(getenv("HOME"));
	} //else if
	else if(chdir(arg_v[1].c_str()) < 0){
	  perror("change directory");
	} //if
      } //else if
      else if(arg_v[0] == "help"){ //help
	//TODO implement help information
      } //else if 
    } //if
      
    if((pid = fork()) < 0) { // error 
      perror("FORK ERROR");
    } //if
    else if (pid == 0) { //child
      //char ** args = new char * [3];
      //args[0] = strdup("cat");
      //args[1] = strdup("eval.cpp");
      //args[2] = nullptr;
      execvp(arg_v[0], arg_v);
    } //else if (child)
    else{ //parent
      if(false){//ev -> is_background());
      } else{
	waitpid(pid,nullptr,0);
	/*
	if ((wpid = waitpid(pid, &pstatus, WNOHANG)) == -1) {
	  perror("waitpid");
	} //if
	else if (WIFEXITED(pstatus)){ //if child process exits
	  //cout << "exited\n";
	  //dead = true;
	} //else if child exited
	else if (WIFSIGNALED(pstatus)){ ///if child process returns signal
	  //cout << "signaled\n";
	  //dead = true;
	} //else if child signaled
	*/
      }//else isback
    }//else
    delete ev; //call deconstructor to reset variables
  } //while
} //main

/* function displays prompt to user and
 * sends user input to the evaluate class
 * to break it down into useful information
 */
eval get_input_info(){

  string input;
  string word;
  int total_args = 1;
  string prompt;

  prompt = get_prompt();

  cout << prompt;
  getline(cin, input); //store string in input
  cout << "\n";
  
  string input_args[100];
  const char *args[100];
    
  istringstream iss(input);

  while(iss >> word) {
    input_args[total_args] = word;
    total_args++;
  } //while

  for(int i = 0; i < total_args; i++){
    args[i] = input_args[i].c_str();
  } //for
  
  eval ev(total_args, args);
  return ev;

} //get_input_info

/* function updates the user
 * prompt to the current working
 * directory each time it is called
 */
string get_prompt(){

  const char *path;
  string the_prompt = "1730sh: ";

  path = get_current_dir_name();
  the_prompt += path;
  the_prompt += "$ ";

  return the_prompt;
} //get_prompt
