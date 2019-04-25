#define _USE_MATH_DEFINES
/*
*   main.cpp
*  
*   k-d Tree Project
*	author: Arvind Rao
*   license: GNU 
*
*/  

#include "kdtree.h"
#include <iostream>
#include <random>
#include <cmath>
#include <array>


const int _numpts = 10;
const int _dim = 3;

glm::vec4 testpoints[_numpts] =
	{
		glm::vec4(38.40924, 4.11543, 8.10499,1),
		glm::vec4(6.50689, 1.3663, 3.43026,1),
		glm::vec4(9.7614, 9.8382, 0.672512,1),
		glm::vec4(0.113181, 2.22785, 3.46726,1),
		glm::vec4(5.23381, 4.69416, 4.74723,1),
		glm::vec4(9.74655, 0.191659, 1.2064,1),
		glm::vec4(2.8546, 7.32662, 8.51895,1),
		glm::vec4(6.21829, 0.779546, 1.82988,1),
		glm::vec4(8.83612, 8.70544, 2.40537,1),
		glm::vec4(6.50697, 2.70078, 1.93852,1)
	};


void printVector(glm::vec4 pt)
{
	
	    std::cout<< pt.x << ", "<< pt.y << ", "<< pt.z<<"\n";
	
}

int main(int argc, char ** argv)
{ 
	//make a list of vectors out of the testpoints array
	std::list<glm::vec4> point_list;
	for(auto i = 0; i < _numpts; i++ )
	{
	
		point_list.push_back(testpoints[i]);
	}
	
	Kdtree kd;
	kd.makeTree(point_list);
	kd.printTree(kd.getRoot());
 	
	std::cout<<"\n\n";
	getchar();
}