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
	//***��ֵ***
	//��Ա������ֵ
	void assign(const char* str);
	void assign(const MString& other);
	//=��������ֵ
	MString& operator=(const char* str);
	MString& operator=(const int nNum);
	MString& operator=(const MString& other);
	MString& operator=(MString&& other);


	//***�ַ�������***
	size_t length() const;

	//***�ַ���***
	char* getStr() const;

	//***�����ַ�������***
	void setLength();

	//***�ַ�������***
	//��Ա������ֵ
	MString& append(const char* str);
	MString& append(const int num);
	MString& append(const char ch);
	MString& append(const MString& other);
	//+��������ֵ
	MString operator+(const char* str);
	MString operator+(const char ch);
	MString operator+(const MString& other);

	//***�ַ����Ƚ�***
	//��Ա�����Ƚ�
	int compare(const char* str);
	int compare(const MString& other);
	// <  ������
	bool operator<(const char* str);
	bool operator<(const MString& other);
	// <= ������
	bool operator<=(const char* str);
	bool operator<=(const MString& other);
	// >  ������
	bool operator>(const char* str);
	bool operator>(const MString& other);
	// >= ������
	bool operator>=(const char* str);
	bool operator>=(const MString& other);
	// == ������
	bool operator==(const char* str);
	bool operator==(const MString& other);
	// != ������
	bool operator!=(const char* str);
	bool operator!=(const MString& other);

	//***�Ӵ�***
	MString substr(int beg = 0, int num = NPOS);

	//***����***
	void swap(MString& other);

	//***����***
	//�����Ӵ����ַ�����λ�ã������ң�
	char* find(const char* str);
	char* find(const char ch);
	char* find(const MString& other);
	//�����Ӵ����ַ�����λ�ã����ҵ���
	char* rfind(const char* str);
	char* rfind(const char ch);
	char* rfind(const MString& other);

	//***�����ַ���***
	void insert(size_t beg, const char* str);
	void insert(size_t beg, const MString& other);
	void insert(size_t beg, const char ch);

	//***[]������***
	char& operator[](int index);

	//***�����***
	// << ������
	friend std::ostream& operator<<(std::ostream& os, const MString& other);
	// >> ������
	friend std::istream& operator>>(std::istream& is, MString& other);

private:
	//***�ͷſռ�***
	void release();
private:
	size_t mLen;
	char* mStr;
};



