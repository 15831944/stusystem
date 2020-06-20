#pragma once
#include <iostream>
#define NPOS -1
class MString
{
public:
	MString();
	MString(size_t size);
	MString(const char* str);
	MString(const MString& other);
	MString(MString&& other);
	~MString();
public:
	//***赋值***
	//成员函数赋值
	void assign(const char* str);
	void assign(const MString& other);
	//=操作符赋值
	MString& operator=(const char* str);
	MString& operator=(const int nNum);
	MString& operator=(const MString& other);
	MString& operator=(MString&& other);


	//***字符串长度***
	size_t length() const;

	//***字符串***
	char* getStr() const;

	//***设置字符串长度***
	void setLength();

	//***字符串连接***
	//成员函数赋值
	MString& append(const char* str);
	MString& append(const int num);
	MString& append(const char ch);
	MString& append(const MString& other);
	//+操作符赋值
	MString operator+(const char* str);
	MString operator+(const char ch);
	MString operator+(const MString& other);

	//***字符串比较***
	//成员函数比较
	int compare(const char* str);
	int compare(const MString& other);
	// <  操作符
	bool operator<(const char* str);
	bool operator<(const MString& other);
	// <= 操作符
	bool operator<=(const char* str);
	bool operator<=(const MString& other);
	// >  操作符
	bool operator>(const char* str);
	bool operator>(const MString& other);
	// >= 操作符
	bool operator>=(const char* str);
	bool operator>=(const MString& other);
	// == 操作符
	bool operator==(const char* str);
	bool operator==(const MString& other);
	// != 操作符
	bool operator!=(const char* str);
	bool operator!=(const MString& other);

	//***子串***
	MString substr(int beg = 0, int num = NPOS);

	//***交换***
	void swap(MString& other);

	//***查找***
	//查找子串或字符出现位置（从左到右）
	char* find(const char* str);
	char* find(const char ch);
	char* find(const MString& other);
	//查找子串或字符出现位置（从右到左）
	char* rfind(const char* str);
	char* rfind(const char ch);
	char* rfind(const MString& other);

	//***插入字符串***
	void insert(size_t beg, const char* str);
	void insert(size_t beg, const MString& other);
	void insert(size_t beg, const char ch);

	//***[]操作符***
	char& operator[](int index);

	//***流输出***
	// << 操作符
	friend std::ostream& operator<<(std::ostream& os, const MString& other);
	// >> 操作符
	friend std::istream& operator>>(std::istream& is, MString& other);

private:
	//***释放空间***
	void release();
private:
	size_t mLen;
	char* mStr;
};



