#include "MException.h"
/****NotFindÒì³£****/

const char* NotFind::defErr = "error: not find!";

NotFind::NotFind(){
	this->error = nullptr;
}

NotFind::NotFind(const char* str){
	int len = strlen(str);
	this->error = new char[len + 1];
	strncpy(this->error, str, len + 1);
}

NotFind::~NotFind(){
	if (this->error != nullptr) {
		delete[] this->error;
		this->error = nullptr;
	}
}

char const* NotFind::what() const{
	if (this->error == nullptr) {
		return this->defErr;
	}
	return this->error;
}

