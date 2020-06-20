#pragma once
#include "MException.h"
#include "stack.h"
#include "MStack.hpp"
#include "Mdata.h"
#include <iostream>
#define MAX(x,y) ( (x) > (y) ? (x) : (y))

template<class T>
class MTree {
public:
	class TreeNode {
	public:
		TreeNode(const T& data, TreeNode* parent = nullptr, TreeNode* lChild = nullptr, TreeNode* rChild = nullptr)
			:data(data), height(1), next(nullptr), parent(parent), lChild(lChild), rChild(rChild) {}
		~TreeNode() {
		}

		T data;
		int height;
		TreeNode* next;
		TreeNode* parent;
		TreeNode* lChild;
		TreeNode* rChild;
	};
public:
	MTree();
	~MTree();
public:
	//***判断是否为空***                                                          
	bool empty(); 

	//***插入元素***
	void insert(const T& data);

	//***删除元素***
	void remove(const T& data);

	//***查找***
	TreeNode* find(const T& data);

	//***获取最大值***
	const T& Max();

	//***变量释放***
	void release();
private:
	//***获取树最小值***
	TreeNode* minData(TreeNode* root);

	//***平衡树***
	void balance(TreeNode* root);

	//***获取节点高***
	int height(TreeNode* root) const;

	//***旋转***
	//左单旋
	void rotateLL(TreeNode* root);
	//右单旋
	void rotateRR(TreeNode* root);
	//先左旋在右旋
	void rotateLR(TreeNode* root);
	//先右旋在左旋
	void rotateRL(TreeNode* root);

private:
	TreeNode* root;
};


template<class T>
inline MTree<T>::MTree()
:root(nullptr){
}

template<class T>
inline MTree<T>::~MTree(){
	this->release();
}

template<class T>
inline bool MTree<T>::empty(){
	return this->root == nullptr;
}

template<class T>
inline void MTree<T>::release() {
	TreeNode* root = this->root;
	if (root == nullptr) {
		return;
	}
	MStack<TreeNode*> st;
	MStack<TreeNode*> st2;
	TreeNode* cur = root;
	while (cur != nullptr || st.size() != 0) {
		while (cur != nullptr) {
			st.push(cur);
			st2.push(cur);
			cur = cur->rChild;
		}
		if (!st.empty()) {
			cur = st.top();
			cur = cur->lChild;
			st.pop();
		}

	}
	TreeNode* tmp =nullptr;
	TreeNode* next = nullptr;
	TreeNode* old = nullptr;
	while (!st2.empty()) {
		tmp = st2.top();
		next = tmp->next;
		while (next!=nullptr) {
			old = next;
			next = next->next;
			delete old;
		}
		delete tmp;
		st2.pop();
	}
	this->root = nullptr;
}

template<class T>
inline void MTree<T>::insert(const T& data){
	TreeNode* newNode = new TreeNode(data);
	//如果根节点是空，字节new节点
	if (this->root == nullptr) {
		this->root = newNode;
		return;
	}
	TreeNode* cur = this->root;//保存当前节点
	TreeNode* parent = nullptr;//保存当前节点的父节点
	while (cur != nullptr) {
		parent = cur;
		if (data > cur->data) {
			cur = cur->rChild;
			continue;
		}
		else if (data < cur->data) {
			cur = cur->lChild;
			continue;
		}
		else {
			newNode->next = cur->next;
			cur->next = newNode;
			return;
		}
	}

	if (data > parent->data) { //值大于当前节点值就把孩子挂在右边
		parent->rChild = newNode;
		parent->rChild->parent = parent;
		this->balance(parent);
		return;
	}
	else if (data < parent->data) {//值小于当前节点值就把孩子挂在左边
		parent->lChild = newNode;
		parent->lChild->parent = parent;
		this->balance(parent);
		return;
	}
}

template<class T>
inline void MTree<T>::remove(const T& data){
	TreeNode* cur = this->root;
	if (cur == nullptr) {
		return;
	}
	T d = data;
	while (cur != nullptr) {
		if (d > cur->data) {
			cur = cur->rChild;
		}
		else if (d < cur->data) {
			cur = cur->lChild;
		}
		else {
			//当前节点没有孩子直接删除
			if (cur->lChild == nullptr && cur->rChild == nullptr) {
				TreeNode* parent = cur->parent;
				if (parent != nullptr) {
					if (parent->lChild == cur) {
						parent->lChild = nullptr;
					}
					else {
						parent->rChild = nullptr;
					}
					this->balance(parent);
				}
				else {
					this->root = nullptr;
				}
				delete cur;
				return;
			}
			//当前节点有一个孩子，把当前节点的孩子给当前父节点
			else if(cur->lChild == nullptr || cur->rChild == nullptr) {
				//获取当前节点存在的一个孩子
				TreeNode* child = cur->lChild == nullptr ? cur->rChild : cur->lChild;
				TreeNode* parent = cur->parent;

				if (parent != nullptr) {
					if (parent->lChild == cur) {
						parent->lChild = child;
					}
					else {
						parent->rChild = child;
					}
				}
				else {
					this->root = child;
				}
				child->parent = parent;
				this->balance(parent);
				delete cur;
				return;
			}
			else {
				//获取右子树的最小值
				TreeNode* min = this->minData(cur->rChild);
				cur->data = min->data;//把当前树值替换最小树值值
				cur->next = min->next;
				cur = min;
				d = cur->data;
			}
			
		}
	}
}

