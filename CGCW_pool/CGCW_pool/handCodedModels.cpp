
#include <GLFW/glfw3.h>
#include "handCodedModels.h"

// Specify Vertices for the wall in x-y plane 
GLfloat wallVertices[] = {	// texture
	//pos					tex				normal
	 0.f,  0.f, 0.f,	0.f, 0.f,		0.f, 0.f, 1.f,	//bl
	 0.f, 1.f, 0.f,		0.f, 1.f,		0.f, 0.f, 1.f,	//tl
	1.f,  0.f, 0.f,		1.f, 0.f,		0.f, 0.f, 1.f,	//br

	1.f, 1.f, 0.f,		1.f, 1.f,		0.f, 0.f, 1.f,	//tr
	 0.f, 1.f, 0.f,		0.f, 1.f,		0.f, 0.f, 1.f,	//tl
	1.f,  0.f, 0.f,		1.f, 0.f,		0.f, 0.f, 1.f 	//br
};
GLfloat wallVertices_col[] = {	// colour
	//pos							col
	  0.f,  0.f, 0.f,  			0.85f, 0.95f, 0.85f,	//bl
	  0.f, 13.f, 0.f,  			0.85f, 0.95f, 0.85f,	//tl
	 13.f,  0.f, 0.f,  			0.85f, 0.95f, 0.85f,	//br

	 13.f, 13.f, 0.f,  			0.85f, 0.95f, 0.85f,	//tr
	  0.f, 13.f, 0.f,  			0.85f, 0.95f, 0.85f,	//tl
	 13.f,  0.f, 0.f,  			0.85f, 0.95f, 0.85f	//br
};
int sizeOfwallVertices = sizeof(wallVertices);

