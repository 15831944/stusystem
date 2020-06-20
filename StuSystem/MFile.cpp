#include "MFile.h"

MFile::MFile(const char* file, int mode){
	this->file = new fstream(file, mode);
}

MFile::~MFile(){
	if (this->file != nullptr) {
		this->file->close();
		delete this->file;
		this->file = nullptr;
	}
}

fstream& MFile::getFileP() const{
	return *this->file;
}

Mdata& MFile::write(MString& str){
	this->file->seekg(0, ios::end);
	char biaozhi = (char)0xFA;
	short len = (short)str.length();
	Mdata* tmp = new Mdata;
	tmp->mOffset = this->file->tellg();
	tmp->mSize = len;
	this->file->write((char*) &biaozhi, sizeof(char));
	this->file->write((char*) &len, sizeof(short));
	this->file->write(str.getStr(), str.length());
	return *tmp;
}

void MFile::write(char ch, long long offset){
	this->file->seekg(offset, ios::beg);
	this->file->write((char*)& ch, sizeof(char));
}

MString MFile::read(Mdata& mdata){
	MString str(mdata.mSize);
	this->file->seekg(mdata.mOffset + 3, ios::beg);
	this->file->read(str.getStr(), mdata.mSize);
	return str;
}

