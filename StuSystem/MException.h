#pragma once
#include <exception>
#include <iostream>
class MException :
public std::exception
{};

class NotFind 
:public MException{
public:
	NotFind();
	NotFind(const char* str);
	~NotFind();
public:
	virtual char const* what() const;
private:
	char* error;
	static const char* defErr;
};



