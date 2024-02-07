//#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <string>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "error.h"
#include "file.h"
#include "shader.h"
#include "shadow.h"
#include "cuboid.h"
#include "cylinder.h"
#include "handCodedModels.h"
#include "texture.h"
#include "texLoad.h"
#include "casteljau.h"
#include "point.h"
#include "objloader.h"


SCamera Camera;

#define NUM_BUFFERS 11
#define NUM_VAOS 11
#define NUM_IBOS 11
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];
GLuint IBOs[NUM_IBOS];

#define WIDTH 1024
#define HEIGHT 1024
#define SH_MAP_WIDTH 1024
#define SH_MAP_HEIGHT 1024


// swimming pool design
float pool_depth = 2.8f;
float pool_pos = 5.4f;

//light direction & position & color
// 1. sunlight
glm::vec3 sunlightDirection = glm::vec3(-.5f, -.81f, .31f);
glm::vec3 sunlightPos = glm::vec3(2.f, 6.f, 7.f);
glm::vec3 sunlightColor = glm::vec3(1.f, 1.f, 1.f);
// 2. spotlight
glm::vec3 lightDirection = glm::vec3(-.3f, -.3f, -.3f);
glm::vec3 lightPos = glm::vec3(3.f, 4.f, 4.f);
glm::vec3 lightColor = glm::vec3(0.9f, 0.9f, 0.5f);
void SetLighting(GLuint program) {
	glUniform3fv(glGetUniformLocation(program, "sunlightDirection"), 1, glm::value_ptr(sunlightDirection));
	glUniform3fv(glGetUniformLocation(program, "sunlightColor"), 1, glm::value_ptr(sunlightColor));
	glUniform3fv(glGetUniformLocation(program, "sunlightPos"), 1, glm::value_ptr(sunlightPos));

	glUniform3fv(glGetUniformLocation(program, "lightDirection"), 1, glm::value_ptr(lightDirection));
	glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos));

	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, glm::value_ptr(Camera.Position));
}

// POSITION VARIABLES 
glm::vec3 poolWall_pos = glm::vec3(pool_pos, -pool_depth, pool_pos);
glm::vec3 poolGround_pos = glm::vec3(pool_pos, -pool_depth, pool_pos);
glm::vec3 book1_pos = glm::vec3(1.8f, 0.98f + 0.073f * 2 + 0.043f * 2.5f + 0.073f, 4.2f);
glm::vec3 book2_pos = glm::vec3(1.8f, 0.98f + 0.073f * 2 + 0.043f * 2.5f, 4.2f);
glm::vec3 table_pos = glm::vec3(1.9f, 0.98f + 0.073f * 2.f, 4.5f);
glm::vec3 doorFrame_pos = glm::vec3(3.9f, 2.33f / 2.f + 0.296f, 0.053f);
glm::vec3 door_pos = glm::vec3(3.9f - 1.28f / 2.f, 2.33f / 2.f + 0.296f, 0.053f);
float table_rotation = -1.86f;


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// press 'q' to open/close window(sunlight)
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		if (sunlightColor == glm::vec3(0.f, 0.f, 0.f)) {
			sunlightColor = glm::vec3(.7f, .7f, 1.f);
		}
		else if (sunlightColor == glm::vec3(.7f, .7f, 1.f)) {
			sunlightColor = glm::vec3(1.f, .7f, .7f);
		}
		else if (sunlightColor == glm::vec3(1.f, .7f, .7f)) {
			sunlightColor = glm::vec3(.7f, 1.f, .7f);
		}
		else if (sunlightColor == glm::vec3(.7f, 1.f, .7f)) {
			sunlightColor = glm::vec3(.5f, .5f, .5f);
		}
		else {
			sunlightColor = glm::vec3(0.f, 0.f, 0.f);
		}
	}

	// press space to re-position the spot light
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		lightDirection = Camera.Front;
		lightPos = Camera.Position;
	}

	// press 't' to rotate the table
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (table_rotation == -1.86f) {
			table_rotation = -1.57f;
		}
		else {
			table_rotation = -1.86f;
		}
	}

	float x_offset = 0.f;
	float y_offset = 0.f;
	bool cam_changed = false;
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
		x_offset = -1.f;
		y_offset = 0.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
		x_offset = 1.f;
		y_offset = 0.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
		x_offset = 0.f;
		y_offset = 1.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
		x_offset = 0.f;
		y_offset = -1.f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_R && action == GLFW_REPEAT) {
		cam_dist += 0.1f;
		cam_changed = true;
	}
	if (key == GLFW_KEY_F && action == GLFW_REPEAT) {
		cam_dist -= 0.1f;
		cam_changed = true;
	}

	if (cam_changed) {
		MoveAndOrientCamera(Camera, glm::vec3(0.f, 0.f, 0.f), cam_dist, x_offset, y_offset);
	}
}

