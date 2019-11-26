/*
	File: Stack.h
	Contains: Stack
*/

#pragma once
#include <iostream>
#include <sstream>

using namespace std;

/// <summary>
/// The Stack class uses a dynamically created array to store the values.
/// </summary>
template <typename T>
class Stack
{
private:
	T* data;				//Pointer to the array
	unsigned int size;		//The number of values in the stack
	unsigned int capacity;	//The maximum allowed number of values in the stack

public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	Stack()
	{
		//Sets the initial values and creates the array
		capacity = 10;
		size = 0;
		data = new T[capacity];
		memset(data, 0, sizeof(T) * capacity);
	}

	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="_capacity">The maximum allowed number of values in the stack.</param>
	Stack(unsigned int _capacity)
	{
		//Sets values and creates the array
		capacity = _capacity;
		size = 0;
		data = new T[capacity];
		memset(data, 0, sizeof(T) * capacity);
	}
	
	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="copy">The stack to copy.</param>
	Stack(const Stack<T>& copy)
	{
		//Copy the data from the copy stack to this stack.
		capacity = copy.capacity;
		size = copy.size;
		data = new T[capacity];
		memcpy(data, copy.data, sizeof(T) * capacity);
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~Stack()
	{
		delete[] data;	//Delete the data
	}

	/// <summary>
	/// Push a value to the stack if there is space.
	/// </summary>
	/// <param name="value">The value to push.</param>
	void Push(const T& value)
	{
		if (size < capacity)
		{
			data[size] = value;
			++size;
		}
	}

	/// <summary>
	/// Pop a value off the stack.
	/// </summary>
	void Pop()
	{
		if (size > 0)
			--size;
	}

	/// <summary>
	/// Clear the stack of all values.
	/// </summary>
	void Clear()
	{
		size = 0;
	}

	/// <summary>
	/// Getter for the size of the stack.
	/// </summary>
	/// <returns>The number of values in the stack.</returns>
	unsigned int Size() const
	{
		return size;
	}

	/// <summary>
	/// Getter for the capacity of the stack.
	/// </summary>
	/// <returns>The maximum number of values allowed in the stack.</returns>
	unsigned int Capacity() const
	{
		return capacity;
	}

	/// <summary>
	/// Check if the stack is empty.
	/// </summary>
	/// <returns>True if the stack is empty.</returns>
	bool Empty() const
	{
		return size == 0;
	}

	/// <summary>
	/// Getter for the top value of the stack.
	/// </summary>
	/// <returns>The top value of the stack.</returns>
	T& Top() const
	{
		if (size > 0)
			return data[size - 1];

		//Throw an error if the top value doesn't exist
		throw out_of_range("Top value does not exist.");
	}

	/// <summary>
	/// = operator overload.
	/// Copies the values from another stack into this stack.
	/// </summary>
	/// <param name="other">The stack to copy values from.</param>
	/// <returns>This stack with values from the other stack.</returns>
	Stack<T>& operator= (const Stack<T>& other)
	{
		delete[] data;	//Delete the data in this stack first

		//Set the values and copy the data over
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];
		memcpy(data, other.data, sizeof(T) * capacity);
		return *this;
	}

	/// <summary>
	/// << operator overload
	/// Allows displaying the value at the top of the stack to an output stream.
	/// </summary>
	/// <param name="os">The output stream to use.</param>
	/// <param name="stack">The stack to display.</param>
	/// <returns>The output stream with the top value of the stack displayed.</returns>
	friend ostream& operator<< (ostream& os, const Stack<T>& stack)
	{
		if (!stack.Empty())
			os << stack.Top();
		else
			os << "Empty";
		return os;
	}

	/// <summary>
	/// Print details about the stack to std::cout.
	/// </summary>
	void PrintDetails() const
	{
		cout << "Size: " << size << "   ";
		cout << "Capacity: " << capacity << "   ";
		if (!Empty())
			cout << Top();
		else
			cout << "Empty";
		cout << endl;
	}

	/// <summary>
	/// Get the list represented as a string.
	/// </summary>
	/// <returns>A string representation of the list.</returns>
	string ToString() const
	{
		ostringstream stream;
		stream << *this;
		return stream.str();
	}
};