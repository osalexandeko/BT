#include "logic.hpp"
#include "pwm.hpp" 
 
/*******************************************************************************
* Tocenizesa string by delimeter.
*@param str - the string.
*@param delim - the delimeter.
*@param out - output, vecror which contains the delimitted string.
*******************************************************************************/
void tokenize(string const & str, const char delim, vector<string> & out){
	size_t start = 0;
	size_t end = 0;
 	string substr;
	 
	while(end != string::npos){
		end = str.find(delim,start);
 		substr = str.substr(start,end - start);
	   	out.push_back(str.substr(start,end - start));
		start = end+1;
		 
	}
	/*for(vector<string>::iterator it = out.begin();it != out.end();it++){
		cout <<".."<<*it << endl;
	} */
	 
}

/*******************************************************************************
* Parses the charecter bufer.
*@param buf - the buffer to parse.
*******************************************************************************/
void logic_parser(char * buf ){
	vector<string>  commands;
	const string s(buf);
	const char d =' ';
	tokenize(s, d, commands);
	if(commands[0].compare("PWM") == 0){
		cout << "logic_parser: PWM."<< endl;
		//19 1000 500000
		uint32_t user_gpio  = stoi(commands[1]);
		uint32_t freq       = stoi(commands[2]);
		uint32_t duty_cycle = stoi(commands[3]);
		pwm_set(user_gpio, freq, duty_cycle);
		
	}
	
	else{
		cout <<"logic_parser: Command is not recognized."<<endl;
	}

	
}
	
	