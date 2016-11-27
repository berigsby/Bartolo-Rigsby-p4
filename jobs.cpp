#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "jobs.h"

/* constructor calls the function
 * which initializes all vars
 */
jobs::jobs(string in_command){
  total_jids ++;
  jid = total_jids;
  status = stati[1];
  command = in_command;
} //constructor

jobs::~jobs(){
  status = stati[0];
}//deconstructor

int jobs::get_jid(){
  return jid;
}//get_jid

int jobs::get_total_jids(){
  return total_jids;
}//get_total_jids

string jobs::get_status(){
  return status;
}//get_status

void jobs::stop_status(){
  status = stati[0];
}//stop_status
