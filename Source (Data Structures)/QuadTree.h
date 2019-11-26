/*
	File: QuadTree.h
	Contains: AABB, QuadTree
*/

#pragma once
#include "CentipedeGameApp.h"
#include "Sprite.h"
#include <glm\vec2.hpp>

using namespace glm;

/// <summary>
/// AABB represents a square with a position and size.
/// </summary>
class AABB
{
public:
	vec2 center;	//Center of the square
	vec2 halfSize;	//Half of the size of the square

	/// <summary>
	/// Default constructor.
	/// </summary>
	AABB() {}
	
	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="_center">The center of the square.</param>
	/// <param name="_halfSize">Half the size of the square.</param>
	AABB(vec2 _center, vec2 _halfSize) 
	{
		center = _center;
		halfSize = _halfSize;
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~AABB() {}
	
	/// /// <summary>
	/// Check if this square contains the given point.
	/// </summary>
	/// <param name="point">The point to check.</param>
	/// <returns>True, if the square contains the point.</returns>
	bool Contains(vec2 point)
	{
		return (point.x < center.x + halfSize.x &&
			point.x > center.x - halfSize.x &&
			point.y < center.y + halfSize.y &&
			point.y > center.y - halfSize.y);
	}

	/// /// <summary>
	/// Check if this square contains the given point.
	/// </summary>
	/// <param name="point">The point to check.</param>
	/// <returns>True, if the square contains the point.</returns>
	bool Contains(Point2D point)
	{
		return (point.x < center.x + halfSize.x &&
			point.x > center.x - halfSize.x &&
			point.y < center.y + halfSize.y &&
			point.y > center.y - halfSize.y);
	}
};

/// <summary>
/// The Quad Tree class used to keep track of objects in space.
/// </summary>
class QuadTree
{
private:
	QuadTree** children;	//Pointer to a pointer to the children nodes
	AABB boundary;			//The box of this node
	Sprite** objects;		//Pointer to a pointer to the objects in this node

public:
	static const int CAPACITY = 2;	//The number of objects per node

	enum SUBTREE { TOP_LEFT = 0, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

	/// <summary>
	/// Default constructor.
	/// </summary>
	QuadTree()
	{
		children = nullptr;
		objects = nullptr;
		boundary.halfSize.x = (float)CentipedeGameApp::instance->getWindowWidth() / 2;
		boundary.halfSize.y = (float)CentipedeGameApp::instance->getWindowHeight() / 2;
		boundary.center = boundary.halfSize;
	}

	/// <summary>
	/// Overloaded constructor.
	/// </summary>
	/// <param name="_boundary">The initial bounday of the quad tree.</param>
	QuadTree(AABB _boundary)
	{
		children = nullptr;
		objects = nullptr;
		boundary = _boundary;
	}

	/// <summary>
	/// Deconstructor.
	/// </summary>
	~QuadTree()
	{
		//Delete the children recursively
		if (children != nullptr)
		{
			for (int i = 0; i < 4; ++i)
				if (children[i] != nullptr)
					delete children[i];
			delete[] children;
			children = nullptr;
		}

		//Delete the pointers to the objects, but not the objects themselves
		if (objects != nullptr)
			delete[] objects;
	}

	/// <summary>
	/// Insert an object into the quad tree.
	/// </summary>
	/// <param name="sprite">The object.</param>
	/// <returns>True, if it was inserted successfully.</returns>
	bool Insert(Sprite* sprite)
	{
		//If the sprite doesn't fit in the boundary, return false
		if (!boundary.Contains(sprite->position))
			return false;

		//If this node doesn't have any children, then try to insert the object in this node
		if (children == nullptr)
		{
			//Create the objects array if it doesn't exist
			if (objects == nullptr)
			{
				objects = new Sprite*[CAPACITY];
				memset(objects, 0, sizeof(Sprite*) * CAPACITY);
			}

			//Check if there is a free space in the objects array
			if (objects[CAPACITY - 1] == nullptr)
			{
				//We have at least 1 free space
				//Iterate and find a place to insert
				for (int i = 0; i < CAPACITY; ++i)
					if (objects[i] == nullptr)
					{
						objects[i] = sprite;
						return true;
					}
			}

			//If we can't insert then subdivide this node
			SubDivide();
		}

		//If we can't insert into this node then try to find a child node to insert into
		for (int i = 0; i < 4; ++i)
			if (children[i]->Insert(sprite))
				return true;

		return false;
	}

