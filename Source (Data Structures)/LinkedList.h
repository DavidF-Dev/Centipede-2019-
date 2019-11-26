/*
	File: LinkedList.h
	Contains: LinkedList, LinkedListNode, LinkedListIterator
*/

#pragma once
#include <iostream>
#include <sstream>

using namespace std;

/// <summary>
/// The Linked List is a container that links elements (nodes) together using pointers.
/// This is a Doubly-Linked List because each node containers a pointer to the next node and previous node.
/// The class also implements a custom iterator to allow traversing the list.
/// </summary>
template <typename T>
class LinkedList
{
private:
	/// <summary>
	/// The Linked List Node contains the data and a pointer to the next & previous node.
	/// </summary>
	template <typename T>
	class LinkedListNode
	{
	public:
		T data;						//The data in the node
		LinkedListNode* next;		//A pointer to the next node
		LinkedListNode* previous;	//A pointer to the previous node

		/// <summary>
		/// Overloaded constructor.
		/// </summary>
		/// <param name="_next">A pointer to the next node/</param>
		/// <param name="_previous">A pointer to the previous node.</param>
		LinkedListNode(LinkedListNode* _next, LinkedListNode* _previous)
		{
			_next = next;
			_previous = previous;
		}
		
		/// <summary>
		/// Overloaded constructor.
		/// </summary>
		/// <param name="_data">The data to store in the node.</param>
		/// <param name="_next">A pointer to the next node.</param>
		/// <param name="_previous">A pointer to the previous node.</param>
		LinkedListNode(const T& _data, LinkedListNode* _next, LinkedListNode* _previous)
		{
			data = _data;
			next = _next;
			previous = _previous;
		}
	};

public:
	/// <summary>
	/// The Linked List Iterator class allows iterating through a linked list.
	/// Implementation helped by https://codereview.stackexchange.com/questions/74609/custom-iterator-for-a-linked-list-class
	/// </summary>
	template <typename T>
	class LinkedListIterator
	{
	private:
		LinkedListNode<T>* node;	//The node that this iterator is pointing to

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		LinkedListIterator()
		{
			node = nullptr;
		}

		/// <summary>
		/// Overloaded constructor.
		/// </summary>
		/// <param name="_node">A pointer to to node that this iterator should point to.</param>
		LinkedListIterator(LinkedListNode<T>* _node)
		{
			node = _node;
		}

		/// <summary>
		/// == operator overload.
		/// Checks if this iterator is equal to another by testing if the node pointers are equal.
		/// I.e. two iterators with the same node are the same iterator.
		/// </summary>
		/// <param name="other">The other iterator to check against.</param>
		/// <returns>True if the two iterators are equal.</returns>
		bool operator== (const LinkedListIterator<T>& other) const
		{
			return node != nullptr && other.node != nullptr && node == other.node;
		}

		/// <summary>
		/// != operator overload.
		/// Checks if this iterator is not equal to another by testing if the node pointers are different.
		/// </summary>
		/// <param name="other">The other iterator to check against.</param>
		/// <returns>True if the two iterators are not equal.</returns>
		bool operator!= (const LinkedListIterator<T>& other) const
		{
			return !(*this == other);
		}

		LinkedListIterator<T> Next() const
		{
			LinkedListIterator<T> iter(node);
			if (iter.node != nullptr)
				iter.node = iter.node->next;
			return iter;
		}

		LinkedListIterator<T> Next(unsigned int increment) const
		{
			LinkedListIterator<T> iter(node);
			while (increment > 0)
			{
				if (iter.node != nullptr)
					iter.node = iter.node->next;
				else
					break;
				--increment;
			}
			return iter;
		}

		LinkedListIterator<T> Previous() const
		{
			LinkedListIterator<T> iter(node);
			if (iter.node != nullptr)
				iter.node = iter.node->previous;
			return iter;
		}

		LinkedListIterator<T> Previous(unsigned int increment) const
		{
			LinkedListIterator<T> iter(node);
			while (increment > 0)
			{
				if (iter.node != nullptr)
					iter.node = iter.node->previous;
				else
					break;
				--increment;
			}
			return iter;
		}

