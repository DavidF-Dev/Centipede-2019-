/*
	File: BinaryTree.h
	Contains: BinaryTree, BinaryTreeNode
*/

#pragma once
#include <iostream>
#include <queue>	//Change to custom implementation

using namespace std;

/// <summary>
/// The Binary Tree Node class represents a node in the binary tree.
/// Each node has a piece of data attached and a pointer to the left & right child.
/// </summary>
template <typename T>
class BinaryTreeNode
{
public:
	T data;						//The data attached to this node
	BinaryTreeNode<T>* left;	//Pointer to the left child
	BinaryTreeNode<T>* right;	//Pointer to the right child

	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="_data">The data to attach to this node.</param>
	BinaryTreeNode(const T& _data)
	{
		data = _data;
		left = nullptr;
		right = nullptr;
	}

	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="_data">The data to attach to this node.</param>
	/// <param name="_left">A pointer to the left child.</param>
	/// <param name="_right">A pointer to the right child.</param>
	BinaryTreeNode(const T& _data, BinaryTreeNode<T>* _left, BinaryTreeNode<T>* _right)
	{
		data = _data;
		left = _left;
		right = _right;
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~BinaryTreeNode() {}

	/// <summary>
	/// Get a copy of this node and its children.
	/// Uses recursion.
	/// https://stackoverflow.com/questions/16098362/how-to-deep-copy-a-binary-tree
	/// </summary>
	/// <returns>A deep copy of this node and its children.</returns>
	BinaryTreeNode<T>* Copy()
	{
		return new BinaryTreeNode<T>(data, 
			left != nullptr ? left->Copy() : nullptr,		//Run on left child if it isn't a nullptr
			right != nullptr ? right->Copy() : nullptr);	//Run on right child if it isn't a nullptr
	}
};

//Enum for choosing how to use the depth search on the binary tree
enum DEPTH_FIRST_SEARCH_TYPE { SEARCH_PRE_ORDER, SEARCH_POST_ORDER, SEARCH_IN_ORDER };

/// <summary>
/// The Binary Tree class has a root node and keeps track of the number of nodes.
/// </summary>
template <typename T>
class BinaryTree
{
private:
	typedef void(*ProcessFnType)(BinaryTreeNode<T>* node);	//Function type definition for use in the search functions
	BinaryTreeNode<T>* root;	//The root node of the tree
	unsigned int size;			//The number of nodes in the tree

public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	BinaryTree()
	{
		root = nullptr;
		size = 0;
	}

