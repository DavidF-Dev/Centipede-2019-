#pragma once
#include <iostream>
#include <math.h>

using namespace std;

template <typename T>
class Heap
{
private:
	T* data;							//Array
	unsigned int size;					//Size of the heap
	const unsigned int MAX_SIZE = 100;	//Maximum size of the heap

	/// <summary>
	/// Swap two values in the heap.
	/// </summary>
	/// <param name="a">Value A.</param>
	/// <param name="b">Value B.</param>
	void Swap(T* a, T* b)
	{
		T temp = *a;
		*a = *b;
		*b = temp;
	}

	/// <summary>
	/// Recursively prints the tree to std::cout
	/// https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
	/// </summary>
	/// <param name="index">The index to process. (Initially 0)</param>
	/// <param name="space">The space between the levels. (Initially 0)</param>
	void PrintTree(unsigned int index, int space) const
	{
		//Exit if the index isn't valid
		if (index == -1)
			return;

		//Increase the distance between the levels
		space += 5;

		//Process the right child
		PrintTree(GetSecondChild(index), space);

		//Print the current node
		cout << endl;
		for (int i = 5; i < space; ++i)
			cout << " ";
		cout << data[index] << endl;

		//Process the left child
		PrintTree(GetFirstChild(index), space);
	}

public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	Heap()
	{
		size = 0;
		data = new T[MAX_SIZE];
		memset(data, 0, sizeof(T) * MAX_SIZE);
	}

	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="rootValue">The value of the root.</param>
	Heap(const T& rootValue)
	{
		size = 0;
		data = new T[MAX_SIZE];
		memset(data, 0, sizeof(T) * MAX_SIZE);
		Push(rootValue);
	}

	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="copy"></param>
	Heap(const Heap& copy)
	{
		size = copy.size;
		data = new T[MAX_SIZE];
		memcpy(data, copy.data, sizeof(T) * MAX_SIZE);
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~Heap()
	{
		delete[] data;
	}

	/// <summary>
	/// Adds a new value to the heap.
	/// </summary>
	/// <param name="value">The value to add to the heap.</param>
	void Push(const T& value)
	{
		//Exit if the heap cannot fit another value
		if (size == MAX_SIZE)
			return;

		//Add the value to the end of the heap
		data[size] = value;
		++size;

		//Exit if this was the first value added
		if (size == 1)
			return;

		//Get the current index of this value
		unsigned int index = size - 1;				

		//Get the index of the parent of this value
		unsigned int parentIndex = GetParent(index);

		//Swap the value with its parent if the parent is smaller than the value
		//Repeat until there are no more swaps required
		while (index > 0 && data[parentIndex] < data[index])
		{
			//Swap the value and its parent
			Swap(&data[parentIndex], &data[index]);

			//Set the index to the parent's index
			index = parentIndex;

			//Find the new indexes' parent index
			parentIndex = GetParent(index);
		}
	}

	/// <summary>
	/// Remove the root element from the tree.
	/// </summary>
	void Pop()
	{
		if (size > 0)
			--size;
	}

	/// <summary>
	/// Removes a value from the heap.
	/// </summary>
	/// <param name="value">The value to remove from the heap.</param>
	void Remove(const T& value)
	{
		if (size == 0)
			return;
		else if (data[size - 1] == value)
			Pop();
		else if (Contains(value))
		{
			unsigned int index = Find(value);		//Find the index of the value to be removed
			Swap(&data[index], &data[size - 1]);	//Swap the value to be removed with the last value
			Pop();									//Pop the last value (reduces the size)

			//Swap the value with its child if the value is larger - pick the larger child
			//Repeat until there are no more swaps required
			while (index < size										//Index is in the bounds of the heap. and
				&& ((HasFirstChild(index) && data[index] < data[GetFirstChild(index)])		//The value is larger than the first child, or
					|| (HasSecondChild(index) && data[index] < data[GetSecondChild(index)])))	//The value is larger than the second child
			{
				//Get the larger child
				int largerIndex = (data[GetFirstChild(index)] > data[GetSecondChild(index)])
					? GetFirstChild(index) : GetSecondChild(index);

				//Swap the value and the larger child
				Swap(&data[index], &data[largerIndex]);

				//Set the index to the larger child's index
				index = largerIndex;
			}
		}
	}

	/// <summary>
	/// Check if the heap contains a given value.
	/// </summary>
	/// <param name="value">The value to check is in the heap.</param>
	/// <returns>True if the value is contained in the heap.</returns>
	bool Contains(const T& value) const
	{
		return Find(value) != -1;
	}

	/// <summary>
	/// Removes all values from the tree.
	/// </summary>
	void Clear()
	{
		size = 0;
	}

	/// <summary>
	/// Get the parent index of the given index.
	/// </summary>
	/// <param name="index">The index to get the parent index of.</param>
	/// <returns>The index of the parent, -1 if there isn't one.</returns>
	int GetParent(unsigned int index) const
	{
		if (index == 0)
			return -1;
		return (int)floor((index - 1) / 2);
	}

	/// <summary>
	/// Get the first child index of the given index.
	/// </summary>
	/// <param name="index">The index to get the first child of.</param>
	/// <returns>The index of the first child, -1 if there isn't one.</returns>
	int GetFirstChild(unsigned int index) const
	{
		unsigned int result = (2 * index) + 1;
		if (result >= size)
			return -1;
		return result;
	}

	/// <summary>
	/// Get the second child index of the given index.
	/// </summary>
	/// <param name="index">The index to get the second child of.</param>
	/// <returns>The index of the second child, -1 if there isn't one.</returns>
	int GetSecondChild(unsigned int index) const
	{
		unsigned int result = (2 * index) + 2;
		if (result >= size)
			return -1;
		return result;
	}

	/// <summary>
	/// Check if the given index has a parent.
	/// </summary>
	/// <param name="index">The index to check.</param>
	/// <returns>True if a parent exists for the given index.</returns>
	bool HasParent(unsigned int index) const
	{
		return GetParent(index) != -1;
	}

	/// <summary>
	/// Check if the given index has a first child.
	/// </summary>
	/// <param name="index">The index to check.</param>
	/// <returns>True if a first child exists for the given index.</returns>
	bool HasFirstChild(unsigned int index) const
	{
		return GetFirstChild(index) != -1;
	}

	/// <summary>
	/// Check if the given index has a first second.
	/// </summary>
	/// <param name="index">The index to check.</param>
	/// <returns>True if a second child exists for the given index.</returns>
	bool HasSecondChild(unsigned int index) const
	{
		return GetSecondChild(index) != -1;
	}

	/// <summary>
	/// Find the index of a value in the heap.
	/// </summary>
	/// <param name="value">The value to search for.</param>
	/// <returns>The index of the value, -1 otherwise.</returns>
	int Find(const T& value) const
	{
		for (unsigned int i = 0; i < size; ++i)
			if (data[i] == value)
				return i;
		return -1;
	}

	/// <summary>
	/// Get the index of the root of the heap.
	/// </summary>
	/// <returns>The index of the root, -1 if the tree is empty.</returns>
	int GetRootIndex() const
	{
		return size == 0 ? -1 : 0;
	}

	/// <summary>
	/// Get the value of the root element of the tree.
	/// </summary>
	/// <returns>The value of the root element of the tree.</returns>
	T& Peek() const
	{
		if (size > 0)
			return data[0];

		//Throw an exception if the root does not exist
		throw out_of_range("Root value does not exist.");
	}

	/// <summary>
	/// Get the size of the tree.
	/// </summary>
	/// <returns>The number of nodes in the tree.</returns>
	unsigned int Size() const
	{
		return size;
	}

	/// <summary>
	/// Get a value from the heap.
	/// </summary>
	/// <param name="index">The index of the value.</param>
	/// <returns>The value.</returns>
	T& operator[] (unsigned int index) const
	{
		if (index >= 0 && index < size)
			return data[index];
		
		//Throw an exception if the index is not within the range of the heap
		throw out_of_range("Index out of range.");
	}

	/// <summary>
	/// Assignment operator overload.
	/// </summary>
	/// <param name="other">The other heap to assign to this one.</param>
	/// <returns>This heap with the values of the other heap.</returns>
	Heap& operator= (const Heap& other)
	{
		delete[] data;
		size = other.size;
		data = new T[MAX_SIZE];
		memcpy(data, other.data, sizeof(T) * MAX_SIZE);
		return *this;
	}

	/// <summary>
	/// Recursively prints the heap to an ostream.
	/// https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
	/// </summary>
	/// <param name="os">The ostream the print the heap to.</param>
	/// <param name="node">The current node to process. (Initially root)</param>
	/// <param name="space">The space between the levels. (Initially 0)</param>
	friend void PrintTreeF(ostream& os, const Heap<T>& heap, unsigned int index, int space)
	{
		//Exit if the index isn't valid
		if (index == -1)
			return;

		//Increase the distance between the levels
		space += 5;

		//Process the right child
		PrintTreeF(os, heap, heap.GetSecondChild(index), space);

		//Print the current node
		os << endl;
		for (int i = 5; i < space; ++i)
			os << " ";
		os << heap[index] << endl;

		//Process the left child
		PrintTreeF(os, heap, heap.GetFirstChild(index), space);
	}

	/// <summary>
	/// Overloaded << operator.
	/// Displays the heap to an ostream.
	/// </summary>
	/// <param name="os">The ostream to print the heap to.</param>
	/// <param name="tree">The heap to print.</param>
	/// <returns>The ostream with the heap printed to it.</returns>
	friend ostream& operator<< (ostream& os, const Heap<T>& heap)
	{
		//Call the other friend function to recursively print the tree
		PrintTreeF(os, heap, heap.GetRootIndex(), 0);
		return os;
	}

	/// <summary>
	/// Print details about the tree.
	/// </summary>
	void PrintDetails() const
	{
		cout << "Size: " << size << endl;
		if (size > 0)
		{
			//Call a function to recursively print the heap to std::cout
			PrintTree(GetRootIndex(), 0);
		}
		else
			cout << "Empty" << endl;
		cout << endl;
	}

	/// <summary>
	/// Get the heap represented as a string.
	/// https://stackoverflow.com/questions/3513173/converting-ostream-into-standard-string
	/// http://www.cplusplus.com/reference/sstream/stringstream/str/
	/// </summary>
	/// <returns>A string representation of the heap.</returns>
	string ToString() const
	{
		ostringstream stream;
		stream << *this;
		return stream.str();
	}
};