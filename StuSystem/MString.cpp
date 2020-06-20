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
* 赋值操作
**************************/
void MString::assign(const char* str){
	this->release(); //释放源空间
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
* 获取字符串长度
**************************/
size_t MString::length() const{
	return this->mLen;
}

/*************************
* 获取字符串
**************************/
char* MString::getStr() const{
	return this->mStr;
}

/*************************
* 设置字符串长度
**************************/
void MString::setLength(){
	if (this->mStr == nullptr) {
		this->mLen = 0;
		return;
	}
	this->mLen = strlen(this->mStr);
}

/*************************
* 字符串连接
**************************/
MString& MString::append(const char* str){
	int tmpLen = strlen(str);	//添加字符串长度
	int allLen = tmpLen + this->mLen;//源字符串和目标字符串总长度
	char* tmp = new char[allLen + 1]();
	if (this->mStr != nullptr) {
		strncpy(tmp, this->mStr, this->mLen + 1);
	}
	strncat(tmp, str, tmpLen + 1);

	this->release(); //释放原有空间

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
* 字符串比较
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
* 交换
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
* 查找
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
* 插入字符串
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
* []操作符
**************************/
char& MString::operator[] (int index){
	return this->mStr[index];
}

/*----------------私有成员方法--------------*/
/*************************
* 释放空间
**************************/
void MString::release(){
	if (this->mStr != nullptr) {
		delete[] this->mStr;
		this->mStr = nullptr;
		this->mLen = 0;
	}
}

/*----------------全局方法--------------*/
/*************************
* 输出流输出字符串
**************************/
std::ostream& operator<<(std::ostream& os, const MString& other){
	os << other.mStr;
	return os;
}

/*************************
* 输入流输入字符串
**************************/
std::istream& operator>>(std::istream& is, MString& other){
	char sz[4096] = { 0 };
	is.getline(sz, 4096 - 1);
	other.assign(sz);
	return is;
}

