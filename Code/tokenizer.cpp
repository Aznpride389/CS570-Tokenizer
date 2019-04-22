#include <iostream>
#include <ctype.h>
#include <vector>
#include <list>
#include <string>
#include <readline/readline.h>

using namespace std;

list<string> parse(char*);
int isSpecial(char);
int isWhiteSpace(char);
int isAlphaNum(char);
int isPunct(char);
char escapeCharacter(char);

int main(){
	static char* line_read = (char*)NULL;

	while(1){	
		line_read = readline ("> ");
		if (line_read != 0){			
			list<string> tokens = parse(line_read);
			string seperator = "";
			
			// handles blank line
			if (tokens.size() == 0){
				continue;
			}
			cout<<'[';
			while( !tokens.empty()){
				string token = tokens.front();
				tokens.pop_front();
				cout<<seperator<<'{'<<token<<'}';
				seperator = ",";
			}
			cout<<']'<<endl;
		}
		else break;
	}
	
}

list<string> parse(char* buffer){
	list<string> tokens;
	int state = 0;
	string token = "";
	int index = 0;

	// state0: nothing special, state1: hit a double quote, state2: hit a single quote, state3: handles escape characters within single quote

	while(1){
		char c = buffer[index];
		if (state == 0){
			if (c == '\0'){
				if (token.size() != 0){
					tokens.push_back(token);
				}
				break;
			}
			else if ( c == '\\'){
				index++;

				//terminate if next char is end
				if ( buffer[index] == '\0'){
					continue;
				}
				token+=buffer[index];
			}
			else if (isAlphaNum(c)){
				token+=c;
			}
			else if (isPunct(c)){
				token+=c;
			}
			else if (c == '\"'){
				state = 1;
			}
			else if (c == '\''){
				state = 2;
			}
			else if (isWhiteSpace(c)){
				if(token.size() != 0) {
					tokens.push_back(token);
					token = "";
				}
			}
			else if (isSpecial(c)){   
				if(token.size() != 0) {
					tokens.push_back(token);
					token = "";
				}
				token+=c;
				tokens.push_back(token);
				token = "";
			}
			else { 
				cout << "Unrecognized: " << c << endl; //in the case of characters not caught in the "buckets" below

			}
		}

		// handles double quotes
		else if (state == 1 ){
			if (c == '\\')
			{
				index++;
				token+=escapeCharacter(buffer[index]);
			}
			else if ( c == '\"' ){
				state = 0;
				tokens.push_back(token);
				token = "";
			}
			else if ( c == '\0') {
				state = 0;
				if(token.size() > 0) {
					tokens.push_back(token);
					token = "";
				}
				break;
			}
			else{
				token+=c;
			}
		}
	
		//handles single quotes
		else if (state == 2 || state == 3){
			if ( c == '\''){
				char next  = buffer[index+1];
				
				if (isWhiteSpace(next)){
					state = 0;
					if(token.size() > 0) {
						tokens.push_back(token);
						token = "";
					}
				}
				else state = 3;
			}
			
			//handles escapecharacters in single quotes
			else if ( c == '\\'){	
				index++;
				if ( buffer[index] == '\''){
					token+= '\\';
					state = 3;
				}
				else{ 
					token+=escapeCharacter(buffer[index]);
				}		
			}
			else if ( state == 3 && isWhiteSpace(c)){
				tokens.push_back(token);
				token = "";
				state = 0 ;
			}
			else if ( c == '\0'){
				state = 0;
				if(token.size() > 0) {
					tokens.push_back(token);
					token = "";
				}
				break;
			}
			else{
				token+=c;
			}
		}
		index++;
	}
	return tokens;
}

int isWhiteSpace(char c){
	return isspace(c);
}

int isSpecial (char c){
	if( c == '|' || c == ';' || c == '<' || c == '>' || c == '&'){
		return 1;
	}
	return 0;
}

int isAlphaNum (char c){
	return isalnum(c);
}

int isPunct ( char c){
	if( c == '+' || c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' || c == '*' || c == '('
	 || c == ')' || c == '=' || c == '.' || c == ':' || c == '-' || c == '/' || c == '_' || c == '[' || c == ']'
	 || c == '{' || c == '}' || c == ',' || c == '?'){
		return 1;
	}
	return 0;
}	

char escapeCharacter (char c)
{
	return c;
}
