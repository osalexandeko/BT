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
	const string str(buf);
	const char delimiter =' ';
	tokenize(str, delimiter, commands);
	
/*******************************************************************************
*  Example: 
*  "PWM_SET 19 1000 50"
*  Means PWM from GPIO 19 at frequency of 1000 Hz with duty cycle 50%.
*  The delimeter is space,
*  0. The type is PWM
*  1. The GPIO, a number (19) for now.
*  2. The frequency in kilo Herz.
*  3. The duty sycle in percents.
*******************************************************************************/
	if(commands[0].compare("PWM_SET") == 0){
		cout << "logic_parser: PWM_SET."<< endl;
		uint32_t user_gpio  = stoi(commands[1]);
		if(19 != user_gpio){
			cout <<"GPIO must be 19."<<endl;
			return;
		}
		uint32_t freq       = stoi(commands[2]);
		if(30000 < freq){
			cout <<"Frequency cannot be greater than 30000[Hz]."<<endl;
			return;
		}
		
		uint32_t duty_cycle = stoi(commands[3]);
		if(100 < duty_cycle){
			cout <<"The duty sycle cannot be greater then 100%."<<endl;
			return;
		}
		pwm_set(user_gpio, freq, duty_cycle*10000);
		
	}
/*******************************************************************************
*  PWM Update Duty Cycle.
*  Example: 
*  "PWM_DS 50"
*  Means PWM duty sycle 50%.
*  The delimeter is space,
*  0. The type is PWM_DS
*  1. The GPIO, a number (19) for now.
*  2. The duty sycle in percents.
*******************************************************************************/
	if(commands[0].compare("PWM_DS") == 0){
		cout << "logic_parser: PWM_DS."<< endl;
		
		uint32_t user_gpio  = stoi(commands[1]);
		if(19 != user_gpio){
			cout <<"GPIO must be 19."<<endl;
			return;
		}
		
		uint32_t duty_cycle = stoi(commands[2]);
		if(255 < duty_cycle){
			cout <<"The duty sycle cannot be greater then 100%."<<endl;
			return;
		}
		gpioPWM(user_gpio, duty_cycle);
		
	}
	else{
		cout <<"logic_parser: Command is not recognized."<<endl;
	}

	
}
	
	