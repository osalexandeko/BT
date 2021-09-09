#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <iostream>
#include <vector>
#include <string>


using namespace std;

void tokenize(string const & str, const char delim, vector<string> & out);
//void logic_parser(string &); 
void * logic_task (void * param);
#endif