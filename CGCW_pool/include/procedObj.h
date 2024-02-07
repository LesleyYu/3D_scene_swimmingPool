#pragma once
#include <vector>
#include "basicGeo.h"


// Create a door frame (no colour) (x-y plane only)
inline float* CreateDoorFrame(float width, float height, float thickness) {
	float* vertices;
	vertices = (float*)malloc(sizeof(float) * 24 * 3);

	// the door frame is composed of three cuboids
	float* leftFrameVerts = CreateCuboid(thickness, thickness, height, -(float)(width + thickness) / 2.f, 0.f, 0.f);
	float* rightFrameVerts = CreateCuboid(thickness, thickness, height, (float)(width + thickness) / 2.f, 0.f, 0.f);
	float* topFrameVerts = CreateCuboid(thickness, width+2.f*thickness, thickness, 0.f, (float)(height + thickness) / 2.f, 0.f);

	// Loop: to contatenate three verts together into one
	unsigned int length = 24;
	for (int i = 0; i < length; i++) { vertices[i] = leftFrameVerts[i]; }
	for (int i = 24; i < length+24; i++) { vertices[i] = rightFrameVerts[i-24]; }
	for (int i = 48; i < length+48; i++) { vertices[i] = topFrameVerts[i-48]; }


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
			indices[i] = cuboidIndices[i - startIndex] + numPoint*n; 
		}
	}

	return indices;
}