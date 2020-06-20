#include "MString.h"

MString::MString(){
	this->mLen = 0;
	this->mStr = nullptr;
}

MString::MString(size_t size){
	this->mLen = size;
	this->mStr = new char[size + 1];
	memset(this->mStr, 0, sizeof(char) * (size + 1));
}

MString::MString(const char* str){
	this->mLen = strlen(str);
	this->mStr = new char[this->mLen + 1];
	strncpy(this->mStr, str, this->mLen + 1);
} 

MString::MString(const MString& other){
	this->mLen = strlen(other.mStr);
	this->mStr = new char[this->mLen + 1];
	strncpy(this->mStr, other.mStr, this->mLen + 1);
}

MString::MString(MString&& other){
	this->mLen = strlen(other.mStr);
	this->mStr = other.mStr;
	other.mStr = nullptr;
}

MString::~MString(){
	this->release();
}

/*************************
* ��ֵ����
**************************/
void MString::assign(const char* str){
	this->release(); //�ͷ�Դ�ռ�
	this->mLen = strlen(str);
	this->mStr = new char[this->mLen + 1];
	strncpy(this->mStr, str, this->mLen + 1);
}

void MString::assign(const MString& other){
	this->assign(other.mStr);
}

MString& MString::operator=(const char* str) {
	this->assign(str);
	return *this;
}

MString& MString::operator=(const int nNum){
	char sztmp[11] = { 0 };
	_itoa(nNum, sztmp, 10);
	this->assign(sztmp);
	return *this;
}

MString& MString::operator=(const MString& other) {
	this->assign(other.mStr);
	return *this;
}

MString& MString::operator=(MString&& other){
	this->swap(other);
	return *this;
}


/*************************
* ��ȡ�ַ�������
**************************/
size_t MString::length() const{
	return this->mLen;
}

/*************************
* ��ȡ�ַ���
**************************/
char* MString::getStr() const{
	return this->mStr;
}

/*************************
* �����ַ�������
**************************/
void MString::setLength(){
	if (this->mStr == nullptr) {
		this->mLen = 0;
		return;
	}
	this->mLen = strlen(this->mStr);
}

/*************************
* �ַ�������
**************************/
MString& MString::append(const char* str){
	int tmpLen = strlen(str);	//����ַ�������
	int allLen = tmpLen + this->mLen;//Դ�ַ�����Ŀ���ַ����ܳ���
	char* tmp = new char[allLen + 1]();
	if (this->mStr != nullptr) {
		strncpy(tmp, this->mStr, this->mLen + 1);
	}
	strncat(tmp, str, tmpLen + 1);

	this->release(); //�ͷ�ԭ�пռ�

	this->mLen = allLen;
	this->mStr = tmp;
	return *this;
}

MString& MString::append(const int num){
	char sztmp[11] = { 0 };
	_itoa(num, sztmp, 10);
	return this->append(sztmp);
}

MString& MString::append(const char ch){
	char sz[2] = { 0 };
	sz[0] = ch;
	return this->append(sz);
}

MString& MString::append(const MString& other){
	return this->append(other.mStr);
}

MString MString::operator+(const char* str){
	MString tmp(this->mStr);
	tmp.append(str);
	return std::move(tmp);
}

MString MString::operator+(const char ch){
	char sz[2] = { 0 };
	sz[0] = ch;
	return(*this + sz);
}

MString MString::operator+(const MString& other){
	return (*this + other.mStr);
}

/*************************
* �ַ����Ƚ�
**************************/
int MString::compare(const char* str){
	return strcmp(this->mStr, str);
}

int MString::compare(const MString& other){
	return this->compare(other.mStr);
}

bool MString::operator<(const char* str){
	return this->compare(str) < 0 ? true : false;
}

bool MString::operator<(const MString& other){
	return this->compare(other) < 0 ? true : false;;
}

bool MString::operator<=(const char* str){
	return this->compare(str) <= 0 ? true : false;
}