		/// <summary>
		/// ++i operator overload.
		/// Will move this iterator to point to the next node.
		/// </summary>
		/// <returns>This iterator representing the next node.</returns>
		LinkedListIterator& operator++ ()
		{
			if (node != nullptr)
				node = node->next;
			return *this;
		}

		/// <summary>
		/// --i operator overload.
		/// Will move this iterator to point to the previous node.
		/// </summary>
		/// <returns>This iterator representing the previous node.</returns>
		LinkedListIterator& operator-- ()
		{
			if (node != nullptr)
				node = node->previous;
			return *this;
		}

		/// <summary>
		/// * de-reference operator overload.
		/// Will return the data within the node.
		/// </summary>
		/// <returns>The data of the node that the iterator is representing.</returns>
		T& operator* () const
		{
			if (node != nullptr)
				return node->data;

			//Throw an error if the node does not exist
			throw out_of_range("Node at this iterator does not exist.");
		}

		/// <summary>
		/// -> arrow operator overload.
		/// Will return the data within the node.
		/// </summary>
		/// <returns>The data of the node that the iterator is representing.</returns>
		T& operator-> () const
		{
			if (node != nullptr)
				return node->data;
			
			//Throw an error if the node does not exist
			throw out_of_range("Node at this iterator does not exist.");
		}
	};

private:
	LinkedListNode<T>* head;	//The head of the linked list (first node)
	LinkedListNode<T>* tail;	//The tail of the linked list (last node)
	LinkedListNode<T>* end;		//One past the tail used for iterating through the linked list
	unsigned int size;			//The number of nodes in the linked list

	/// <summary>
	/// A function that removes a specific node from the linked list.
	/// This is private because the linked list node class is also private.
	/// </summary>
	/// <param name="node"></param>
	void Remove(LinkedListNode<T>* node)
	{
		if (node == head)		//If we are removing the head, then pop the front node
			PopFront();
		else if (node == tail)	//If we are removing the tail, then pop the back node
			PopBack();
		else
		{
			//Otherwise delete the node and readjust the pointers
			node->previous->next = node->next;
			node->next->previous = node->previous;
			delete node;
			--size;
		}
	}

	/// <summary>
	/// Find a node that an iterator points to.
	/// </summary>
	/// <param name="iter">The iterator.</param>
	/// <returns>The node that the iterator points to.</returns>
	LinkedListNode<T>* FindNode(LinkedListIterator<T> iter) const
	{
		//If the linked list is empty, then return
		if (size == 0)
			return nullptr;
		//If the iterator points to the first position, return the first node
		else if (iter == Begin())
			return head;
		//If the iterator points to the second last position, return the last node
		else if (iter == End().Previous())
			return tail;
		else
		{
			//Find the node that the iterator points to and return it
			LinkedListNode<T>* node = head->next;
			for (auto i = Begin().Next(); i != End().Previous(); ++i)
			{
				if (iter == i)
					return node;
				node = node->next;
			}
		}

		return nullptr;
	}

