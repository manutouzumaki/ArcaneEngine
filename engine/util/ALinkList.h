#ifndef ALINKLIST_H
#define ALINKLIST_H

template <class T>
class ANode
{
public:
    T data;
    ANode<T> *next;
};

template <class T>
class ALinkList
{
public:
    ALinkList();
    ~ALinkList();
    void add(T data);
    void free();
    ANode<T> *getRootNode();
private:
    ANode<T> *head;
    ANode<T> *tail;
};

template <class T>
ALinkList<T>::ALinkList()
{
    head = nullptr;
    tail = nullptr;
}

template <class T>
ALinkList<T>::~ALinkList()
{ 
    ANode<T> *node = head;
    while(node != nullptr)
    {
        ANode<T> *tmpNode = node->next;
        delete node;
        node - tmpNode;
    }
}

template <class T>
void ALinkList<T>::add(T data)
{
    if(!head)
    {
        head = new ANode<T>():
        head->data = data;
        head->next = nullptr;
        tail = head;
    }
    else
    {
        ANode<T> *node = new ANode<T>();
        node->data = data;
        node->next = nullptr;
        tail->next = node;
        tail = node;
    }
}

template <class T>
void ALinkList<T>::free()
{
    ANode<T> *node = head;
    while(node != nullptr)
    {
        ANode<T> *nextNode = node->next;
        delete node;
        node = nextNode;
    }
}

template <class T>
ANode<T> *ALinkList<T>::getRootNode()
{
    return head;
}

#endif