bool MString::operator<=(const MString& other){
	return this->compare(other) <= 0 ? true : false;;
}

bool MString::operator>(const char* str){
	return this->compare(str) > 0 ? true : false;
}

bool MString::operator>(const MString& other){
	return this->compare(other) > 0 ? true : false;;
}

bool MString::operator>=(const char* str){
	return this->compare(str) >= 0 ? true : false;
}

bool MString::operator>=(const MString& other){
	return this->compare(other) >= 0 ? true : false;
}

bool MString::operator==(const char* str){
	return this->compare(str) == 0 ? true : false;
}

bool MString::operator==(const MString& other){
	return this->compare(other) == 0 ? true : false;
}

bool MString::operator!=(const char* str){
	return this->compare(str) != 0 ? true : false;
}

bool MString::operator!=(const MString& other){
	return this->compare(other) != 0 ? true : false;
}

//MString MString::substr(int beg, int num){
//
//}

/*************************
* ����
**************************/
void MString::swap(MString& other){
	if (this == &other) {
		return;
	}
	char* tmpStr = std::move(other.mStr);
	int tmpInt = std::move(other.mLen);

	other.mStr = std::move(this->mStr);
	other.mLen = std::move(this->mLen);

	this->mStr = std::move(tmpStr);
	this->mLen = std::move(tmpInt);
}

/*************************
* ����
**************************/
char* MString::find(const char* str){
	return strstr(this->mStr,str);
}

char* MString::find(const char ch){
	return strchr(this->mStr,ch);
}

char* MString::find(const MString& other){
	return this->find(other.mStr);
}

char* MString::rfind(const char* str){
	if (this->mLen < strlen(str)) {
		return nullptr;
	}
	
	int up = this->mLen - strlen(str);
	int down = up;
	int subIndex = 0;
	while (up >= 0) {
		while (str[subIndex] != '\0') {
			if (this->mStr[up] == str[subIndex]) {
				subIndex++;
				up++;
			}
			else {
				up = down;
				subIndex = 0;
				break;
			}
		}
		if (str[subIndex] == '\0') {
			return &this->mStr[down];
		}
		up--;
		down--;
	}
	return nullptr;
}

char* MString::rfind(const char ch){
	return strrchr(this->mStr,ch);
}

char* MString::rfind(const MString& other){
	return this->rfind(other.mStr);
}


/*************************
* �����ַ���
**************************/
void MString::insert(size_t beg, const char* str){
	if (beg > this->mLen || beg < 0) {
		return;
	}
	int len = strlen(str);
	int allLen = len + this->mLen;
	char* tmp = new char[allLen + len + 1]();
	for (size_t i = 0; i < beg; i++) {
		tmp[i] = this->mStr[i];
	}
	strcat(tmp, str);
	strcat(tmp, &this->mStr[beg]);
	this->release();
	this->mLen = allLen;
	this->mStr = tmp;
}

void MString::insert(size_t beg, const MString& other){
	this->insert(beg, other.mStr);
}

void MString::insert(size_t beg, const char ch){
	char sz[2] = { 0 };
	sz[0] = ch;
	this->insert(beg, sz);
}


/*************************
* []������
**************************/
char& MString::operator[] (int index){
	return this->mStr[index];
}

/*----------------˽�г�Ա����--------------*/
/*************************
* �ͷſռ�
**************************/
void MString::release(){
	if (this->mStr != nullptr) {
		delete[] this->mStr;
		this->mStr = nullptr;
		this->mLen = 0;
	}
}

/*----------------ȫ�ַ���--------------*/
/*************************
* ���������ַ���
**************************/
std::ostream& operator<<(std::ostream& os, const MString& other){
	os << other.mStr;
	return os;
}

/*************************
* �����������ַ���
**************************/
std::istream& operator>>(std::istream& is, MString& other){
	char sz[4096] = { 0 };
	is.getline(sz, 4096 - 1);
	other.assign(sz);
	return is;
}

