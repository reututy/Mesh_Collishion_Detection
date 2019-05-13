/*
*
* k-d tree declaration file
*	
* author: Arvind Rao	
* license: GNU
* 
*
*/

#ifndef KDTREE_
#define KDTREE_
#define _USE_MATH_DEFINES

#include <memory>
#include <vector>
#include <list>
#include <cmath>
#include <queue>
#include <utility>
#include "glm/glm.hpp"

const int N = 3;

class Node
{
 public:
	 Node* left; // = std::unique_ptr<Node>( new Node(3) );
	 Node* right; //( new Node(3));
	 glm::vec4 data;
	 
     //default constructor
	 Node(int _n);
	 
	 //copy constructor
	 Node(int _n, glm::vec4 _data);
	 
	 //default deconstructor
	 ~Node();
};

class Kdtree
{
private:
	//helper for makeTree 
	void _makeTree(Node* head, std::list<glm::vec4> &plist, int depth);

	Node* root;
	int depth;

 public:
	//default constructor
	Kdtree();

	//default deconstructor
	~Kdtree();
	
	/* 
	*   Return the tree root node
	*/
	Node* getRoot() const { return root; };
	/*
	* support function for printTree
	*/
	void print_data(glm::vec4 pt);
		
	/*  prints the tree
	*   and really works best for small trees 
	*   as a test of tree construction.
	*/  
	void printTree(Node* head);
	
	//function for finding the median of a list of points
	glm::vec4 findMedian(int axis, std::list<glm::vec4> &plist, std::list<glm::vec4> &left,
								std::list<glm::vec4> &right );
	//function for making the tree
	void makeTree(std::list<glm::vec4> &plist);

	void FindDepth(Node* head);

	int GetDepth();

};

#endif