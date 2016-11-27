#ifndef JOBS_H
#define JOBS_H
#include <string.h>
#include <string>

using namespace std;

class jobs{

 private:
  int jid;
  int total_jids;
  const string stati[2] = {"Stopped","Running"};
  string status;
  string command;
  
 public:
  jobs(string in_command);
  ~jobs();
  
  int get_jid();
  int get_total_jids();
  string get_status();
  void stop_status();
  void start_status();

};

#endif
