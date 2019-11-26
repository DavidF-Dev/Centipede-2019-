/*
	File: Dequeue.h
	Contains: Dequeue, Node
*/

#pragma once
#include <iostream>
#include <sstream>

using namespace std;

/// <summary>
/// The Dequeue class allows pushing and popping from both ends of the container.
/// I implemented it using a 'linked list' style format with nodes and pointers.
/// However, unlike the linked list class, the node does not contain a pointer to the previous node.
/// </summary>
template <typename T>
class Dequeue
{
private:
	/// <summary>
	/// The Node class contains data and a pointer to the next node.
	/// </summary>
	template <typename T>
	class Node
	{
	public:
		T data;			//Data
		Node* next;		//Pointer to the next node

		/// <summary>
		/// Overloaded constructor
		/// </summary>
		/// <param name="_data">The data to store in the node.</param>
		/// <param name="_next">A pointer to the next node.</param>
		Node(const T& _data, Node* _next)
		{
			data = _data;
			next = _next;
		}
	};

	Node<T>* head;			//Head of the container
	Node<T>* tail;			//Tail of the container
	unsigned int size;		//Size of the container

public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	Dequeue()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="copy">The Dequeue to copy into this one.</param>
	Dequeue(const Dequeue<T>& copy)
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
		Node<T>* node = copy.head;
		while (node != nullptr)	//Push all the nodes from the copy Dequeue into this one
		{
			PushBack(node->data);
			node = node->next;
		}
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~Dequeue()
	{
		while (head != nullptr)		//Pop until there are none left
			PopFront();
	}

	/// <summary>
	/// Push a value to the front of the Dequeue.
	/// </summary>
	/// <param name="value">The value to push.</param>
	void PushFront(const T& value)
	{
		if (head == nullptr)	//If there are no nodes yet, simply set the head and tail
		{
			head = new Node<T>(value, nullptr);
			tail = head;
		}
		else	//Otherwise create a new node and re-adjust the pointers
		{
			Node<T>* newNode = new Node<T>(value, head);
			newNode->next = head;
			head = newNode;
		}
		++size;
	}

	/// <summary>
	/// Pop the node off the front of the Dequeue.
	/// </summary>
	void PopFront()
	{
		if (head == nullptr)		//If there are no nodes, then return
			return;
		else if (head == tail)		//If there is only 1 node, then delete the head
		{
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		else		//Otherwise, readjust the pointers at the head
		{
			Node<T>* oldNode = head;
			head = oldNode->next;
			delete oldNode;
		}
		--size;
	}

	/// <summary>
	/// Push a value to the end of the Dequeue.
	/// </summary>
	/// <param name="value">The value to push to the end.</param>
	void PushBack(const T& value)
	{
		if (tail == nullptr)	//If there are no nodes yet, then just set the tail
		{
			tail = new Node<T>(value, nullptr);
			head = tail;
		}
		else	//Otherwise create a new node and readjust the pointers
		{
			Node<T>* newNode = new Node<T>(value, nullptr);
			tail->next = newNode;
			tail = newNode;
		}
		++size;
	}

	/// <summary>
	/// Pop the last node off the Dequeue.
	/// Works different from a Double-Linked List because we don't know the node before the tail.
	/// </summary>
	void PopBack()
	{
		if (tail == nullptr)		//If there are no nodes, then return
			return;
		else if (tail == head)		//If there is only 1 node, then delete the tail
		{
			delete tail;
			tail = nullptr;
			head = nullptr;
		}
		else	//Otherwise find the node before the tail and set its next to nullptr
		{
			Node<T>* node = head;
			while (node->next->next != nullptr)		//Loop through the nodes until we find the node before the tail
				node = node->next;
			delete node->next;		//Delete the tail
			tail = node;			//Set the new tail
			tail->next = nullptr;	//Set the tail's next pointer to nullptr
		}
		--size;
	}

	/// <summary>
	/// Empty the Dequeue of all nodes.
	/// </summary>
	void Clear()
	{
		while (head != nullptr)		//Pop the values until the Dequeue is empty
			PopFront();
		size = 0;
	}

	/// <summary>
	/// Getter for the size of the Dequeue.
	/// </summary>
	/// <returns>The number of elements in the Dequeue.</returns>
	unsigned int Size() const
	{
		return size;
	}

	/// <summary>
	/// Whether the Dequeue is empty or not.
	/// </summary>
	/// <returns>True if the Dequeue is empty.</returns>
	bool Empty() const
	{
		return size == 0;
	}

	/// <summary>
	/// Getter for the first element in the Dequeue.
	/// </summary>
	/// <returns>The first element.</returns>
	T& Top() const
	{
		return head->data;
	}

	/// <summary>
	/// Getter for the last element in the Dequeue.
	/// </summary>
	/// <returns>The last element.</returns>
	T& Bottom() const
	{
		return tail->data;
	}

	/// <summary>
	/// Assignment operator overload.
	/// </summary>
	/// <param name="other">The copy.</param>
	/// <returns>This Dequeue with new data.</returns>
	Dequeue<T>& operator= (const Dequeue<T>& other)
	{
		Clear();
		Node<T>* node = other.head;
		while (node != nullptr)		//Push the nodes from the other Dequeue into this one
		{
			PushBack(node->data);
			node = node->next;
		}
		return *this;
	}

	/// <summary>
	/// << operator overload.
	/// Allows outputting this container to an output stream.
	/// </summary>
	/// <param name="os">The output stream to display to.</param>
	/// <param name="dequeue">The Dequeue to display.</param>
	/// <returns>The output stream with the Dequeue displayed in it.</returns>
	friend ostream& operator<< (ostream& os, const Dequeue<T>& dequeue)
	{
		os << "[";
		if (!dequeue.Empty())
			os << "Top: " << dequeue.Top() << ", Bottom: " << dequeue.Bottom();
		os << "]";
		return os;
	}

	/// <summary>
	/// Print out details about this Dequeue to std::cout.
	/// </summary>
	void PrintDetails() const
	{
		cout << "Size: " << size << "  ";
		Node<T>* node = head;
		while (node != nullptr)
		{
			cout << node->data;
			cout << " ";
			node = node->next;
		}
		cout << endl;
	}

	/// <summary>
	/// Get the dequeue represented as a string.
	/// </summary>
	/// <returns>A string representation of the dequeue.</returns>
	string ToString() const
	{
		ostringstream stream;
		stream << *this;
		return stream.str();
	}
};