#pragma once
#include <fstream>
#include "Mdata.h"
#include "MString.h"

using namespace std;
class MFile{
public:
	MFile(const char* file, int mode);
	~MFile();
public:
	fstream& getFileP() const;
	Mdata& write(MString& str);
	void write(char ch, long long offset);
	MString read(Mdata& mdata);
private:
	fstream* file;
};