// hand-coded table (with normals)
GLfloat tableVertices_hand[] = {
	// tabletop cuboid					// tex			// normal
	-1.05f,  0.073f, -1.05f,			0.f, 0.f,		0.f, 1.f, 0.f,	// 0	up
	-1.05f,  0.073f,  1.05f,			0.f, 1.f,		0.f, 0.f, 1.f,	// 1	front
	 1.05f,  0.073f,  1.05f,			1.f, 1.f,		1.f, 0.f, 0.f,	// 2	right
	 1.05f,  0.073f, -1.05f,			1.f, 0.f,		0.f, 1.f, 0.f,	// 3
	-1.05f, -0.073f, -1.05f,			0.f, 0.f,		0.f, -1.f, 0.f,	// 4	bottom
	-1.05f, -0.073f,  1.05f,			0.f, 1.f,		-1.f, 0.f, 0.f,	// 5	left
	 1.05f, -0.073f,  1.05f,			1.f, 1.f,		0.f, 0.f, 0.f,	// 6
	 1.05f, -0.073f, -1.05f,			1.f, 0.f,		0.f, 0.f, -1.f,	// 7	back
	 // table leg 1 (origin)
	 -1.05f, -0.073f,		  -1.05f,	0.f, 0.f,		0.f, 1.f, 0.f,	// 8	up
	 -1.05f, -0.073f,		  -0.97f,	0.f, 0.f,		0.f, 0.f, 1.f,	// 9	front
	 -0.97f, -0.073f,		  -0.97f,	0.05f, 0.f,		1.f, 0.f, 0.f,	// 10	right
	 -0.97f, -0.073f,		  -1.05f,	0.05f, 0.f,		0.f, 0.f, 0.f,	// 11
	 -1.05f, -0.073f - 0.98f, -1.05f,	0.f, 0.05f,		0.f, -1.f, 0.f,	// 12	bottom
	 -1.05f, -0.073f - 0.98f, -0.97f,	0.f, 0.05f,		-1.f, 0.f, 0.f,	// 13	left
	 -0.97f, -0.073f - 0.98f, -0.97f,	0.05f, 0.05f,	0.f, 0.f, 0.f,	// 14	
	 -0.97f, -0.073f - 0.98f, -1.05f,	0.05f, 0.05f,	0.f, 0.f, -1.f,	// 15	back
	 // table leg 2 (left)
	 -1.05f, -0.073f,		  0.97f,	0.f, 0.f,		0.f, 1.f, 0.f,	// 16	up
	 -1.05f, -0.073f, 		  1.05f,	0.f, 0.f,		0.f, 0.f, 1.f,	// 17	front
	 -0.97f, -0.073f, 		  1.05f,	0.05f, 0.f,		1.f, 0.f, 0.f,	// 18	right
	 -0.97f, -0.073f, 		  0.97f,	0.05f, 0.f,		0.f, 0.f, 0.f,	// 19
	 -1.05f, -0.073f - 0.98f, 0.97f,	0.f, 0.05f,		0.f, -1.f, 0.f,	// 20	bottom
	 -1.05f, -0.073f - 0.98f, 1.05f,	0.f, 0.05f,		-1.f, 0.f, 0.f,	// 21	left
	 -0.97f, -0.073f - 0.98f, 1.05f,	0.05f, 0.05f,	0.f, 0.f, 0.f,	// 22
	 -0.97f, -0.073f - 0.98f, 0.97f,	0.05f, 0.05f,	0.f, 0.f, -1.f,	// 23	back
	 // table leg 3 (right)
	 0.97f,  -0.073f,		0.97f,		0.f, 0.f,		0.f, 1.f, 0.f,	// 24	up
	 0.97f,  -0.073f, 		1.05f,		0.f, 0.f,		0.f, 0.f, 1.f,	// 25	front
	 1.05f,  -0.073f,		1.05f,		0.05f, 0.f,		1.f, 0.f, 0.f,	// 26	right
	 1.05f,  -0.073f,		0.97f,		0.05f, 0.f,		0.f, 0.f, 0.f,	// 27
	 0.97f,  -0.073f - 0.98f, 0.97f,	0.f, 0.05f,		0.f, -1.f, 0.f,	// 28	bottom
	 0.97f,  -0.073f - 0.98f, 1.05f,	0.f, 0.05f,		-1.f, 0.f, 0.f,	// 29	left
	 1.05f,  -0.073f - 0.98f, 1.05f,	0.05f, 0.05f,	0.f, 0.f, 0.f,	// 30
	 1.05f,  -0.073f - 0.98f, 0.97f,	0.05f, 0.05f,	0.f, 0.f, -1.f,	// 31	back
	 // table leg 4 (front)
	0.97f,  -0.073f,		 -1.05f,	0.f, 0.f,		0.f, 1.f, 0.f,	// 32	up
	0.97f,  -0.073f,		 -0.97f,	0.f, 0.f,		0.f, 0.f, 1.f,	// 33	front
	1.05f,  -0.073f,		 -0.97f,	0.05f, 0.f,		1.f, 0.f, 0.f,	// 34	right
	1.05f,  -0.073f,		 -1.05f,	0.05f, 0.f,		0.f, 0.f, 0.f,	// 35
	0.97f,  -0.073f - 0.98f, -1.05f,	0.f, 0.05f,		0.f, -1.f, 0.f,	// 36	bottom
	0.97f,  -0.073f - 0.98f, -0.97f,	0.f, 0.05f,		-1.f, 0.f, 0.f,	// 37	left
	1.05f,  -0.073f - 0.98f, -0.97f,	0.05f, 0.05f,	0.f, 0.f, 0.f,	// 38
	1.05f,  -0.073f - 0.98f, -1.05f,	0.05f, 0.05f,	0.f, 0.f, -1.f	// 39	back
};
int sizeOfTableVertices_hand = sizeof(tableVertices_hand);
GLuint tableIndices[] = {
	// table top
	0, 1, 2,	0, 2, 3,	// up-side
	4, 5, 6,	4, 6, 7,	// bottom-side
	5, 1, 0,	5, 4, 0,	// left-side
	2, 3, 7,	2, 6, 7,	// right side
	1, 2, 6,	1, 5, 6,	// front-side
	7, 3, 0,	7, 4, 0,	// back-side
	// leg 1 (origin)
	8, 9, 10,	8, 11, 10,	// up-side
	12, 13, 14,	12, 15, 14,	// bottom-side
	13, 9, 8,	13, 12, 8,	// left
	10, 14, 15,	10, 11, 14,	// right
	9, 10, 14,	9, 13, 14,	// front
	15, 11, 8,	15, 12, 8,	// back
	// leg 2 (left)
	16, 17, 18,	16, 19, 18,	// up-side
	20, 21, 22,	20, 23, 22,	// bottom-side
	21, 17, 16,	21, 20, 16,	// left-side
	18, 19, 23,	18, 22, 23,	// right
	17, 18, 22,	17, 21, 22,	// front
	23, 19, 16,	23, 20, 16,	// back
	// leg 3 (front)
	24, 25, 26,	24, 27, 26,	// up
	28, 29, 30,	28, 31, 30,	// bottom
	29, 25, 24,	29, 28, 24,	// left
	26, 27, 31,	26, 30, 31,	// right
	25, 26, 30,	25, 29, 30,	// front
	31, 27, 24,	31, 28, 24,	// back
	// leg 4 (right)
	32, 33, 34,	32, 35, 34,	// up
	36, 37, 38,	36, 39, 38,	// bottom
	37, 33, 32,	37, 36, 32,	// left
	34, 35, 39, 34, 38, 39,	// right
	33, 34, 38,	33, 37, 38,	// front
	39, 35, 32,	39, 36, 32	// back
};
int sizeOfTableIndices = sizeof(tableIndices);


