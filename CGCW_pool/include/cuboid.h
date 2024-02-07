#pragma once

#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


// CREATING A cuboid (with texture)
inline float* CreateCuboid(float length, float width, float height) {
	float* vertices;
	vertices = (float*)malloc(sizeof(float) * 64);
	// get half value of params to find the centre of the object
	float x = width / 2.f;	// half of width	(in x-direction)
	float y = height / 2.f;	// half of height	(in y-direction)
	float z = length / 2.f;	// half of length	(in z-direction)

	GLfloat xzPlane[] = {
		//pos			tex			normal
		-x, y, -z,		1.f, 1.f,	0.f, 1.f, 0.f,	// 0
		-x, y,  z,		0.f, 1.f,	-1.f, 0.f, 0.f,	// 1
		 x, y,  z,		0.f, 0.f,	0.f, 0.f, 1.f,	// 2
		 x, y, -z,		1.f, 0.f	,0.f, 0.f, 0.f	// 3
	};
	int len = sizeof(xzPlane) / sizeof(GLfloat);

	// up-side
	for (int i = 0; i < len; i++) {
		vertices[i] = xzPlane[i];
	}

	// bottom-side
	for (int i = len; i < len * 2; i++) {
		if (vertices[i - len] == y) { vertices[i] = -y; }
		else { vertices[i] = vertices[i - len]; }
	}

	// handling normals
	vertices[38] = -1.f;
	vertices[45] =  0.f;
	vertices[53] =  1.f;
	vertices[55] =  0.f;
	vertices[63] = -1.f;

	return vertices;
}


// CREATING A cuboid (with texture, origin on the left side)
inline float* CreateDoor(float length, float width, float height) {
	float* vertices;
	vertices = (float*)malloc(sizeof(float) * 64);

	// get half value of params to find the centre of the object
	float y = height / 2.f;	// half of height	(in y-direction)
	float z = length / 2.f;	// half of length	(in z-direction)

	GLfloat xzPlane[] = {
		//pos			tex			normal
		0.f, y, -z,		0.f, 1.f,	0.f, 1.f, 0.f,	// 0
		0.f, y,  z,		0.f, 1.f,	-1.f, 0.f, 0.f,	// 1
		width, y,z,		0.5f, 1.f,	0.f, 0.f, 1.f,	// 2
		width, y,-z,	0.5f, 1.f	,0.f, 0.f, 0.f	// 3
	};
	int len = sizeof(xzPlane) / sizeof(GLfloat);

	// up-side
	for (int i = 0; i < len; i++) {
		vertices[i] = xzPlane[i];
	}
	// bottom-side
	for (int i = len; i < len * 2; i++) {
		if (vertices[i - len] == y) { vertices[i] = -y; }
		else { vertices[i] = vertices[i - len]; }
	}

	// handling normals
	vertices[38] = -1.f;
	vertices[45] = 0.f;
	vertices[53] = 1.f;
	vertices[55] = 0.f;
	vertices[63] = -1.f;
	// handling textures
	for (int i = 36; i < 64; i += 8) { vertices[i] = 0.f; }

	//// up-side
	//for (int i = 1; i < 5 * 4; i += 5) { vertices[i] = y; }	// plane z 
	//// v0
	//vertices[0] = 0.f;
	//vertices[2] = -z;
	////		tex
	//vertices[3] = 0.f;
	//vertices[4] = 1.0f;
	//// v1
	//vertices[5] = 0.f;
	//vertices[7] = +z;
	////		tex
	//vertices[8] = 0.f;
	//vertices[9] = 1.0f;
	//// v2
	//vertices[10] = +width;
	//vertices[12] = +z;
	////		tex
	//vertices[13] = 0.5f;
	//vertices[14] = 1.0f;
	//// v3
	//vertices[15] = +width;
	//vertices[17] = -z;
	////		tex
	//vertices[18] = 0.5f;
	//vertices[19] = 1.0f;
	//
	//// down-side
	//for (int i = 21; i < 5 * 4 * 2; i += 5) { vertices[i] = -y; }	// plane -z 
	//// v4
	//vertices[20] = 0.f;
	//vertices[22] = -z;
	////		tex
	//vertices[23] = 0.f;
	//vertices[24] = 0.f;
	//// v5
	//vertices[25] = 0.f;
	//vertices[27] = +z;
	////		tex
	//vertices[28] = 0.f;
	//vertices[29] = 0.f;
	//// v6
	//vertices[30] = +width;
	//vertices[32] = +z;
	////		tex
	//vertices[33] = 0.5f;
	//vertices[34] = 0.f;
	//// v7
	//vertices[35] = +width;
	//vertices[37] = -z;
	////		tex
	//vertices[38] = 0.5f;
	//vertices[39] = 0.f;

	return vertices;
}

