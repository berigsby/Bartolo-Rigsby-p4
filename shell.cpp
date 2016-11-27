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
#include "jobs.h"
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

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

    if(ev -> get_procs() == 0){ //check for other commands other than processes
      if(arg_v[0].compare("exit") == 0){ //exit
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
	if(numProc == 0 && (ev -> get_std_in()).compare("STDIN_FILENO") != 0){
	  int filein = -1;
	  filein = open((ev->get_std_in()).c_str(), O_RDONLY);
	  if(dup2(filein, STDIN_FILENO) == -1){
	    //error
	  }//if
	  close(filein);
	}//if
	if(numProc == ev -> get_pipes()){
	  if((ev -> get_std_out()).compare("STDOUT_FILENO") != 0){
	    int fileout = -1;
	    if(ev-> get_out_trunc()) fileout = open((ev->get_std_out()).c_str(), O_WRONLY | O_TRUNC);
	    else fileout = open((ev->get_std_out()).c_str(), O_WRONLY | O_APPEND);
	    if(dup2(fileout, STDOUT_FILENO) == -1){
	      //error
	    }//if
	    close(fileout);
	  }//if did change
	}//if last proc
	if((ev -> get_std_err()).compare("STDERR_FILENO") != 0){
	  int fileout = -1;
	  if(ev-> get_err_trunc()) fileout = open((ev->get_std_err()).c_str(), O_WRONLY | O_TRUNC);
	  else fileout = open((ev->get_std_err()).c_str(), O_WRONLY | O_APPEND);
	  if(dup2(fileout, STDOUT_FILENO) == -1){
	    //error
	  }//if
	  close(fileout);
	}//if did change
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
  string the_path(path);

  if(the_path.find(getenv("HOME")) != string::npos){ //if the path contains the HOME dir
    the_path.erase(0, strlen(getenv("HOME"))); //erase HOME dir from path
    the_prompt += "~";
  } //if

  the_prompt += the_path;
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