void ResizeCallback(GLFWwindow*, int w, int h)
{
	glViewport(0, 0, w, h);
}

// writing data into buffers
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

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, int attrib1, int attrib2, int attrib3) {
	// object buffer
	glNamedBufferStorage(Buffers[id], sizeVerts, verts, 0);
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	// setup each attribute in the vertex array in the VAO
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, attrib2, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)(attrib1 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, attrib3, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)((attrib1 + attrib2) * sizeof(float)));
	glEnableVertexAttribArray(2);
	// unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
}

void PopulateBuffer(int id, pos* verts, int sizeVerts, int attrib1, int attrib2, int attrib3) {
	// object buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, &verts[0], GL_STATIC_DRAW);
	// setup each attribute in the vertex array in the VAO
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, attrib2, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)(attrib1 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, attrib3, GL_FLOAT, GL_FALSE, (attrib1 + attrib2 + attrib3) * sizeof(float), (void*)((attrib1 + attrib2) * sizeof(float)));
	glEnableVertexAttribArray(2);
	// unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
}

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, GLuint* indices, int sizeIndices, int attrib1) {
	// object buffer & index buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);
	// attributes
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, attrib1 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// IBO
}

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, GLuint* indices, int sizeIndices, int attrib1, int attrib2) {
	// object buffer & index buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);
	// attributes
	glVertexAttribPointer(0, attrib1, GL_FLOAT, GL_FALSE, (attrib1 + attrib2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, attrib2, GL_FLOAT, GL_FALSE, (attrib1 + attrib2) * sizeof(float), (void*)(attrib1 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// UnBinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// VBO
	glBindVertexArray(0);						// VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// IBO
}

void PopulateBuffer(int id, GLfloat* verts, int sizeVerts, GLuint* indices, int sizeIndices, int attrib1, int attrib2, int attrib3) {
	// object buffer & index buffer
	glBindVertexArray(VAOs[id]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[id]);
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// IBO
}

// draw function for shadowed objects
void drawFloorAndObjs(GLuint shaderProgram)
{
	glBindVertexArray(VAOs[5]);

	//floor
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(7, 0.1, 2));
	model = glm::scale(model, glm::vec3(3, 0.01, 3));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	//object: chair
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(5.5, 0.1, 2));
	model = glm::scale(model, glm::vec3(.4f, .4f, .4f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAOs[10]);
	glDrawArrays(GL_TRIANGLES, 0, 365280);	// chairVerts.size() = 365280

}

void generateDepthMap(GLuint shadowShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{
	glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	drawFloorAndObjs(shadowShaderProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void drawCurve(GLuint program, int num) {
	// set up and copy model matrix
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(pool_pos, -1.f, pool_pos));
	model = glm::scale(model, glm::vec3(0.4, 0.4, 0));
	// draw 10 curves
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(2.f, 0.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[6]);
		glDrawArrays(GL_LINE_STRIP, 0, num);
	}
	// set up and copy model matrix
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(pool_pos, -1.f, pool_pos));
	model = glm::rotate(model, -1.57f, glm::vec3(0.f, 1.f, 0.f));		// rotation
	model = glm::scale(model, glm::vec3(0.4, 0.4, 0));
	// draw 10 curves
	for (int i = 0; i < 5; i++) {
		model = glm::translate(model, glm::vec3(2.f, 0.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[6]);
		glDrawArrays(GL_LINE_STRIP, 0, num);
	}
}

int main(int argc, char** argv)
{
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 8);	// antialiasing
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "swimming pool", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetWindowSizeCallback(window, ResizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugMessageCallback, 0);

	// shadow initialisation
	ShadowStruct shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);
	GLuint shadow_program = CompileShader("shadow.vert", "shadow.frag");

	// camera initialisation
	InitCamera(Camera);
	MoveAndOrientCamera(Camera, glm::vec3(0.f, 0.f, 0.f), cam_dist, 0.f, 0.f);


	// programs
	GLuint texPhongProgram = CompileShader("texPhong.vert", "texPhong.frag");
	GLuint defaultProgram = CompileShader("default.vert", "default.frag");
	GLuint shaPhongProgram = CompileShader("shaPhong.vert", "shaPhong.frag");

	// textures
	GLuint ayatoTex = setup_texture("KamisatoAyato.bmp");
	GLuint wallTex = setup_mipmaps(wallTexFiles, 11);
	GLuint poolWallTex = setup_mipmaps(poolWallTexFiles, 11);
	GLuint tableTex = setup_mipmaps(poolGroundTexFiles, 11);
	GLuint tableTex2 = setup_mipmaps(bookTexFiles, 11);
	GLuint groundTex = setup_mipmaps(groundTexFiles, 11);
	GLuint poolGroundTex = setup_mipmaps(tableTexFiles, 11);
	GLuint bookTex = setup_mipmaps(bookTexFiles, 11);
	GLuint doorTex = setup_mipmaps(doorTexFiles, 11);

	// Create buffers and VAOs
	glCreateBuffers(NUM_BUFFERS, Buffers);
	glGenVertexArrays(NUM_VAOS, VAOs);
	// create IBO
	glGenBuffers(NUM_IBOS, IBOs);

	// wall
	PopulateBuffer(0, wallVertices, sizeOfwallVertices, 3, 2, 3);

	// create cuboids: books
	float* bookVerts = CreateCuboid(0.46f, 0.67f, 0.073f);
	// generate indices for books
	GLuint* bookIndices = GenerateCuboidsIndices(1);
	// book (with IBO)
	PopulateBuffer(1, bookVerts, sizeof(float) * (40 + 24), bookIndices, sizeof(GLuint) * 36, 3, 2, 3);

	// table (hand-coded)
	PopulateBuffer(2, tableVertices_hand, sizeOfTableVertices_hand, tableIndices, sizeOfTableIndices, 3, 2, 3);

	// create cuboids: door
	float* doorVerts = CreateDoor(0.053f, 1.28f, 2.83f);
	// generate indices for cuboids
	GLuint* doorIndices = GenerateCuboidsIndices(1);
	// door (with IBO)
	PopulateBuffer(3, doorVerts, sizeof(float) * (40 + 24), doorIndices, sizeof(GLuint) * 36, 3, 2, 3);

	// create door frame
	float* doorFrameVerts = CreateDoorFrame(1.28f, 2.83f, 0.053f);
	// GLuint doorFrameIndices[] = {
	//	// cuboid 1
	//	0, 1, 2,	0, 2, 3,	// up-side
	//	4, 5, 6,	4, 6, 7,	// down-side
	//	0, 1, 5,	0, 4, 5,	// left-side
	//	2, 3, 7,	2, 6, 7,	// right side
	//	1, 2, 6,	1, 5, 6,	// front-side
	//	0, 3, 7,	0, 4, 7,		// back-side
	//	// cuboid 2
	//	0 + 8, 1 + 8, 2 + 8,	0 + 8, 2 + 8, 3 + 8,	// up-side
	//	4 + 8, 5 + 8, 6 + 8,	4 + 8, 6 + 8, 7 + 8,	// down-side
	//	0 + 8, 1 + 8, 5 + 8,	0 + 8, 4 + 8, 5 + 8,	// left-side
	//	2 + 8, 3 + 8, 7 + 8,	2 + 8, 6 + 8, 7 + 8,	// right side
	//	1 + 8, 2 + 8, 6 + 8,	1 + 8, 5 + 8, 6 + 8,	// front-side
	//	0 + 8, 3 + 8, 7 + 8,	0 + 8, 4 + 8, 7 + 8,		// back-side
	//	// cuboid 3
	//	0 + 16, 1 + 16, 2 + 16,	0 + 16, 2 + 16, 3 + 16,	// up-side
	//	4 + 16, 5 + 16, 6 + 16,	4 + 16, 6 + 16, 7 + 16,	// down-side
	//	0 + 16, 1 + 16, 5 + 16,	0 + 16, 4 + 16, 5 + 16,	// left-side
	//	2 + 16, 3 + 16, 7 + 16,	2 + 16, 6 + 16, 7 + 16,	// right side
	//	1 + 16, 2 + 16, 6 + 16,	1 + 16, 5 + 16, 6 + 16,	// front-side
	//	0 + 16, 3 + 16, 7 + 16,	0 + 16, 4 + 16, 7 + 16		// back-side
	//};
	GLuint* doorFrameIndices = GenerateCuboidsIndices(3);
	// door frame (with IBO)
	PopulateBuffer(4, doorFrameVerts, sizeof(float) * 24 * 3, doorFrameIndices, sizeof(GLuint) * 36 * 3, 3);

	// shadow cubes
	PopulateBuffer(5, cubeVertices, sizeOfCubeVertices, 3, 3, 3);

	// curves
	std::vector<point> ctrl_points;
	ctrl_points.push_back(point(-1.f, 0.f, 0.f));
	ctrl_points.push_back(point(-.75f, 1.75f, 0.f));
	ctrl_points.push_back(point(-.25f, -1.5f, 0.f));
	ctrl_points.push_back(point(.25f, 1.5f, 0.f));
	ctrl_points.push_back(point(-1.4f, -.25f, 0.f));
	ctrl_points.push_back(point(.85f, .9f, 0.f));
	ctrl_points.push_back(point(1.f, 0.f, 0.f));
	// number of evals
	int num_evaluations = 64;
	std::vector<point> curve = EvaluateBezierCurve(ctrl_points, num_evaluations);
	// generate vertices
	int num_curve_verts = curve.size();
	int num_curve_floats = num_curve_verts * 6;
	float* curve_vertices = MakeFloatsFromVector(curve, num_curve_verts, num_curve_floats, 0.f, 0.f, 0.f);
	PopulateBuffer(6, curve_vertices, sizeof(float) * num_curve_floats, 3, 3);

	// cylinder
	SCylinder cylinder;
	InitCylinder(cylinder);
	cylinder.radius = 0.08f;
	cylinder.height = 1.2f;
	std::vector<GLfloat> verts = buildCylinderVerts(cylinder);;
	GLfloat* CylinderVerts = &verts[0];
	std::vector<GLuint> indices = buildCylinderIndices(cylinder);;
	GLuint* CylinderIndices = &indices[0];
	PopulateBuffer(7, CylinderVerts, cylinder.sizeOfVerts * sizeof(GLfloat), CylinderIndices, cylinder.sizeOfIndices * sizeof(GLuint), 3, 2, 3);

	// load OBJ file
	// 1. Kamisato Ayato
	std::vector< GLfloat > AyatoVerts;
	int AyatoLength = -1;
	bool KamisatoAyato = loadOBJ("objModels/KamisatoAyato.obj", AyatoVerts, &AyatoLength); // KamisatoAyato.obj
	PopulateBuffer(8, &AyatoVerts[0], AyatoLength * sizeof(GLfloat), 3, 2, 3);
	// deck chair
	std::vector< GLfloat > chair2Verts;
	int chair2Length = -1;
	bool deck_chair2 = loadOBJ("objModels/deck_chair2.obj", chair2Verts, &chair2Length);
	PopulateBuffer(9, &chair2Verts[0], chair2Length * sizeof(GLfloat), 3, 2, 3);
	// chair
	std::vector< GLfloat > chairVerts;
	int chairLength = -1;
	bool chair = loadOBJ("objModels/chair.obj", chairVerts, &chairLength); // KamisatoAyato.obj
	PopulateBuffer(10, &chairVerts[0], chairLength * sizeof(GLfloat), 3, 2, 3);


	// other settings
	glEnable(GL_DEPTH_TEST);	//ENABLE DEPTH TEST
	glPointSize(8);
	// Provoking Vertex. enables flat shading: normals of the vertex only depend on the 1st vertex
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);


	while (!glfwWindowShouldClose(window))
	{
		static const GLfloat bgd[] = { 1.f, 1.f, 1.f, 1.f };
		glClearBufferfv(GL_COLOR, 0, bgd);
		// Clear the depth buffer at the start of the render loop.
		glClear(GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		// --------------- 3D environment setup--------------
		// the VIEW matrix: from WORLD space to VIEW space
		// set the camera position and rotation
		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
		// the PROJECTION matrix: from VIEW space to CLIP space
		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(75.f), (float)WIDTH / (float)HEIGHT, 0.1f, 40.f);
		// initialise model matrix
		glm::mat4 model;
		// sunlight transformations through time: animation
		glm::mat4 rotationMat(1); // Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, 0.003f, glm::vec3(0.f, 0.f, 1.f));
		sunlightDirection = glm::vec3(rotationMat * glm::vec4(sunlightDirection, 0.0));
		sunlightPos = glm::vec3(rotationMat * glm::vec4(sunlightPos, 0.0));
		// shadow
		float near_plane = 1.0f, far_plane = 50.5f;
		glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(sunlightPos, sunlightPos + sunlightDirection, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;


		// program setting: texture phong program (lightings on textured objects)
		glUseProgram(texPhongProgram);
		// view and projection
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// lighting
		SetLighting(texPhongProgram);

		// -------------- 2D plane: walls --------------
		// set up and copy model matrix: 
		// 1. the x-y wall
		model = glm::mat4(1.f);
		model = glm::scale(model, glm::vec3(13.f, 13.f, 1.f));				// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, wallTex);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 2. the y-z wall
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, 13.f));
		model = glm::rotate(model, 1.57f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		model = glm::scale(model, glm::vec3(13.f, 13.f, 1.f));				// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 3. the x-y pool wall
		glBindTexture(GL_TEXTURE_2D, poolWallTex);
		model = glm::mat4(1.f);
		model = glm::translate(model, poolWall_pos + glm::vec3(0.f, pool_depth - 13.f, 0.f));
		model = glm::scale(model, glm::vec3(13.f, 13.f, 1.f));				// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 4. the y-z pool wall
		model = glm::mat4(1.f);
		model = glm::translate(model, poolWall_pos + glm::vec3(0.f, pool_depth, 0.f));
		model = glm::rotate(model, -3.14f, glm::vec3(0.f, 0.f, 1.f));		// rotation: rotate 180deg so that the normal is inversed as well
		model = glm::rotate(model, -1.57f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		model = glm::scale(model, glm::vec3(13.f, 13.f, 1.f));				// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 4. the pool ground
		glBindTexture(GL_TEXTURE_2D, poolGroundTex);
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(pool_pos, -2.8f, 8.f + pool_pos));
		model = glm::rotate(model, -1.57f, glm::vec3(1.f, 0.f, 0.f));		// rotation
		model = glm::scale(model, glm::vec3(8.f, 8.f, 1.f));				// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 5. ground (made up by two)
		glBindTexture(GL_TEXTURE_2D, groundTex);
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, 13.f));
		model = glm::rotate(model, -1.57f, glm::vec3(1.f, 0.f, 0.f));		// rotation
		model = glm::scale(model, glm::vec3(pool_pos, 13.f, 1.f));			// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, pool_pos));
		model = glm::rotate(model, -1.57f, glm::vec3(1.f, 0.f, 0.f));		// rotation
		model = glm::scale(model, glm::vec3(13.f, pool_pos, 1.f));			// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// -------------- Cuboids: books --------------
		// texture
		glBindTexture(GL_TEXTURE_2D, bookTex);
		// set up and copy MODEL matrix separately
		// 1. book 1
		model = glm::mat4(1.f);
		model = glm::translate(model, book1_pos);
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		// 2. book 2
		model = glm::mat4(1.f);
		model = glm::translate(model, book2_pos);
		model = glm::rotate(model, 3.f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		model = glm::scale(model, glm::vec3(1.1f, 1.5f, 1.1f));			// scale
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, doorTex);
		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		// 3. book 3
		model = glm::mat4(1.f);
		model = glm::translate(model, book2_pos + glm::vec3(0.2f, 0.f, 0.7f));
		model = glm::rotate(model, .3f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// -------------- table (hand-coded) --------------
		// texture
		glBindTexture(GL_TEXTURE_2D, tableTex);
		// set up and copy model matrix
		model = glm::mat4(1.f);
		model = glm::translate(model, table_pos);
		model = glm::rotate(model, table_rotation, glm::vec3(0.f, 1.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[2]);
		glDrawElements(GL_TRIANGLES, sizeOfTableIndices / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// -------------- Cuboid: Door --------------
		// texture
		glBindTexture(GL_TEXTURE_2D, doorTex);
		// MODEL matrix
		model = glm::mat4(1.f);
		model = glm::translate(model, door_pos);
		// animation
		float rotationDeg;
		int n = glfwGetTime() / 1.99f;
		// rotationDeg = -((float)glfwGetTime() - 1.99f * n);
		if ((n % 2) == 0) {		// close the door
			rotationDeg = -((float)glfwGetTime() - 1.99f * n);
		}
		else {					// open the door
			rotationDeg = -(1.99f - ((float)glfwGetTime() - 1.99f * n));
		}
		model = glm::rotate(model, rotationDeg, glm::vec3(0.f, 1.f, 0.f));
		// lighting
		SetLighting(texPhongProgram);
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[3]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// -------------- cylinder (procedually generated) --------------
		// texture
		glBindTexture(GL_TEXTURE_2D, bookTex);
		// set up and copy model matrix
		// 1. up horizontal handle A
		glm::vec3 handle_pos = glm::vec3(pool_pos + 0.5f, 0.3f, pool_pos - 0.1f);
		model = glm::mat4(1.f);
		model = glm::translate(model, handle_pos);
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[7]);
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 2. up horizontal handle B
		model = glm::translate(model, glm::vec3(1.1f, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 3. inner vertical handle A
		model = glm::mat4(1.f);
		model = glm::translate(model, handle_pos + glm::vec3(0.f, -0.55f, -0.55f));
		model = glm::rotate(model, -1.57f, glm::vec3(1.f, 0.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 4. inner vertical handle B
		model = glm::translate(model, glm::vec3(1.1f, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 5. outer vertical handle A
		model = glm::mat4(1.f);
		model = glm::scale(model, glm::vec3(1, 3.5, 1));
		model = glm::translate(model, handle_pos + glm::vec3(0.f, -0.795f, 0.55f));
		model = glm::rotate(model, -1.57f, glm::vec3(1.f, 0.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 6. inner vertical handle B
		model = glm::translate(model, glm::vec3(1.1f, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 7. stair 1
		model = glm::mat4(1.f);
		model = glm::scale(model, glm::vec3(1, 0.5, 1));
		model = glm::translate(model, handle_pos + glm::vec3(0.55f, -1.f, 0.55f));
		model = glm::rotate(model, -1.57f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 8. stair 2
		model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 9. stair 3
		model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		// 10. stair 4
		model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, cylinder.sizeOfIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// -------------- Ayato (OBJ file) --------------
		// texture
		glBindTexture(GL_TEXTURE_2D, ayatoTex);
		// set up and copy model matrix
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(4.0f, 0.f, 7.f));
		model = glm::scale(model, glm::vec3(0.17, 0.17, 0.17));
		model = glm::rotate(model, 1.57f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[8]);
		glDrawArrays(GL_TRIANGLES, 0, AyatoVerts.size());
		glBindVertexArray(0);
		
		// -------------- deck chair (OBJ file) --------------
		// texture
		glBindTexture(GL_TEXTURE_2D, doorTex);
		// set up and copy model matrix
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(2.3f, 0.f, 7.f));
		model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4f));
		//model = glm::rotate(model, 1.57f, glm::vec3(0.f, 1.f, 0.f));		// rotation
		glUniformMatrix4fv(glGetUniformLocation(texPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// draw
		glBindVertexArray(VAOs[9]);
		glDrawArrays(GL_TRIANGLES, 0, chair2Verts.size());
		glBindVertexArray(0);


		// program setting: texture phong program (lightings on textured objects)
		glUseProgram(defaultProgram);
		// view and projection
		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// -------------- door frame (procedurally generated) --------------
		// set up and copy model matrix
		model = glm::mat4(1.f);
		model = glm::translate(model, doorFrame_pos);
		glUniformMatrix4fv(glGetUniformLocation(defaultProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		// uCol
		glUniform3f(glGetUniformLocation(defaultProgram, "uCol"), .2f, .1f, 0.05f);	// the colour
		// draw
		glBindVertexArray(VAOs[4]);
		glDrawElements(GL_TRIANGLES, 36 * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// -------------- bezier curves --------------
		// uCol
		glUniform3f(glGetUniformLocation(defaultProgram, "uCol"), .6f, 0.6f, 1.0f);	// the colour
		drawCurve(defaultProgram, num_curve_verts);
		glBindVertexArray(0);


		// -------------- chair 2 (shadow) --------------
		// May cause the lighting animations become static (fixed)
		generateDepthMap(shadow_program, shadow, projectedLightSpaceMatrix);
		// saveShadowMapToBitmap(shadow.Texture, SH_MAP_WIDTH, SH_MAP_HEIGHT);	// doesn't need generation every frame: other lightings stop animation!
		glUseProgram(shaPhongProgram);
		glBindTexture(GL_TEXTURE_2D, shadow.Texture);
		// lightings
		glUniformMatrix4fv(glGetUniformLocation(shaPhongProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
		SetLighting(shaPhongProgram);
		// view and projection
		glUniformMatrix4fv(glGetUniformLocation(shaPhongProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaPhongProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// uCol
		glUniform3f(glGetUniformLocation(shaPhongProgram, "uCol"), .6f, .6f, .6f); // .6f, .3f, .0f);	// the colour
		// draw
		drawFloorAndObjs(shaPhongProgram);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
 		glfwPollEvents();
	}

	// free memory for procedually created objs
	free(bookVerts);
	free(doorFrameVerts);

	// Delete all the objects created
	glDeleteVertexArrays(NUM_VAOS, VAOs);
	glDeleteBuffers(NUM_BUFFERS, Buffers);
	glDeleteBuffers(NUM_IBOS, IBOs);
	glDeleteProgram(texPhongProgram);
	glDeleteProgram(shaPhongProgram);
	glDeleteProgram(defaultProgram);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