GLfloat cubeVertices[] = {
	//back face
	//pos					//col				//normal
	-0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, -1.f,
	 0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, -1.f,
	 0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, -1.f,
	 0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, -1.f,
	-0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, -1.f,
	-0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, -1.f,

	//front face
	-0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, 1.f,
	 0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, 1.f,
	 0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, 1.f,
	 0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, 1.f,
	-0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, 1.f,
	-0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 0.f, 1.f,

	//left face
	-0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	-1.f, 0.f, 0.f,
	-0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	-1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	-1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	-1.f, 0.f, 0.f,
	-0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	-1.f, 0.f, 0.f,
	-0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	-1.f, 0.f, 0.f,

	//right face
	0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	1.f, 0.f, 0.f,
	0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f, 	0.6f, 0.5f, 0.6f,	1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	1.f, 0.f, 0.f,
	0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	1.f, 0.f, 0.f,
	0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	1.f, 0.f, 0.f,

	//bottom face
	-0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, -1.f, 0.f,
	 0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, -1.f, 0.f,
	 0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, -1.f, 0.f,
	 0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, -1.f, 0.f,
	-0.5f, -0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, -1.f, 0.f,
	-0.5f, -0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, -1.f, 0.f,

	//top face
	-0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 1.f, 0.f,
	 0.5f,  0.5f, -0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 1.f, 0.f,
	 0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 1.f, 0.f,
	 0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 1.f, 0.f,
	-0.5f,  0.5f,  0.5f,  	0.6f, 0.5f, 0.6f,	0.f, 1.f, 0.f,
	-0.5f,  0.5f, -0.5f, 	0.6f, 0.5f, 0.6f,	0.f, 1.f, 0.f
};
int sizeOfCubeVertices = sizeof(cubeVertices);