template<class T>
inline typename MTree<T>::TreeNode* MTree<T>::find(const T& data){
	TreeNode* cur = this->root;
	while (cur != nullptr) {
		if (data > cur->data) {
			cur = cur->rChild;
		}
		else if (data < cur->data) {
			cur = cur->lChild;
		}else{
			return cur;
		}
	}
	throw NotFind();
}

template<class T>
inline const T& MTree<T>::Max(){
	TreeNode* cur = this->root;
	while (cur != nullptr) {
		if (cur->rChild == nullptr) {
			return cur->data;
		}
		cur = cur->rChild;
	}
	throw NotFind();
}

template<class T>
inline typename MTree<T>::TreeNode* MTree<T>::minData(TreeNode* root){
	TreeNode* cur = root;
	if (cur == nullptr) {
		return nullptr;
	}
	while (true) {
		if (cur->lChild == nullptr) {
			return cur;
		}
		cur = cur->lChild;
	}
}

template<class T>
inline void MTree<T>::balance(TreeNode* root){
	TreeNode* cur = root;
	while (cur != nullptr) {
		//设置当前节点高度
		cur->height = MAX(this->height(cur->lChild), this->height(cur->rChild)) + 1;

		//计算平衡因子
		int bf = this->height(cur->lChild) - this->height(cur->rChild);
		
		if (bf > 1) { //左边不平衡
			bf = this->height(cur->lChild->lChild) - this->height(cur->lChild->rChild);
			if (bf > 0) {
				this->rotateRR(cur);
			}
			else {
				rotateLR(cur);
			}
		}
		else if (bf < -1) { //右边不平衡
			bf = this->height(cur->rChild->lChild) - this->height(cur->rChild->rChild);
			if (bf < 0) {
				this->rotateLL(cur);
			}
			else {
				rotateRL(cur);
			}
		}
		//修改当前节点的父节点高度
		cur = cur->parent;
	}
}

template<class T>
inline int MTree<T>::height(TreeNode* root) const{
	return root == nullptr ? 0:root->height;
}

template<class T>
inline void MTree<T>::rotateLL(TreeNode* root){
	TreeNode* parent = root->parent;
	TreeNode* k1 = root;
	TreeNode* k2 = k1->rChild;
	TreeNode* a = k1->lChild;
	TreeNode* b = k2->lChild;
	TreeNode* c = k2->rChild;

	k1->rChild = b;
	k1->parent = k2;

	k2->lChild = k1;
	k2->parent = parent;

	if (b != nullptr) {
		b->parent = k1;
	}
	k1->height = MAX(this->height(a),this->height(b)) + 1;
	k2->height = MAX(this->height(k1),this->height(c)) + 1;

	if (parent != nullptr) {
		if (parent->lChild == k1) {
			parent->lChild = k2;
		}
		else {
			parent->rChild = k2;
		}
	}
	else {
		this->root = k2;
	}
}

template<class T>
inline void MTree<T>::rotateRR(TreeNode* root){
	TreeNode* parent = root->parent;
	TreeNode* k1 = root;
	TreeNode* k2 = k1->lChild;
	TreeNode* a = k2->lChild;
	TreeNode* b = k2->rChild;
	TreeNode* c = k1->rChild;

	k1->lChild = b;
	k1->parent = k2;

	k2->rChild = k1;
	k2->parent = parent;

	if (b != nullptr) {
		b->parent = k1;
	}
	k1->height = MAX(this->height(b), this->height(c)) + 1;
	k2->height = MAX(this->height(a), this->height(k1)) + 1;

	if (parent != nullptr) {
		if (parent->lChild == k1) {
			parent->lChild = k2;
		}
		else {
			parent->rChild = k2;
		}
	}
	else {
		this->root = k2;
	}
}

template<class T>
inline void MTree<T>::rotateLR(TreeNode* root){
	this->rotateLL(root->lChild);
	this->rotateRR(root);
}

template<class T>
inline void MTree<T>::rotateRL(TreeNode* root){
	this->rotateRR(root->rChild);
	this->rotateLL(root);
}