	/// <summary>
	/// Subdivide this node into 4 new sub-trees.
	/// Calculate the new center position and dimensions of each sub-tree by
	/// splitting the current node's boundary into four.
	/// Then remove all the object's from this node and insert into the sub-trees.
	/// </summary>
	void SubDivide()
	{
		//Create 4 new sub-trees
		children = new QuadTree*[4];

		//Calculate the size and the center for each sub-tree
		vec2 qSize = vec2(boundary.halfSize.x / 2, boundary.halfSize.y / 2);

		vec2 qCenter = vec2(boundary.center.x - qSize.x, boundary.center.y - qSize.y);
		children[TOP_LEFT] = new QuadTree(AABB(qCenter, qSize));

		qCenter = vec2(boundary.center.x + qSize.x, boundary.center.y - qSize.y);
		children[TOP_RIGHT] = new QuadTree(AABB(qCenter, qSize));

		qCenter = vec2(boundary.center.x - qSize.x, boundary.center.y + qSize.y);
		children[BOTTOM_LEFT] = new QuadTree(AABB(qCenter, qSize));

		qCenter = vec2(boundary.center.x + qSize.x, boundary.center.y + qSize.y);
		children[BOTTOM_RIGHT] = new QuadTree(AABB(qCenter, qSize));

		//Insert the objects into the correct sub-tree
		if (objects != nullptr)
		{
			for (int i = 0; i < CAPACITY; ++i)
			{
				if (objects[i] == nullptr)
					continue;

				for (int j = 0; j < 4; ++j)
					if (children[j]->Insert(objects[i]))
						break;

				objects[i] = nullptr;
			}

			//Delete the objects from this sub-tree
			delete objects;
			objects = nullptr;
		}
	}

	/// <summary>
	/// Clear the sub-tree by creating a new one
	/// </summary>
	void Clear()
	{
		*this = QuadTree();
	}

	/// <summary>
	/// Draw the quad-tree lines, showing the nodes visually.
	/// </summary>
	/// <param name="renderer">Pointer to the renderer.</param>
	void Draw(Renderer2D* renderer)
	{
		renderer->setRenderColour(1, 0, 0);
		renderer->drawLine
		(
			boundary.center.x - boundary.halfSize.x,
			boundary.center.y - boundary.halfSize.y,
			boundary.center.x + boundary.halfSize.x,
			boundary.center.y - boundary.halfSize.y
		);
		renderer->drawLine
		(
			boundary.center.x - boundary.halfSize.x,
			boundary.center.y + boundary.halfSize.y - 1,
			boundary.center.x + boundary.halfSize.x,
			boundary.center.y + boundary.halfSize.y - 1
		);
		renderer->drawLine
		(
			boundary.center.x - boundary.halfSize.x + 1,
			boundary.center.y + boundary.halfSize.y,
			boundary.center.x - boundary.halfSize.x + 1,
			boundary.center.y - boundary.halfSize.y
		);
		renderer->drawLine
		(
			boundary.center.x + boundary.halfSize.x,
			boundary.center.y + boundary.halfSize.y,
			boundary.center.x + boundary.halfSize.x,
			boundary.center.y - boundary.halfSize.y
		);

		//Draw the sub-trees
		if (children != nullptr)
			for (int i = 0; i < 4; ++i)
				children[i]->Draw(renderer);
	}
};