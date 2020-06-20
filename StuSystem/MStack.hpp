#pragma once
#include <iostream>
template<class T>
class MStack {
private:
	class MStackNode {
	public:
		MStackNode():next(nullptr) {}
		MStackNode(const T& data) :data(data),next(nullptr) {}
	public:
		T data;
		MStackNode* next;
	};
public:
	MStack();
	~MStack();
public:
	T& top();
	void push(const T& data);
	void pop();
	int size() const;
	bool empty() const;
	void swap(MStack<T>& other);
private:
	void release();
private:
	MStackNode* mHead;
	int mSize;
};

template<class T>
inline MStack<T>::MStack():mHead(nullptr),mSize(0){
}

template<class T>
inline MStack<T>::~MStack(){
	MStackNode* tmp = this->mHead;
	while (tmp) {
		MStackNode* next = tmp->next;
		delete tmp;
		tmp = next;
	}
}

template<class T>
inline T& MStack<T>::top(){
	/*if (this->empty()) {
		return T();
	}*/
	return this->mHead->data;
}

template<class T>
inline void MStack<T>::push(const T& data){
	MStackNode* tmp = new MStackNode(data);
	tmp->next = this->mHead;
	this->mHead = tmp;
	this->mSize++;
}

template<class T>
inline void MStack<T>::pop(){
	if (this->empty()) {
		return;
	}
	MStackNode* old = this->mHead;
	this->mHead = this->mHead->next;
	delete old;
	this->mSize--;
}

template<class T>
inline int MStack<T>::size() const{
	return this->mSize;
}

template<class T>
inline bool MStack<T>::empty() const{
	return this->mSize == 0;
}

template<class T>
inline void MStack<T>::swap(MStack<T>& other){
	MStackNode* tmp = std::move(other.mHead);
	int tmpSize = other.mSize;

	other.mHead = std::move(this->mHead);
	other.mSize = this->mSize;

	this->mHead = std::move(tmp);
	this->mSize = tmpSize;
}