// hand-coded table	(no normals)
//GLfloat tableVertices_hand[] = {
//	// tabletop cuboid					// tex
//	-1.05f,  0.073f, -1.05f,			0.f, 0.f,	// 0
//	-1.05f,  0.073f,  1.05f,			0.f, 1.f,	// 1
//	 1.05f,  0.073f,  1.05f,			1.f, 1.f,	// 2
//	 1.05f,  0.073f, -1.05f,			1.f, 0.f,	// 3
//	-1.05f, -0.073f, -1.05f,			0.f, 0.f,	// 4
//	-1.05f, -0.073f,  1.05f,			0.f, 1.f,	// 5
//	 1.05f, -0.073f,  1.05f,			1.f, 1.f,	// 6
//	 1.05f, -0.073f, -1.05f,			1.f, 0.f,	// 7
//	 // table leg 1 (origin)
//	 -1.05f, -0.073f,		  -1.05f,	0.f, 0.f,	// 8 (same as 4)
//	 -1.05f, -0.073f,		  -0.97f,	0.f, 0.f,	// 9
//	 -0.97f, -0.073f,		  -0.97f,	0.05f, 0.f,// 10
//	 -0.97f, -0.073f,		  -1.05f,	0.05f, 0.f,	// 11
//	 -1.05f, -0.073f - 0.98f, -1.05f,	0.f, 0.05f,	// 12
//	 -1.05f, -0.073f - 0.98f, -0.97f,	0.f, 0.05f,// 13
//	 -0.97f, -0.073f - 0.98f, -0.97f,	0.05f, 0.05f,// 14
//	 -0.97f, -0.073f - 0.98f, -1.05f,	0.05f, 0.05f, // 15
//	 // table leg 2 (left)
//	 -1.05f, -0.073f,		  0.97f,	0.f, 0.f,	// 16
//	 -1.05f, -0.073f, 		  1.05f,	0.f, 0.f,	// 17 (same as 5)
//	 -0.97f, -0.073f, 		  1.05f,	0.05f, 0.f,	// 18
//	 -0.97f, -0.073f, 		  0.97f,	0.05f, 0.f,	// 19
//	 -1.05f, -0.073f - 0.98f, 0.97f,	0.f, 0.05f,	// 20
//	 -1.05f, -0.073f - 0.98f, 1.05f,	0.f, 0.05f,	// 21
//	 -0.97f, -0.073f - 0.98f, 1.05f,	0.05f, 0.05f,	// 22
//	 -0.97f, -0.073f - 0.98f, 0.97f,	0.05f, 0.05f,	// 23
//	 // table leg 3 (right)
//	 0.97f,  -0.073f,		0.97f,		0.f, 0.f,	// 24
//	 0.97f,  -0.073f, 		1.05f,		0.f, 0.f,	// 25
//	 1.05f,  -0.073f,		1.05f,		0.05f, 0.f,	// 26 (same as 6)
//	 1.05f,  -0.073f,		0.97f,		0.05f, 0.f,	// 27
//	 0.97f,  -0.073f - 0.98f, 0.97f,	0.f, 0.05f,	// 28
//	 0.97f,  -0.073f - 0.98f, 1.05f,	0.f, 0.05f,	// 29
//	 1.05f,  -0.073f - 0.98f, 1.05f,	0.05f, 0.05f,	// 30
//	 1.05f,  -0.073f - 0.98f, 0.97f,	0.05f, 0.05f,	// 31
//	  // table leg 4 (front)
//	 0.97f,  -0.073f,		 -1.05f,	0.f, 0.f,	// 32
//	 0.97f,  -0.073f,		 -0.97f,	0.f, 0.f,	// 33
//	 1.05f,  -0.073f,		 -0.97f,	0.05f, 0.f,	// 34
//	 1.05f,  -0.073f,		 -1.05f,	0.05f, 0.f,	// 35 (same as 7)
//	 0.97f,  -0.073f - 0.98f, -1.05f,	0.f, 0.05f,	// 36
//	 0.97f,  -0.073f - 0.98f, -0.97f,	0.f, 0.05f,	// 37
//	 1.05f,  -0.073f - 0.98f, -0.97f,	0.05f, 0.05f,	// 38
//	 1.05f,  -0.073f - 0.98f, -1.05f,	0.05f, 0.05f	// 39
//};
