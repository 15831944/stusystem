#include "Mdata.h"

bool Mdata::operator>(const Mdata& other) const{
	return this->mId > other.mId;
}

bool Mdata::operator<(const Mdata& other) const{
	return this->mId < other.mId;
}

bool Mdata::operator==(const Mdata& other) const{
	return this->mId == other.mId;
}

bool Mdata::operator!=(const Mdata& other) const{
	return this->mId != other.mId;
}

bool Mdata::operator>=(const Mdata& other) const{
	return this->mId >= other.mId;
}

bool Mdata::operator<=(const Mdata& other) const{
	return this->mId <= other.mId;;
}
