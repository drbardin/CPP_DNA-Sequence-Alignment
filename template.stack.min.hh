/* @author Dan Bardin */
#include <iostream> // provides objects like cin and cout for sending data
// to and from the standard streams input and output.
// These objects are part of the std namespace.
#include <cstdlib>  // has exit etc.
#include <fstream>  // file streams and operations
#include <sstream>  // string streams and operations
#include <iomanip>
using namespace std;

// a container for a set of identifiers.
// Because of this line, there is no need to use std::cout
#include <typeinfo> // This header file is included for using typeid.
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  // A library with functions for testing characters.

#include "wrapper.hh"
template <class T>
class Stack
{
private:
    struct Node
    {
        T *element;
        Node *next;
    };
    Node *top;
public:
    Stack();
    ~Stack();
    void push(T *obj);  // push object onto stack.
    T *pop();           // pop the top element off the stack
    T *peek();          // see what the next element on the stack is
    bool isEmpty();     // does the stack have any elements?

    T *findMin(Stack<T>);
    T *findMax(Stack<T>);

};

template <class T>
Stack<T>::Stack()
{
    top = NULL;
}

template <class T>
Stack<T>::~Stack()
{
    while (top != NULL)
    {
        Node *tmp = top;
        top = top->next;
        delete tmp;
    }
}

template <class T>
void Stack<T>::push(T *obj)
{
    Node *n = new Node;
    n->element = obj;
    n->next = top;
    top = n;
}

template <class T>
T *Stack<T>::pop()
{
    if (top == NULL)
        fatal("The stack is empty");
    Node *tmp = top;
    T *element = top->element;
    top = top->next;
    delete tmp;
    return element;
}

template <class T>
T *Stack<T>::peek()
{
    if (top == NULL)
        fatal("The stack is empty.");
    T *element = top->element;
    return element;
}

template <class T>
bool Stack<T>::isEmpty()
{
    return top == NULL;
}

template<class T>
T *findMin(Stack<T> stack)
{
    Stack<T> temp;
    if (stack.isEmpty())
        fatal("This stack is empty.");

    T *ref = stack.pop(); // use &ref to disassociate reference.
    temp.push(ref);
    T *min = ref;
    while (!stack.isEmpty())
    {
        T *element = stack.pop(); // &element, scope for one iter only
        temp.push(element);
        if (element <= min)
            min = element;
    }
    // restore stack.
    while (!temp.isEmpty())
        stack.push(temp.pop());

    return min;
}

template<class T>
T *findMax(Stack<T> stack)
{
    Stack<T> temp;
    if (stack.isEmpty())
        fatal("This stack is empty.");

    T *ref = stack.pop(); // use &ref to disassociate reference.
    temp.push(ref);
    T *max = ref;
    while (!stack.isEmpty())
    {
        T *element = stack.pop(); // &element, scope for one iter only
        temp.push(element);
        if (max <= element )
            max = element;
    }
    // restore stack.
    while (!temp.isEmpty())
        stack.push(temp.pop());

    return max;
}
