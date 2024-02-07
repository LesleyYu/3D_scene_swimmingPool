#pragma once
#ifndef PI
#define PI 3.14159265358979323846
#endif

#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <vector>

struct SCylinder
{
	int num_segments;
	float radius;
	float height;

	//std::vector<float> vertices;
	//std::vector<int> indices;

	// used for generating indices
	int baseCenterIndex;
	int topCenterIndex;

	int sizeOfVerts;
	int sizeOfIndices;
};

inline void InitCylinder(SCylinder& in)
{
	in.num_segments = 32;
	in.radius = 1.f;
	in.height = 3.f;

	in.baseCenterIndex = 0;
	in.topCenterIndex = 0;
	in.sizeOfVerts = 0;
	in.sizeOfIndices = 0;
}

// procedural modelling: CREATING A CIRCLE (only verts)
inline std::vector<GLfloat> CreateCircle(SCylinder& cyl) {
	std::vector<GLfloat> vertices;

	float offset, angle;
	offset = 2 * PI / cyl.num_segments;
	angle = 0;
	for (int n = 0; n <= cyl.num_segments; n++) {
		// the start of the vertex index
		// triangle n: v0
		vertices.push_back(cos(angle)); // x
		vertices.push_back(sin(angle)); // y
		vertices.push_back(0);          // z

		// increment angle
		angle += offset;
	}
	return vertices;
}

// procedural modelling: cylinder vertices
inline std::vector<GLfloat> buildCylinderVerts(SCylinder& cyl) {
	std::vector<GLfloat> vertices;
	int num_segments = cyl.num_segments;
	float radius = cyl.radius;
	float height = cyl.height;

	// get unit circle vectors on XY-plane
	std::vector<GLfloat> unitVertices = CreateCircle(cyl);

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= num_segments; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];
			// position vector
			vertices.push_back(ux * radius);             // vx
			vertices.push_back(uy * radius);             // vy
			vertices.push_back(h);                       // vz
			// texture coordinate
			vertices.push_back((float)j / num_segments);// s
			vertices.push_back(t);						// t
			// normal vector		// !!!!
			vertices.push_back(ux);                       // nx
			vertices.push_back(uy);                       // ny
			vertices.push_back(uz);                       // nz
		}
	}

	// the starting index for the base/top surface
	cyl.baseCenterIndex = (int)vertices.size() / 3;
	cyl.topCenterIndex = cyl.baseCenterIndex + num_segments + 1; // include center vertex

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float nz = -1 + i * 2;                           // z value of normal; -1 to 1

		// center point
		vertices.push_back(0);		vertices.push_back(0);		vertices.push_back(h);	// v
		vertices.push_back(0.5f);	vertices.push_back(0.5f);							// t
		vertices.push_back(0);		vertices.push_back(0);		 vertices.push_back(nz);// n

		for (int j = 0, k = 0; j < num_segments; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			// position vector
			vertices.push_back(ux * radius);		// vx
			vertices.push_back(uy * radius);		// vy
			vertices.push_back(h);					// vz
			// texture coordinate
			vertices.push_back(-ux * 0.5f + 0.5f);	// s
			vertices.push_back(-uy * 0.5f + 0.5f);	// t
			// normal vector
			vertices.push_back(0);                  // nx
			vertices.push_back(0);                  // ny
			vertices.push_back(nz);                 // nz
		}
	}

	cyl.sizeOfVerts = vertices.size();

	return vertices;
}


// generate CCW index list of cylinder triangles
inline std::vector<GLuint> buildCylinderIndices(SCylinder& cyl) {
	std::vector<GLuint> indices;
	int num_segments = cyl.num_segments;
	float radius = cyl.radius;
	float height = cyl.height; 
	int baseCenterIndex = cyl.baseCenterIndex;
	int topCenterIndex = cyl.topCenterIndex;
	int k1 = 0;                         // 1st vertex index at base
	int k2 = num_segments + 1;           // 1st vertex index at top

	// indices for the side surface
	for (int i = 0; i < num_segments; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		indices.push_back(k1);
		indices.push_back(k1 + 1);
		indices.push_back(k2);

		// k2 => k1+1 => k2+1
		indices.push_back(k2);
		indices.push_back(k1 + 1);
		indices.push_back(k2 + 1);
	}

	// indices for the base surface
	for (int i = 0, k = baseCenterIndex + 1; i < num_segments; ++i, ++k)
	{
		if (i < num_segments - 1)
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < num_segments; ++i, ++k)
	{
		if (i < num_segments - 1)
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}
		else // last triangle
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}

	cyl.sizeOfIndices = indices.size();

	return indices;
}

