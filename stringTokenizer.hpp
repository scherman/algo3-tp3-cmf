#ifndef TEST_DEBUGGER_TOKENIZER
#define TEST_DEBUGGER_TOKENIZER

#include <string>

#include <vector>
using namespace std;



class stringTokenizer{

private:
	vector<string> tokens;

public:
	stringTokenizer(){

	}

	stringTokenizer(const string& str, string::value_type delimiter){
		tokenize(str, delimiter);
	}

	void tokenize(const string& str, string::value_type delimiter){

		tokens.clear();

		string::size_type initPos = string::size_type(0);
		string::size_type pos = str.find(delimiter);
		do{
			tokens.push_back(str.substr(initPos, pos - initPos));
			initPos = ++pos;
			pos = str.find(delimiter, initPos);
		}while(pos != string::npos);

		if(initPos < str.length())
			tokens.push_back(str.substr(initPos));
	}


	const string& operator[](unsigned n) const{
		return tokens[n];
	}

	const unsigned tokenCount() const{
		return tokens.size();
	}
};


#endif // TEST_DEBUGGER_TOKENIZER
