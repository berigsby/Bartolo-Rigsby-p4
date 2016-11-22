#include <ios>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::cin;

void nope_out(const string & prefix);

int main(const int argc, const char * argv []) {

  // turn off buffering for cout
  cout.setf(std::ios::unitbuf);

  while(true){

    struct myProcesses{
      char array[8][120];
      void reset(){
	for(int x = 0; x < 8; x++){
	  array[x][0] = '\0';
	}//for
      }//reset
    };

    myProcesses myProc[12];

    cout << "repl$ ";
    string wholething;
    getline(cin, wholething);
    int num = 0;
    
    if(wholething == "") continue;

    bool hasBar = true;
    bool changeinappend = false;
    bool changeintrunc = false;
    string changeins = "";
    unsigned int changeini = 0;
    bool changeoutappend = false;
    bool changeouttrunc = false;
    string changeouts = "";
    unsigned int changeouti = 0;
    bool changeeappend = false;
    bool changeetrunc = false;
    string changees = "";
    unsigned int changeei = 0;
    unsigned int changeloc = wholething.length();
    //int changetemp = changeloc;
    
    if(wholething.find(" << ") != string::npos){
      changeini = wholething.find(" << ");
      changeinappend = true;
      if(changeini < changeloc) changeloc = changeini;
      //changeini += 4;
    }//if
    if(wholething.find(" < ") != string::npos){
      changeini = wholething.find(" < ");
      changeintrunc = true;
      if(changeini < changeloc) changeloc = changeini;
      //changeini += 3;
    }//if

    if(wholething.find(" >> ") != string::npos){
      changeouti = wholething.find(" >> ");
      changeoutappend = true;
      if(changeouti < changeloc) changeloc = changeouti;
      //changeouti +=4;
    }//if
    if(wholething.find(" > ") != string::npos){
      changeouti = wholething.find(" > ");
      changeouttrunc = true;
      if(changeouti < changeloc) changeloc = changeouti;
      //changeouti +=3;
    }//if

    if(wholething.find(" e>> ") != string::npos){
      changeei = wholething.find(" e>> ");
      changeeappend = true;
      if(changeei < changeloc) changeloc = changeei;
      //changeei += 5;
    }//if
    if(wholething.find(" e> ") != string::npos){
      changeei = wholething.find(" e> ");
      changeetrunc = true;
      if(changeei < changeloc) changeloc = changeei;
      //changeeei += 4;
    }//if

    if(changeinappend || changeintrunc){
      if(changeoutappend || changeouttrunc){
	changeins = wholething.substr(changeini + 3,changeouti-changeini-3);
      } else if(changeeappend || changeetrunc){
	changeins = wholething.substr(changeini + 3,changeei-changeini-4);
      } else {
	changeins = wholething.substr(changeini + 3);
      }//else
    }//if
    if(changeoutappend || changeouttrunc){
      if(changeeappend || changeetrunc) changeouts = wholething.substr(changeouti+3,changeei-changeouti-4);
      else changeouts = wholething.substr(changeouti + 3);
    }//if
    if(changeeappend || changeetrunc){
      changees = wholething.substr(changeei + 4);
    }//if
    
    wholething = wholething.substr(0,changeloc);
    
    while(hasBar){
      int nxtbar = wholething.find_first_of("|");
      if(nxtbar == -1){
	hasBar = false;
	nxtbar = wholething.length();
      }//if
      string thingToParse = wholething.substr(0,nxtbar);
      char * charToParse = (&thingToParse[0]);
      if(hasBar) wholething = wholething.substr(nxtbar+1);
      int location = 0;
      //cout << charToParse << endl;
      for(unsigned int x = 0; x < thingToParse.length(); x++){
	if(charToParse[x] == ' '){
	  continue;
	}//if
	//if(charToParse[x] == '\0') break;
	for(unsigned int y = 0; y < thingToParse.length(); y++,x++){
	  if(charToParse[x] == ' '){
	    //x++;
	    break;
	  }//if
	  myProc[num].array[location][y] = charToParse[x];
	  myProc[num].array[location][y+1] = '\0';
	}//for
	location++;
      }//for
      num++;
    }//while
    num--;
    
    cout << endl;
    cout << "Job STDIN  = ";
    if(changeinappend || changeintrunc) cout << changeins <<endl;
    else cout << "STDIN_FILENO" <<endl;
    cout << "Job STDOUT = ";
    if(changeoutappend || changeouttrunc) cout << changeouts <<endl;
    else cout << "STDOUT_FILENO" <<endl;
    cout << "Job STDERR = ";
    if(changeeappend || changeetrunc) cout << changees <<endl;
    else cout << "STDERR_FILENO" <<endl;

    cout << endl;
    cout << num << " pipe(s)" << endl;
    cout << num+1 << " process(es)" << endl;
    cout << endl;

    for(int x = 0; x <= num; x++){
      cout << "Process " << x << " argv:" << endl;
      for(int y = 0;true; y++){
	if(myProc[x].array[y][0] == '\0') break;
	cout << y << ": ";
	cout << myProc[x].array[y] << endl;
      }//for
      cout << endl;
    }//for
    for(int x = 0; x < 12; x++){
      myProc[x].reset();
    }//for
  }//while

}//main

void nope_out(const string & prefix) {
  perror(prefix.c_str());
  exit(EXIT_FAILURE);
} // nope_out
