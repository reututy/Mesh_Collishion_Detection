/**
*
* k-d tree implementation file
*	
* author: Arvind Rao	
* license: GNU
* 
*
*/
#include "kdtree.h"
#include <iostream>

// implementation of Node Class

Node::Node(int _n) { left=0; right=0;}
Node::Node(int _n, glm::vec4 _data) : data(_data){left = 0; right = 0;}

Node::~Node(){}


// implementation of kdtree

Kdtree::Kdtree(){}
Kdtree::~Kdtree(){}

/*
*
*
*/
glm::vec4 Kdtree::findMedian(int axis, std::list<glm::vec4> &plist, std::list<glm::vec4> &left,
							std::list<glm::vec4> &right )
{
	glm::vec4 median;
	int size = plist.size();
	int med = (int)ceil( float(size) / float(2) );
	int count = 0;
	
	if ( size == 1)
		return plist.front();
	
	// Using lambda function here, to define comparison function--parametrized by 'axis'
	plist.sort( [&](glm::vec4& a, glm::vec4& b){return a[axis] < b[axis];});
	
	for ( auto& x : plist )
	{
		if (count < med)
			left.push_back(x);
		else
			right.push_back(x);
		++count;
	}
	median = left.back();
	left.pop_back();
	return median;
}

void Kdtree::print_data(glm::vec4 pt)
{
	for( int i = 0; i < N; i++)
	{
	    std::cout<< pt[i] << ", ";
	}
	std::cout<<"\n";
}

/*
*
*
*/
void Kdtree::printTree( Node* head )
{
	//find the tree depth 
	int maxdepth = 3;
	int spaces = (int)pow(2, maxdepth +1 ) - 1;
	int depth = 0;
	
	std::cout<< "\n**** Print of Tree **********\n";
	std::queue< Node* > current, next; 
	Node * temp = head;
	current.push(temp);

	while( !current.empty() )
	{
		temp = current.front();
		current.pop();

		if (temp == nullptr)
			std::cout<<"NULL\n";
		else
		{
			Kdtree::print_data(temp->data);
			next.push(temp->left);
			next.push(temp->right);
		}
		if(current.empty())
		{
			depth++;
			std::cout<< "level: "<<  depth <<"\n";
			std::swap(current, next);
		}
	}
}

/*
*  algorithm is based on http://en.wikipedia.org/wiki/Kd_tree
*/
void Kdtree::makeTree(std::list<glm::vec4>& plist)
{
	Node* head = new Node(3);
	Kdtree::_makeTree( head, plist, 0 );
	Kdtree::root = head;
	Kdtree::FindDepth(head);
}

void Kdtree::_makeTree(Node* head, std::list<glm::vec4>& plist, int depth)
{	
	if( !plist.empty() ) 
	{
		int k = N;
		int axis = depth % k;
		
		std::list<glm::vec4> left_list;
		std::list<glm::vec4> right_list;
		glm::vec4 median = Kdtree::findMedian(axis, plist, left_list, right_list);
		head->data = median;
		
		Node* left_node = new Node(k);
		Node* right_node = new Node(k);
		
		Kdtree::_makeTree(left_node, left_list, depth+1);
		if (!left_list.empty()) head->left = left_node;
		
		Kdtree::_makeTree(right_node, right_list, depth+1);
		if (!right_list.empty()) head->right = right_node;
	}
} 

void Kdtree::FindDepth(Node* head)
{
	//find the tree depth 
	int maxdepth = 3;
	int spaces = (int)pow(2, maxdepth + 1) - 1;
	depth = 0;

	//std::cout << "\n**** Print of Tree **********\n";
	std::queue< Node* > current, next;
	Node * temp = head;
	current.push(temp);

	while (!current.empty())
	{
		temp = current.front();
		current.pop();

		if (temp != nullptr)
		{
			//Kdtree::print_data(temp->data);
			next.push(temp->left);
			next.push(temp->right);
		}
		if (current.empty())
		{
			depth++;
			//std::cout << "level: " << depth << "\n";
			std::swap(current, next);
		}
	}
}

int Kdtree::GetDepth()
{
	return depth;
}