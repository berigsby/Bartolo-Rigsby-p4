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

void close_pipe(int pipefd[2]);
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
    /*
    char ** args = new char * [ev -> get_argc()];
    for(int i = 0; i < ev -> get_argc(); i++){
      args[i] = strdup(arg_v[i].c_str());
    } //for
    args[ev->get_argc()] = nullptr;
    */
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

    int pipefd1[2];
    //int pipefd2[2];
    if(pipe(pipefd1) == -1){
      //error
    }//if
    //if(pipe(pipefd2) == -1){
      //error
    //}//if

    for(int numProc = 0; numProc <= ev -> get_pipes(); numProc++){
      if((pid = fork()) < 0) { // error 
	perror("FORK ERROR");
      } //if
      else if (pid == 0) { //child
	if(ev -> get_pipes() == 0){
	  //is one process
	} else if(numProc < ev -> get_pipes()) {
	  if(dup2(pipefd1[1], STDOUT_FILENO) == -1){
	    //error
	  }//if
	}//else
	if(numProc != 0){
	  if(dup2(pipefd1[0], STDIN_FILENO) == -1){
	    //error
	  }//if
	}//if
	close_pipe(pipefd1);
	//close_pipe(pipefd2);
	string *arg_v1 = ev -> get_process(numProc);
	char ** args = new char * [ev -> get_process_args(numProc)];
	for(int i = 0; i < ev -> get_process_args(numProc); i++){
	  args[i] = strdup(arg_v1[i].c_str());
	} //for
	args[ev->get_process_args(numProc)] = nullptr;
	execvp(args[0], args);
      } //else if (child)
      else{ //parent
	if(numProc != ev -> get_pipes()){
	  // waitpid(pid,nullptr,0);
	  continue;
	}//if
	close_pipe(pipefd1);
	//close_pipe(pipefd2);
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
    }//for numProc
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

void close_pipe(int pipefd [2]) {
  if (close(pipefd[0]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  } // if
  if (close(pipefd[1]) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  } // if
} // close_pipe
