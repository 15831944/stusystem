#pragma once
template <typename T>
class Stack     //栈类模板
{
public:
    Stack(int capacity=5):capacity_(capacity),size_(0),ptr_elem(new T[capacity_]){}//构造
    ~Stack();   //析构
    bool empty();   //判空
    void push(const T&);    //压栈
    T& pop();   //弹出
    int size();
private:
    int size_;   //当前大小
    int capacity_;  //当前容量
    T* ptr_elem;    //元素数组
};

template <typename T>
bool Stack<T>::empty()  //判空
{
    return size_ == 0;
}

template <typename T>
Stack<T>::~Stack()      //析构
{
    if (ptr_elem != nullptr)
    {
        delete[] ptr_elem;
        ptr_elem = nullptr;
    }
}

template <typename T>
void Stack<T>::push(const T& elem)  //压栈
{
    if (size_ == capacity_)
    {
        capacity_ *= 2;
        T* temp = new T[capacity_];
        for (int i = 0; i < size_; ++i)
        {
            temp[i] = ptr_elem[i];
        }
        delete[] ptr_elem;
        ptr_elem = temp;
    }
    ptr_elem[size_++] = elem;
}

template <typename T>
T& Stack<T>::pop()      //弹出
{
    if (empty())
    {
        throw "empty";
    }
    return ptr_elem[--size_];
}

template <typename T>
int Stack<T>::size()
{
    return size_;
}