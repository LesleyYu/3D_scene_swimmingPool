#pragma once

#define NUM_BUFFERS 9
#define NUM_VAOS 9
#define NUM_EBOS 9

void GenerateBuffers(int numB, int numA, int numE) {
	GLuint Buffers[NUM_BUFFERS];
	GLuint VAOs[NUM_VAOS];
	GLuint EBOs[NUM_EBOS];
	// Create buffers and VAOs
	glCreateBuffers(numB, Buffers);
	glGenVertexArrays(numA, VAOs);
	// create EBO
	glGenBuffers(numE, EBOs);
}

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, int attrib1, int attrib2) {
	// object buffer
	glNamedBufferStorage(Buffers[id], sizeVerts, verts, 0);
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	// setup each attribute in the vertex array in the VAO
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, (attrib1 + attrib2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, attrib2, GL_FLOAT, GL_FALSE, (attrib1 + attrib2) * sizeof(float), (void*)(attrib1 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
}

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, GLuint* indices, int sizeIndices, int attrib1, int attrib2) {
	// object buffer & index buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);
	// attributes
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, (attrib1 + attrib2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, attrib2, GL_FLOAT, GL_FALSE, (attrib1 + attrib2) * sizeof(float), (void*)(attrib1 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// EBO
}

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, GLuint* indices, int sizeIndices, int attrib1) {
	// object buffer & index buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);
	// attributes
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, attrib1 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// EBO
}

inline void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, GLuint* indices, int sizeIndices, int attrib1, int attrib2, int attrib3) {
	// object buffer & index buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);
	// attributes
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, attrib2, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)(attrib1 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, attrib3, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)((attrib1 + attrib2) * sizeof(float)));
	glEnableVertexAttribArray(2);
	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// EBO
}

