#pragma once

#include <stdio.h>
#include <glm/glm/glm.hpp>

struct SCamera
{
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	const float MovementSpeed = 3.5f;
	float MouseSensitivity = 10.f;

};


void InitCamera(SCamera& in)
{
	in.Front = glm::vec3(0.0f, 0.0f, -1.0f);
	in.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	in.Up = glm::vec3(0.0f, 1.0f, 0.0f);
	in.WorldUp = in.Up;
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));

	in.Yaw = -(90.f + 45.f);
	in.Pitch = -20.f;
}

float cam_dist = -13.f;

void MoveAndOrientCamera(SCamera& in, glm::vec3 target, float distance, float xoffset, float yoffset)
{
	in.Yaw = in.Yaw + xoffset * in.MovementSpeed;
	in.Pitch = in.Pitch + yoffset * in.MovementSpeed;

	if (in.Pitch > 10.f)
		in.Pitch = 10.0f;
	if (in.Pitch < -70.f)
		in.Pitch = -70.f;
	if (in.Yaw < -(135.f + 40.f))
		in.Yaw = -(135.f + 40.f);
	if (in.Yaw > -(135.f - 40.f))
		in.Yaw = -(135.f - 40.f);
	if (cam_dist < -17.f ) cam_dist = -17.f;

	// calculate new position of the camera
	float Px, Py, Pz;
	Py = sin(glm::radians(in.Pitch));
	Px = cos(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch));
	Pz = sin(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch));
	in.Position = glm::vec3(Px * distance, Py * distance, Pz * distance);

	// calculate the new front direction
	in.Front = glm::normalize(target - in.Position);
	// calculate the new right direction
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
	// calculate the new up direction
	in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}