// CREATING A cuboid (no colour no texture) (has translations)
// params for translations. Useful for creating cuboids in part of another object
inline float* CreateCuboid(float length, float width, float height, float pos_x, float pos_y, float pos_z) {
	float* vertices;
	vertices = (float*)malloc(sizeof(float) * 24);
	// get half value of params to find the centre of the object
	float x = width / 2.f;	// half of width	(in x-direction)
	float y = height / 2.f;	// half of height	(in y-direction)
	float z = length / 2.f;	// half of length	(in z-direction)

	// up-side
	for (int i = 1; i < 3 * 4; i += 3) { vertices[i] = y + pos_y; }	// plane z 
	// v0
	vertices[0] = -x + pos_x;
	vertices[2] = -z + pos_z;
	// v1
	vertices[3] = -x + pos_x;
	vertices[5] = +z + pos_z;
	// v2
	vertices[6] = +x + pos_x;
	vertices[8] = +z + pos_z;
	// v3
	vertices[9] = +x + pos_x;
	vertices[11] = -z + pos_z;

	// down-side
	for (int i = 13; i < 24; i += 3) { vertices[i] = -y + pos_y; }	// plane -z 
	// v4
	vertices[12] = -x + pos_x;
	vertices[14] = -z + pos_z;
	// v5
	vertices[15] = -x + pos_x;
	vertices[17] = +z + pos_z;
	// v6
	vertices[18] = +x + pos_x;
	vertices[20] = +z + pos_z;
	// v7
	vertices[21] = +x + pos_x;
	vertices[23] = -z + pos_z;

	return vertices;
}


// Create a door frame (no colour) (x-y plane only)
inline float* CreateDoorFrame(float width, float height, float thickness) {
	float* vertices;
	vertices = (float*)malloc(sizeof(float) * 24 * 3);

	// the door frame is composed of three cuboids
	float* leftFrameVerts = CreateCuboid(thickness, thickness, height, -(float)(width + thickness) / 2.f, 0.f, 0.f);
	float* rightFrameVerts = CreateCuboid(thickness, thickness, height, (float)(width + thickness) / 2.f, 0.f, 0.f);
	float* topFrameVerts = CreateCuboid(thickness, width + 2.f * thickness, thickness, 0.f, (float)(height + thickness) / 2.f, 0.f);

	// Loop: to contatenate three verts together into one
	int length = 24;
	for (int i = 0; i < length; i++) { vertices[i] = leftFrameVerts[i]; }
	for (int i = 24; i < length + 24; i++) { vertices[i] = rightFrameVerts[i - 24]; }
	for (int i = 48; i < length + 48; i++) { vertices[i] = topFrameVerts[i - 48]; }


	// free memory of 3 separate cuboids
	free(leftFrameVerts);
	free(rightFrameVerts);
	free(topFrameVerts);

	return vertices;
}

// todo: procedually generate indices for cuboids
inline GLuint* GenerateCuboidsIndices(int num) {
	// 1 cuboid consists of 36 indices
	int sizeCuboid = 36;
	// 1 cuboid consists of 8 points
	int numPoint = 8;

	// memory storage
	GLuint* indices = (GLuint*)malloc(sizeof(GLuint) * sizeCuboid * num);

	// indices for one cuboid	// corresponds to the normals!!
	GLuint cuboidIndices[] = {
		0, 1, 2,	0, 3, 2,	// up-side
		4, 5, 6,	4, 7, 6,	// down-side
		1, 5, 4,	1, 0, 4,	// left-side
		6, 7, 3,	6, 2, 3,	// right side
		2, 1, 5,	2, 6, 5,	// front-side
		7, 3, 0,	7, 4, 0		// back-side
	};

	for (int n = 0; n < num; n++) {
		// the start index of the n-th cuboid
		int startIndex = n * sizeCuboid;
		// generate indices
		for (int i = startIndex; i < startIndex + sizeCuboid; i++) {
			indices[i] = cuboidIndices[i - startIndex] + numPoint * n;
		}
	}

	return indices;
}