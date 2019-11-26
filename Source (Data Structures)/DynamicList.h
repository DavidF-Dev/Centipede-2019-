/*
	File: DynamicList.h
	Contains: List
*/

#pragma once
#include <iostream>
#include <sstream>

using namespace std;

/// <summary>
/// The List class is a Dynamic List container that expands in memory when needed.
/// </summary>
template <typename T>
class List
{
private:
	T* data;				//A pointer to the start of the array in the heap
	unsigned int size;		//The size of the list
	unsigned int capacity;	//The current capacity of the list
	const unsigned int MAX_CAPACITY = 500;	//The maximum capacity allowed to be reserved for the list

	/// <summary>
	/// Reduce the size of the list.
	/// Also checks if space should be freed in memory.
	/// </summary>
	/// <param name="amount">The amount to reduce the size by.</param>
	void ReduceSize(unsigned int amount)
	{
		if (amount > size)
			size = 0;
		else
			size -= amount;
		
		if (capacity > 5 && size < (capacity / 2) - 2)
			Discard(capacity / 2);
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
	
	/// <summary>
	/// Returns the minimum of two numbers.
	/// </summary>
	/// <param name="x">First number.</param>
	/// <param name="y">Second number.</param>
	/// <returns>The minimum of the two given numbers.</returns>
	int Min(int x, int y) const
	{
		return ((x <= y) ? x : y);
	}

	/// <summary>
	/// Quick sort chooses an element as a pivot and partitions around the pivot.
	/// This implementation chooses the last element as the pivot.
	/// https://www.geeksforgeeks.org/quick-sort/
	/// </summary>
	/// <param name="low">The starting index of the sort.</param>
	/// <param name="high">The ending index of the sort.</param>
	void QuickSort(int low, int high)
	{
		if (low < high)
		{
			int partitionIndex = Partition(low, high);	//Partitioning index
			QuickSort(low, partitionIndex - 1);		//Before partition
			QuickSort(partitionIndex + 1, high);	//After partition
		}
	}

	/// <summary>
	/// Takes last element as pivot and places the pivot element at its correct position in the sorted list.
	/// Places all smaller elements to the left and all greater elements to the right of the pivot.
	/// </summary>
	/// <param name="low">Lowest point of the list.</param>
	/// <param name="high">Highest point of the list.</param>
	/// <returns>Partitioning index.</returns>
	int Partition(int low, int high)
	{
		//Pivot (last element)
		T pivot = data[high];

		//Index of smaller element
		int i = (low - 1);

		for (int j = low; j <= high - 1; ++j)
		{
			//If the current element is smaller than or equal to the pivot
			if (data[j] <= pivot)
			{
				//Increase the index of the smaller element
				++i;
				Swap(&data[i], &data[j]);
			}
		}

		Swap(&data[i + 1], &data[high]);
		return (i + 1);
	}

	/// <summary>
	/// Heapify a sub-tree.
	/// https://www.geeksforgeeks.org/heap-sort/
	/// </summary>
	/// <param name="_size">Size of the heap.</param>
	/// <param name="index">Index in the heap.</param>
	void Heapify(int _size, int index)
	{
		int largest = index;	//Initialise largest as the root
		int left = 2 * index + 1;	//Left child
		int right = 2 * index + 2;	//Right child

		//If the left child is larger than the root, then set as largest
		if (left < _size && data[left] > data[largest])
			largest = left;

		//If the right child is larger than the root, then set as largest
		if (right < _size && data[right] > data[largest])
			largest = right;

		//If the largest is not the root
		if (largest != index)
		{
			//Swap the index and the largest
			Swap(&data[index], &data[largest]);

			//Recursively heapify the affected sub-tree
			Heapify(_size, largest);
		}
	}

public:
	/// <summary>
	/// Default constructor.
	/// Has an initial capacity of 5.
	/// </summary>
	List()
	{
		capacity = 5;
		size = 0;
		data = new T[capacity];
		memset(data, 0, sizeof(T) * capacity);
	}
	
	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="_capacity">The initial capacity of this list.</param>
	List(unsigned int _capacity)
	{
		if (_capacity == 0)
			capacity = 1;
		else if (_capacity > MAX_CAPACITY)
			capacity = MAX_CAPACITY;
		else
			capacity = _capacity;
		size = 0;
		data = new T[capacity];
		memset(data, 0, sizeof(T) * capacity);
	}
	
	/// <summary>
	/// Copy constructor.
	/// Copies the data from the copy to this list.
	/// </summary>
	/// <param name="copy">The copy.</param>
	List(const List& copy)
	{
		capacity = copy.capacity;
		size = copy.size;
		data = new T[capacity];
		memcpy(data, copy.data, sizeof(T) * capacity);
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~List()
	{
		delete[] data;
	}

	/// <summary>
	/// Increase the capacity of the list by a certain amount.
	/// </summary>
	/// <param name="amount">The amount to increase the capacity by.</param>
	void Reserve(unsigned int amount)
	{
		if (capacity == MAX_CAPACITY)					//If the capacity is at the maximum capacity, then return
			return;
		else if (capacity + amount > MAX_CAPACITY)		//If the new capacity will be greater than the maximum capacity, handle accordingly
		{
			T* newData = new T[MAX_CAPACITY];
			memcpy(newData, data, sizeof(T) * MAX_CAPACITY);
			delete[] data;
			data = newData;
			capacity = MAX_CAPACITY;
		}	
		else	//Otherwise handle like normal
		{
			T* newData = new T[capacity + amount];
			memcpy(newData, data, sizeof(T) * capacity);
			delete[] data;
			data = newData;
			capacity += amount;
		}
	}

	/// <summary>
	/// Decrease the capacity of the list by a certain amount.
	/// </summary>
	/// <param name="amount">The amount to decrease the capacity by.</param>
	void Discard(unsigned int amount)
	{
		//If the reduced capacity would be below or equal to zero, set the capacity to 1
		if (capacity - amount <= 0)
		{
			delete[] data;
			data = new T[1];
			memset(data, 0, sizeof(T));
			capacity = 1;
			size = 0;
		}
		else
		{
			T* newData = new T[capacity - amount];
			memcpy(newData, data, sizeof(T) * (capacity - amount));
			delete[] data;
			data = newData;
			capacity -= amount;

			if (size > capacity)
				size = capacity;
		}
	}

	/// <summary>
	/// Push a new value to the list.
	/// If it will not fit then increase the capacity.
	/// </summary>
	/// <param name="value">The value to push to the list.</param>
	void Push(const T& value)
	{
		if (size == capacity)		//If there is no more capacity then double the capacity
			Reserve(capacity);
		if (size != capacity)		//If there is more capacity now, then add the value
		{
			data[size] = value;
			++size;
		}
	}

	/// <summary>
	/// Insert a value at a specified index in the list.
	/// </summary>
	/// <param name="index">The index to insert at.</param>
	/// <param name="value">The value to insert.</param>
	void Insert(unsigned int index, const T& value)
	{
		//If an invalid index, return
		if (index < 0 || index > size)
			return;
		//If the index is the size, push to the back (handles if the size is 0)
		else if (index == size)
			Push(value);
		else
		{
			//Push the value to the back
			Push(value);

			//Move the value down the list to the desired index
			for (unsigned int i = size - 1; i != index; --i)
				Swap(&data[i], &data[i - 1]);

			//Set the value at the index (dunno why)
			data[index] = value;
		}
	}
	
	/// <summary>
	/// Insert another list at a specific index in this list.
	/// </summary>
	/// <param name="index">The index to insert the list.</param>
	/// <param name="values">The list.</param>
	void Insert(unsigned int index, const List<T>& values)
	{
		if (values.Size() == 0)
			return;
		else
			//Iterate through the values and insert them into our list
			for (unsigned int i = 0; i < values.Size(); ++i)
				Insert(index + i, values[i]);
	}

	/// <summary>
	/// Pop the element off the end of the list.
	/// </summary>
	void Pop()
	{
		if (size > 0)
			ReduceSize(1);
	}

	/// <summary>
	/// Remove the value at a specific index from the list.
	/// This will remove any order from the list.
	/// To preserve order, RemoveKeepOrder() should be used.
	/// </summary>
	/// <param name="index">The index of the value to be removed.</param>
	void Remove(unsigned int index)
	{
		if (size > 0 && index >= 0 && index < size)
		{
			if (size == 1 || index == (size - 1))
				Pop();
			else
			{
				data[index] = data[size - 1];
				ReduceSize(1);
			}
		}
	}
	
	/// <summary>
	/// Remove all occurences of a value from the list.
	/// This will remove any orderfrom the list.
	/// To preserve order, RemoveKeepOrder() should be used.
	/// </summary>
	/// <param name="value">The value to remove from the array.</param>
	void Remove(T& value)
	{
		for (unsigned int i = 0; i < size; ++i)
			if (data[i] == value)
			{
				Remove(i);
				--i;
			}
	}

	/// <summary>
	/// Remove the value at a specific index from the list.
	/// This will keep any order in the list.
	/// </summary>
	/// <param name="index">The index of the value to be removed.</param>
	void RemoveKeepOrder(unsigned int index)
	{
		if (size == 0)
			return;
		else if (index == (size - 1) || size == 1)
			Pop();
		else
		{
			//Move each value past the given index back by one position
			for (unsigned int i = index; i < size - 1; ++i)
				data[i] = data[i + 1];
			
			//Reduce the size
			ReduceSize(1);
		}
	}

	/// <summary>
	/// Remove all occurences of a specific value from the list.
	/// This will keep any order in the list.
	/// </summary>
	/// <param name="value">Value to remove.</param>
	void RemoveKeepOrder(T& value)
	{
		for (unsigned int i = 0; i < size; ++i)
			if (data[i] == value)
			{
				RemoveKeepOrder(i);
				--i;
			}
	}

	/// <summary>
	/// Clear the list.
	/// </summary>
	void Clear()
	{
		Discard(capacity);
	}

	/// <summary>
	/// Perform a recursive quick sort of the list.
	/// </summary>
	void QuickSort()
	{
		QuickSort(0, size - 1);
	}

	/// <summary>
	/// Sort the list using an optimised cocktail shaker sort algorithm.
	/// </summary>
	void CocktailShakerSort()
	{
		if (size < 2)
			return;

		int passes = 0;	//Keep track of the number of times the list has been traversed to reduce the next traversal
		bool sorted = false;
		
		//Sort the list by traversing it and swapping values
		while (!sorted)
		{
			sorted = true;

			//Traverse forwards through the list
			for (unsigned int i = 0; i < size - (passes + 1); ++i)
			{
				if (data[i] > data[i + 1])
				{
					Swap(&data[i], &data[i + 1]);
					sorted = false;
				}
			}

			//Traverse backwards through the list
			for (unsigned int i = size - 1; i > 0; --i)
			{
				if (data[i - 1] > data[i])
				{
					Swap(&data[i], &data[i - 1]);
					sorted = false;
				}
			}

			++passes;
		}
	}

	/// <summary>
	/// Perform an insertion sort on the list.
	/// https://www.geeksforgeeks.org/insertion-sort/
	/// </summary>
	void InsertionSort()
	{
		T key;
		int j;
		for (unsigned int i = 1; i < size; ++i)
		{
			key = data[i];
			j = i - 1;

			while (j >= 0 && data[j] > key)
			{
				data[j + 1] = data[j];
				j = j - 1;
			}
			data[j + 1] = key;
		}
	}

	/// <summary>
	/// Perform a heap sort on the list.
	/// https://www.geeksforgeeks.org/heap-sort/
	/// </summary>
	void HeapSort()
	{
		//Build a heap (rearrange array)
		for (int i = size / 2 - 1; i >= 0; --i)
			Heapify(size, i);

		//One by one extract an element from the heap
		for (int i = size - 1; i >= 0; --i)
		{
			//Move current root to end
			Swap(&data[0], &data[i]);

			//Call max heapify on the reduced heap
			Heapify(i, 0);
		}
	}

	/// <summary>
	/// Return the index of the value if present.
	/// https://www.geeksforgeeks.org/fibonacci-search/
	/// </summary>
	/// <param name="value">Value to search for.</param>
	/// <returns>Index of the value, -1 if not found.</returns>
	int FibonacciSearch(const T& value) const
	{
		int fibMMm2 = 0;				//(m-2)'th Fibonnaci number
		int fibMMm1 = 1;				//(m-1)'th Fibonnaci number
		int fibM = fibMMm1 + fibMMm2;	// m'th Fibonacci number

		//Store the smallest Fibonacci number greater than or equal to the size of the list in fibM
		while (fibM < (int)size)
		{
			fibMMm2 = fibMMm1;
			fibMMm1 = fibM;
			fibM = fibMMm1 + fibMMm2;
		}

		//Marks the eliminated range from the front
		int offset = -1;

		//While there are elements to be searched
		while (fibM > 1)
		{
			int i = Min(offset + fibMMm2, size - 1);

			//If the value is greater than the value at index fibMMm2, cut the list from offset to i
			if (data[i] < value)
			{
				fibM = fibMMm1;
				fibMMm1 = fibMMm2;
				fibMMm2 = fibM - fibMMm1;
				offset = i;
			}
			//If the value is greater than the value at index fibMMm2, cut the list after i + 1
			else if (data[i] > value)
			{
				fibM = fibMMm2;
				fibMMm1 = fibMMm1 - fibMMm2;
				fibMMm2 = fibM - fibMMm1;
			}
			//Value was found
			else
				return i;
		}

		//Compare the last element
		if (fibMMm1 && data[offset + 1] == value) 
			return offset + 1;

		//Value not found
		return -1;
	}

	/// <summary>
	/// Perform a binary search for a value in the list.
	/// </summary>
	/// <param name="value">The value to search for.</param>
	/// <returns>The index of the value in the list, or -1 if not found.</returns>
	int BinarySearch(const T& value) const
	{
		if (size == 0)
			return -1;

		unsigned int startIndex = 0;
		unsigned int endIndex = size - 1;

		while (startIndex <= endIndex)
		{
			unsigned int pivot = (startIndex + endIndex) / 2;

			if (data[pivot] == value)
				return pivot;

			if (value < data[pivot])
				endIndex = pivot - 1;
			else
				startIndex = pivot + 1;
		}

		return -1;
	}

	/// <summary>
	/// Perform a jump search on the list to find a value.
	/// https://www.geeksforgeeks.org/jump-search/
	/// </summary>
	/// <param name="value">The value to search for.</param>
	/// <returns>The index of the value in the list, or -1 if not found.</returns>
	int JumpSearch(const T& value) const
	{
		if (size == 0)
			return -1;

		//Find a block size to be jumped
		int step = (int)sqrt(size);

		//Find the block where the search value is present
		int prev = 0;
		while (data[Min(step, size) - 1] < value)
		{
			prev = step;
			step += (int)sqrt(size);
			if (prev >= (int)size)
				return -1;
		}

		//Perform a linear search in the block beginning with prev
		while (data[prev] < value)
		{
			++prev;

			//If we reached the next block of the end of the array, then the value is not present
			if (prev == Min(step, size))
				return -1;
		}

		//Check if the value is found
		if (data[prev] == value)
			return prev;

		return -1;
	}

	/// <summary>
	/// Getter for the size of the list.
	/// </summary>
	/// <returns>The number of elements in the list.</returns>
	unsigned int Size() const
	{
		return size;
	}

	/// <summary>
	/// Getter for the current capacity of the list.
	/// </summary>
	/// <returns>The current capacity of the list.</returns>
	unsigned int Capacity() const
	{
		return capacity;
	}

	/// <summary>
	/// Getter for the maximum possible capacity for any list.
	/// </summary>
	/// <returns>The maximum possible capacity for any list.</returns>
	const unsigned int MaxCapacity() const
	{
		return MAX_CAPACITY;
	}

	/// <summary>
	/// Assignment operator overload.
	/// </summary>
	/// <param name="other">The other list to copy data from.</param>
	/// <returns>This list with copied data.</returns>
	List& operator= (const List& other)
	{
		delete[] data;
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];
		memcpy(data, other.data, sizeof(T) * capacity);
		return *this;
	}

	/// <summary>
	/// << operator overload.
	/// Allows displaying the list to an ostream.
	/// </summary>
	/// <param name="os">The ostream to display the list to.</param>
	/// <param name="list">The list to display,</param>
	/// <returns>The ostream with the list displayed.</returns>
	friend ostream& operator<< (ostream& os, const List<T>& list)
	{
		os << "[";
		for (unsigned int i = 0; i < list.Size(); ++i)
		{
			if (i != 0)
				os << ", ";
			os << list[i];
		}
		os << "]";
		return os;
	}

	/// <summary>
	/// [] sub-script operator overload.
	/// Allow accessing the list using square brackets.
	/// </summary>
	/// <param name="index">The index to access.</param>
	/// <returns>The element at the specified index.</returns>
	T& operator[] (const unsigned int index) const
	{
		if (index >= 0 && index < size)
			return data[index];

		//Throw an error if the index is outside the range of the list
		throw out_of_range("Index out of range.");
	}

	/// <summary>
	/// Print details about this list to std::cout.
	/// </summary>
	void PrintDetails() const
	{
		cout << "Size: " << size << "   ";
		cout << "Capacity: " << capacity << "   ";
		for (unsigned int i = 0; i < size; ++i)
			cout << data[i] << " ";
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