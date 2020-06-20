#pragma once
#include <fstream>
class Mdata {
public:
	Mdata() {
		mOffset = 0;
		mSize = 0;
		mId = 0;
	}
public:
	long long mOffset;
	short mSize;
	int mId;
	bool operator>(const Mdata& other) const;
	bool operator<(const Mdata& other) const;
	bool operator==(const Mdata& other) const;
	bool operator!=(const Mdata& other) const;
	bool operator>=(const Mdata& other) const;
	bool operator<=(const Mdata& other) const;
};
