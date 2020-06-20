#pragma once
template <typename T>
class Stack     //ջ��ģ��
{
public:
    Stack(int capacity=5):capacity_(capacity),size_(0),ptr_elem(new T[capacity_]){}//����
    ~Stack();   //����
    bool empty();   //�п�
    void push(const T&);    //ѹջ
    T& pop();   //����
    int size();
private:
    int size_;   //��ǰ��С
    int capacity_;  //��ǰ����
    T* ptr_elem;    //Ԫ������
};

template <typename T>
bool Stack<T>::empty()  //�п�
{
    return size_ == 0;
}

template <typename T>
Stack<T>::~Stack()      //����
{
    if (ptr_elem != nullptr)
    {
        delete[] ptr_elem;
        ptr_elem = nullptr;
    }
}

template <typename T>
void Stack<T>::push(const T& elem)  //ѹջ
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
T& Stack<T>::pop()      //����
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