	/// <summary>
	/// Swap two pointers.
	/// </summary>
	/// <param name="a">Pointer A.</param>
	/// <param name="b">Pointer B.</param>
	void Swap(T* a, T* b)
	{
		T temp = *a;
		*a = *b;
		*b = temp;
	}

public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	LinkedList()
	{
		//Set all the values to their initial state
		head = new LinkedListNode<T>(nullptr, nullptr);
		tail = head;
		end = new LinkedListNode<T>(nullptr, tail);
		head->next = end;
		size = 0;
	}
	
	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="copy">The list we are copying.</param>
	LinkedList(const LinkedList<T>& copy)
	{
		//Set all the values to their initial state
		head = new LinkedListNode<T>(nullptr, nullptr);
		tail = head;
		end = new LinkedListNode<T>(nullptr, tail);
		head->next = end;
		size = 0;

		//Iterate through the copy and push its data into this list
		for (auto i = copy.Begin(); i != copy.End(); ++i)
			PushBack(*i);
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~LinkedList()
	{
		//Pop the nodes until none remain
		while (size > 0)
			PopFront();

		//Delete remaining pointers
		delete end;
		delete head;
	}
	
	/// <summary>
	/// Push a value to the front of the linked list.
	/// </summary>
	/// <param name="value">The value to push.</param>
	void PushFront(const T& value)
	{
		if (size == 0)	//If there are no nodes, set the data on the head
			head->data = value;
		else
		{
			//Create a new node and readjust the head pointer
			LinkedListNode<T>* newNode = new LinkedListNode<T>(value, head, nullptr);
			head->previous = newNode;
			head = newNode;
		}
		++size;
	}

	/// <summary>
	/// Pop a value off the front of the linked list.
	/// </summary>
	void PopFront()
	{
		if (size == 0)	//If there are no nodes, just return
			return;
		else if (size == 1)	//If there is only one node...
		{
			//Set the node after the head to the end node and point to the tail to the head
			head->next = end;
			tail = head;
			end->previous = tail;
		}
		else
		{
			//Delete the head and readjust the pointers
			head = head->next;
			delete head->previous;
			head->previous = nullptr;
		}
		--size;
	}

	/// <summary>
	/// Push a value to the end of the linked list.
	/// </summary>
	/// <param name="value">The value to push.</param>
	void PushBack(const T& value)
	{
		if (size == 0)	//If there are no nodes, set the data on the tail
			tail->data = value;
		else
		{
			//Create a new node and readjust the tail pointer and the end pointer
			LinkedListNode<T>* newNode = new LinkedListNode<T>(value, nullptr, tail);
			tail->next = newNode;
			tail = newNode;
			tail->next = end;
		}
		end->previous = tail;
		++size;
	}

	/// <summary>
	/// Pop a value off the back of the linked list.
	/// </summary>
	void PopBack()
	{
		if (size == 0)	//If there are no nodes, just return
			return;
		else if (size == 1)	//If there is only one node...
		{
			//Set the node after the head to the end node and point to the tail to the head
			head->next = end;
			tail = head;
			end->previous = tail;
		}
		else
		{
			//Delete the tail and readjust the pointers, and set the end pointer
			tail = tail->previous;
			delete tail->next;
			tail->next = end;
			end->previous = tail;
		}
		--size;
	}

	/// <summary>
	/// Inserts a value before the given iterator.
	/// </summary>
	/// <param name="iter">The iterator to insert a node before.</param>
	/// <param name="value">The value to insert into the linked list.</param>
	void Insert(LinkedListIterator<T>& iter, const T& value)
	{
		//If the linked list is empty or the iterator points to the first position, push to the front
		if (size == 0 || iter == Begin())
			PushFront(value);
		//If the iterator points to the last position, push to the back
		else if (iter == End())
			PushBack(value);
		else
		{
			//Find the node
			LinkedListNode<T>* node = FindNode(iter);
			if (node != nullptr)
			{
				//Insert the node into the linked list
				LinkedListNode<T>* newNode = new LinkedListNode<T>(value, node, node->previous);
				node->previous->next = newNode;
				node->previous = newNode;
			}
		}
	}

	/// <summary>
	/// Remove all occurrences of a specific value from the linked list.
	/// </summary>
	/// <param name="value">The value to remove from the linked list.</param>
	void Remove(const T& value)
	{
		if (size == 0)	//If there are no nodes then return
			return;
		else
		{
			//Loop through the nodes in the linked list and push the ones to be removed to another linked list
			LinkedListNode<T>* node = head;
			LinkedList<LinkedListNode<T>*> toRemove = LinkedList<LinkedListNode<T>*>();
			while (node != end)
			{
				//If the data matches the value to remove add it to the list of nodes to be removed
				if (node->data == value)
					toRemove.PushBack(node);
				node = node->next;
			}

			//Loop through the nodes in the list of nodes to remove, removing the nodes one by one
			while (toRemove.Size() > 0)
			{
				Remove(toRemove.Last());
				toRemove.PopBack();
			}
		}
	}

	/// <summary>
	/// Erase a specific node from the linked list.
	/// </summary>
	/// <param name="iter">The position of the node to remove.</param>
	void Erase(LinkedListIterator<T>& iter)
	{
		LinkedListNode<T>* node = FindNode(iter);
		if (node != nullptr)
			Remove(node);
	}

	/// <summary>
	/// Clear all values from the linked list.
	/// </summary>
	void Clear()
	{
		//Pop the front node until there are no more nodes
		while (size > 0)
			PopFront();
	}

	/// <summary>
	/// Getter for the size of the linked list.
	/// </summary>
	/// <returns>The number of nodes in the linked list.</returns>
	unsigned int Size() const
	{
		return size;
	}

	/// <summary>
	/// Check if the linked list is empty.
	/// </summary>
	/// <returns>True, if empty.</returns>
	bool Empty() const
	{
		return size == 0;
	}

	/// <summary>
	/// Sort the linked list using a bubble sort.
	/// </summary>
	void BubbleSort()
	{
		if (size < 2)
			return;

		bool sorted = false;
		while (!sorted)
		{
			sorted = true;

			LinkedListNode<T>* node = head;
			while (node != tail)
			{
				//If this node's data is greater than the next node's data,
				//swap the data
				if (node->data > node->next->data)
				{
					Swap(&node->data, &node->next->data);
					sorted = false;
				}
				node = node->next;
			}
		}
	}

	/// <summary>
	/// Performa basic linear search for a value.
	/// </summary>
	/// <param name="value">The value to search for.</param>
	/// <returns>The iterator pointing to the value if found, otherwise points to End().</returns>
	LinkedListIterator<T> LinearSearch(const T& value) const
	{
		for (LinkedListIterator<T> iter = Begin(); iter != End(); ++iter)
			if (*iter == value)
				return iter;
		return End();
	}

	/// <summary>
	/// Getter for the first value in the linked list.
	/// </summary>
	/// <returns>The first value in the linked list.</returns>
	T& First() const
	{
		if (head != nullptr)
			return head->data;

		//Throw an error if the head does not exist
		throw out_of_range("Head value does not exist.");
	}

	/// <summary>
	/// Getter for the last value in the linked list.
	/// </summary>
	/// <returns>The last value in the linked list.</returns>
	T& Last() const
	{
		if (tail != nullptr)
			return tail->data;
		
		//Throw an error if the tail does not exist
		throw out_of_range("Tail value does not exist.");
	}

	/// <summary>
	/// A getter for an iterator pointing to the beginning of the linked list.
	/// </summary>
	/// <returns>An iterator at the start of the linked list.</returns>
	LinkedListIterator<T> Begin() const
	{
		return LinkedListIterator<T>(head);
	}

	/// <summary>
	/// A getter for an iterator pointing to the end of the linked list.
	/// </summary>
	/// <returns>An iterator at the end of the linked list.</returns>
	LinkedListIterator<T> End() const
	{
		if (size == 0)
			return Begin();
		return LinkedListIterator<T>(end);	//Use end to represent an iterator one past the tail
	}

	/// <summary>
	/// Assignment operator overload.
	/// Assigns this list the values of another list.
	/// </summary>
	/// <param name="other">The list to copy the values from.</param>
	/// <returns>This linked list with the values from the other linked list.</returns>
	LinkedList<T>& operator= (const LinkedList<T>& other)
	{
		//Clear this linked list first to clean up memory
		Clear();
		//Loop through the other linked list and push the values into our linked list
		for (auto i = other.Begin(); i != other.End(); ++i)
			PushBack(*i);
		return *this;
	}

	/// <summary>
	/// << operator overload.
	/// Allows displaying the linked list to an output stream.	
	/// </summary>
	/// <param name="os">The output stream to display to.</param>
	/// <param name="list">The linked list to display.</param>
	/// <returns>The output stream with the linked list displayed to it.</returns>
	friend ostream& operator<< (ostream& os, const LinkedList<T>& list)
	{
		os << "[";
		//Iterate through the linked list and output the data to the output stream
		for (auto i = list.Begin(); i != list.End(); ++i)
		{
			if (i != list.Begin())
				os << ", ";
			os << *i;
		}
		os << "]";
		return os;
	}

	/// <summary>
	/// Print details about the linked list to std::cout.
	/// </summary>
	void PrintDetails()
	{
		cout << "Size: " << size << "   ";
		for (auto i = Begin(); i != End(); ++i)
		{
			cout << *i;
			cout << " ";
		}
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