	/// <summary>
	/// Copy constructor.
	/// </summary>
	/// <param name="copy">The tree to copy.</param>
	BinaryTree(const BinaryTree& copy)
	{
		size = copy.size;
		if (!Empty())
			root = copy.root->Copy();
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~BinaryTree()
	{
		Clear();
	}

	/// <summary>
	/// Insert a new node into the tree in the correct position.
	/// If the node already exists, no changes will be made.
	/// </summary>
	/// <param name="data">The data to add to the tree.</param>
	void Insert(const T& data)
	{
		if (Empty())	//If the tree is empty then set up the root
		{
			root = new BinaryTreeNode<T>(data);
			size = 1;
		}
		else
		{
			BinaryTreeNode<T>* node = root;			//Keep track of the current node and its parent
			BinaryTreeNode<T>* parent = nullptr;
			while (node != nullptr)
			{
				parent = node;
				if (data < node->data)				//If the data is less than the data of this node, then traverse to the left child
					node = node->left;
				else if (data > node->data)			//If the data is greater than the data of this node, then traverse to the right child
					node = node->right;
				else								//If the data already exists, exit the function
					return;
			}

			//Attach a new node as a leaf to the parent
			if (data < parent->data)
				parent->left = new BinaryTreeNode<T>(data);
			else
				parent->right = new BinaryTreeNode<T>(data);
			++size;
		}
	}

	/// <summary>
	/// Remove a node from the tree.
	/// </summary>
	/// <param name="data">The data to remove from the tree.</param>
	void Remove(const T& data)
	{
		BinaryTreeNode<T>** ppNode = new BinaryTreeNode<T>*;
		BinaryTreeNode<T>** ppParent = new BinaryTreeNode<T>*;
		if (Find(data, ppNode, ppParent))	//Try and find the node with the value to be removed
		{
			if ((*ppNode)->right != nullptr)	//Check if the current node has a right branch
			{
				//Find the minimum value in the right branch by iterating down the left branch of the current node's
				//right child until there are no more left branch nodes
				BinaryTreeNode<T>* minimumNode = (*ppNode)->right;
				BinaryTreeNode<T>* minimumParent = (*ppNode);
				while (minimumNode->left != nullptr)
				{
					minimumParent = minimumNode;
					minimumNode = minimumNode->left;
				}
				
				//Copy the value from the minimum node to the current node
				(*ppNode)->data = minimumNode->data;

				//Delete the minimum node
				if (minimumParent->left == minimumNode)
					minimumParent->left = minimumNode->right;
				if (minimumParent->right == minimumNode)
					minimumParent->right = minimumNode->right;
				delete minimumNode;
			}
			else	//If the current node has no right branch
			{
				//Delete the current node
				if ((*ppNode) == root)
					root = (*ppNode)->left;
				else if ((*ppParent)->left == (*ppNode))
					(*ppParent)->left = (*ppNode)->left;
				else if ((*ppParent)->right == (*ppNode))
					(*ppParent)->right = (*ppNode)->left;
				delete (*ppNode);
			}
			--size;	//Decrease the size
		}
		delete ppNode;
		delete ppParent;
	}

	/// <summary>
	/// Get a Tree Node that contains the given data value.
	/// Searches through the tree.
	/// </summary>
	/// <param name="data">The data to search for.</param>
	/// <returns>The node with the data.</returns>
	BinaryTreeNode<T>* Find(const T& data) const
	{
		BinaryTreeNode<T>** ppNode = new BinaryTreeNode<T>*;
		BinaryTreeNode<T>** ppParent = new BinaryTreeNode<T>*;
		bool found = Find(data, ppNode, ppParent);		//Try to find the node
		BinaryTreeNode<T>* foundNode = found ? (*ppNode) : nullptr;	//Store the pointer in a different pointer so we can delete below
		delete ppNode;
		delete ppParent;
		return foundNode;		//Return the node if it was found
	}

	/// <summary>
	/// The function will return true if a node with the given data is present in the tree.
	/// The function will also use pointers to return additional information.
	/// </summary>
	/// <param name="data">The data to search for.</param>
	/// <param name="ppNode">The node that the data is found at.</param>
	/// <param name="ppParent">The parent of the node that the data is found at.</param>
	/// <returns>True if the node is found in the tree that matches the given data.</returns>
	bool Find(const T& data, BinaryTreeNode<T>** ppNode, BinaryTreeNode<T>** ppParent) const
	{
		*ppNode = root;					//Start at the root
		while ((*ppNode) != nullptr)	//Loop until we reach a nullptr node
		{
			if (data == (*ppNode)->data)	//If the data matches the node data, then return true
				return true;
			else
			{
				*ppParent = *ppNode;	//Update the parent node before changing the node itself
				//Traverse to the left or right node depending on the relationship between the data and the data in the current node
				*ppNode = (data < (*ppNode)->data) ? (*ppNode)->left : (*ppNode)->right;
			}
		}
		return false;	//If no node is found, then return false
	}

	/// <summary>
	/// Empties the tree.
	/// </summary>
	void Clear()
	{
		//Loop until the tree is empty
		while (!Empty())
			Remove(root->data);
	}

	/// <summary>
	/// Iterate through the tree and perform a function on each node.
	/// </summary>
	/// <param name="searchType">The way to traverse through the tree. (PRE_ORDER, POST_ORDER, IN_ORDER)</param>
	/// <param name="ProcessFn">A function pointer that will process any particular node in the tree.</param>
	void DepthFirstSearch(DEPTH_FIRST_SEARCH_TYPE searchType, ProcessFnType ProcessFn)
	{
		if (!Empty())
		{
			//Run the appropriate function depending on the search type
			if (searchType == SEARCH_PRE_ORDER)
				DepthFirstPreOrderSearch(root, ProcessFn);
			else if (searchType == SEARCH_POST_ORDER)
				DepthFirstPostOrderSearch(root, ProcessFn);
			else
				DepthFirstInOrderSearch(root, ProcessFn);
		}
	}

	/// <summary>
	/// Performs a Breadth First traversal of the tree and processes each node with the given function.
	/// </summary>
	/// <param name="ProcessFn">A function pointer that will process any particular node in the tree.</param>
	void BreadthFirstSearch(ProcessFnType ProcessFn)
	{
		if (!Empty())
		{
			queue<BinaryTreeNode<T>*> list;		//Create a queue to contain which node to process next
			list.push(root);					//Push the root as it will be processed first
			while (!list.empty())				//Keep looping until the queue is empty i.e. all nodes have been processed
			{
				BinaryTreeNode<T>* node = list.front();		//Grab the node at the front of the queue
				ProcessFn(node);							//Process the node
				list.pop();									//Pop the node off the queue
				if (node->left != nullptr)					//If a left child exists, push it to the end of the queue
					list.push(node->left);
				if (node->right != nullptr)					//If a right child exists, push it to the end of the queue
					list.push(node->right);
			}
		}
	}

	/// <summary>
	/// Getter for the root of the tree.
	/// </summary>
	/// <returns>The root node of the tree.</returns>
	BinaryTreeNode<T>* GetRoot() const
	{
		return root;
	}

	/// <summary>
	/// Check if the tree is empty.
	/// </summary>
	/// <returns>True if the tree is empty.</returns>
	bool Empty() const
	{
		return size == 0;
	}

	/// <summary>
	/// Getter for the size of the tree.
	/// </summary>
	/// <returns>The number of nodes in the tree.</returns>
	unsigned int Size()
	{
		return size;
	}

	/// <summary>
	/// Getter for the number of edges in the tree.
	/// This is (the number of nodes) - 1.
	/// </summary>
	/// <returns>The number of edges in the tree.</returns>
	unsigned int Edges() const
	{
		if (Empty())
			return 0;
		else
			return size - 1;
	}

	/// <summary>
	/// Assignment operator overload.
	/// Performs a deep copy.
	/// </summary>
	/// <param name="other">The tree to copy to this tree.</param>
	/// <returns>This tree with the same data as the given tree.</returns>
	BinaryTree<T>& operator= (const BinaryTree<T>& other)
	{
		Clear();
		size = other.size;
		if (!Empty())
			root = other.root->Copy();
		return *this;
	}

	/// <summary>
	/// Recursively prints the tree to an ostream.
	/// https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
	/// </summary>
	/// <param name="os">The ostream the print the tree to.</param>
	/// <param name="node">The current node to process. (Initially root)</param>
	/// <param name="space">The space between the levels. (Initially 0)</param>
	friend void PrintTreeF(ostream& os, BinaryTreeNode<T>* node, int space)
	{
		if (node == nullptr)
			return;

		//Increase the distance between the levels
		space += 5;

		//Process the right child
		PrintTreeF(os, node->right, space);

		//Print the current node
		os << endl;
		for (int i = 5; i < space; ++i)
			os << " ";
		os << node->data << endl;

		//Process the left child
		PrintTreeF(os, node->left, space);
	}

	/// <summary>
	/// Overloaded << operator.
	/// Displays the tree to an ostream.
	/// </summary>
	/// <param name="os">The ostream to print the tree to.</param>
	/// <param name="tree">The tree to print.</param>
	/// <returns>The ostream with the tree printed to it.</returns>
	friend ostream& operator<< (ostream& os, const BinaryTree<T>& tree)
	{
		//Call the other friend function to recursively print the tree
		PrintTreeF(os, tree.GetRoot(), 0);
		return os;
	}

	/// <summary>
	/// Print details about the tree.
	/// </summary>
	void PrintDetails() const
	{
		cout << "Size: " << size << "   Edges: " << Edges() << endl;
		if (!Empty())
		{
			//Call a function to recursively print the tree to std::cout
			PrintTree(root, 0);
		}
		else
			cout << "Empty" << endl;
		cout << endl;
	}

	/// <summary>
	/// Get the tree represented as a string.
	/// https://stackoverflow.com/questions/3513173/converting-ostream-into-standard-string
	/// http://www.cplusplus.com/reference/sstream/stringstream/str/
	/// </summary>
	/// <returns>A string representation of the tree.</returns>
	string ToString() const
	{
		ostringstream stream;
		stream << *this;
		return stream.str();
	}

private:
	/// <summary>
	/// Traverse the tree using the depth first pre order search.
	/// - Process node
	/// - Loop through children of node
	///   - Call pre order function on each child
	/// </summary>
	/// <param name="node">The current node of the recursive function.</param>
	/// <param name="ProcessFn">The function to pointer to call on each node.</param>
	void DepthFirstPreOrderSearch(BinaryTreeNode<T>* node, ProcessFnType ProcessFn)
	{
		ProcessFn(node);
		if (node->left != nullptr)
			DepthFirstPreOrderSearch(node->left, ProcessFn);
		if (node->right != nullptr)
			DepthFirstPreOrderSearch(node->right, ProcessFn);
	}

	/// <summary>
	/// Traverse the tree using the depth first post order search.
	/// - Loop through children of node
	///   - Call post order function on each child
	/// - Process node
	/// </summary>
	/// <param name="node">The current node of the recursive function.</param>
	/// <param name="ProcessFn">The function to pointer to call on each node.</param>
	void DepthFirstPostOrderSearch(BinaryTreeNode<T>* node, ProcessFnType ProcessFn)
	{
		if (node->left != nullptr)
			DepthFirstPostOrderSearch(node->left, ProcessFn);
		if (node->right != nullptr)
			DepthFirstPostOrderSearch(node->right, ProcessFn);
		ProcessFn(node);
	}

	/// <summary>
	/// Traverse the tree using the depth first in order search.
	/// - Call in order function on first child
	/// - Process node
	/// - Call in order function on second child
	/// </summary>
	/// <param name="node">The current node of the recursive function.</param>
	/// <param name="ProcessFn">The function to pointer to call on each node.</param>
	void DepthFirstInOrderSearch(BinaryTreeNode<T>* node, ProcessFnType ProcessFn)
	{
		if (node->left != nullptr)
			DepthFirstInOrderSearch(node->left, ProcessFn);
		ProcessFn(node);
		if (node->right != nullptr)
			DepthFirstInOrderSearch(node->right, ProcessFn);
	}

	/// <summary>
	/// Recursively prints the tree to std::cout
	/// https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
	/// </summary>
	/// <param name="node">The current node to process. (Initially root)</param>
	/// <param name="space">The space between the levels. (Initially 0)</param>
	void PrintTree(BinaryTreeNode<T>* node, int space) const
	{
		if (node == nullptr)
			return;
		
		//Increase the distance between the levels
		space += 5;

		//Process the right child
		PrintTree(node->right, space);

		//Print the current node
		cout << endl;
		for (int i = 5; i < space; ++i)
			cout << " ";
		cout << node->data << endl;

		//Process the left child
		PrintTree(node->left, space);
	}
};