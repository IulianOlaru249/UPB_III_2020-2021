#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

/* Cubul din stanga va face o transatie intre doua limite
*  Cubul din centru va face o pulsatie, scaland in functie de un factor, intre 2 limite
*  Cubul din dreapta va face o miscare de rotire, inainte si inapoi intre doua limite pe axele Ox si Oy
*  Bonus: se considera pamantul centrul si luna aflata la distanta moonToEarth de el. Se genereaza toate 
*	punctele de pe conturul cercului cu centrul in centrul pamantului si raza moonToEarth. Asemanator
*	cu masinuta din lab-ul trecut. Se face apoi tranzlatia pe Ox si OZ cu distantele calculate.
*
*  Obs: cele 3 cuburi pot fi miscate si din taste, cum e mentionat in laborator. Trebuie 
    decomentat codul din input_update
*/

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

float moonToEarth = 1.5f;

float limit_left = -2;
float limit_right = 2;
float trans_fact = 1;

float scale_limit_left = 1.0f;
float scale_limit_right = 2.0f;
float puls_fact = 1;

float rot_limit_left = 0.0f;
float rot_limit_right = 4.0f;
float rot_fact = 1;

float moonScaleX = 0.25;
float moonScaleY = 0.25;
float moonScaleZ = 0.25;

float translateEarthX = 0;
float translateEarthY = 3.0f;
float translateEarthZ = 0;

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	// moon
	angularStepOXMoon = 0;
	angularStepOYMoon = 0;
	angularStepOZMoon = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-5.0f, 0.5f,-1.5f);


	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);

	/* Animation for 1st box */
	translateX += deltaTimeSeconds * trans_fact * 2;
	if (translateX >= limit_right || translateX <= limit_left)
		trans_fact *= -1;

	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);

	/* Animation for 2nd box */
	scaleX += deltaTimeSeconds * puls_fact;
	scaleY += deltaTimeSeconds * puls_fact;
	scaleZ += deltaTimeSeconds * puls_fact;
	if (scaleX >= scale_limit_right || scaleX <= scale_limit_left)
		puls_fact *= -1.0f;
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);

	/* Animation for 3rd box */
	angularStepOY += deltaTimeSeconds * rot_fact;
	if (angularStepOY >= rot_limit_right || angularStepOY <= rot_limit_left)
		rot_fact *= -1.0f;

	angularStepOX += deltaTimeSeconds * rot_fact;
	if (angularStepOX >= rot_limit_right || angularStepOX <= rot_limit_left)
		rot_fact *= -1.0f;
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


	/*Bonus*/

	/* Earth */
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(translateEarthX, translateEarthY, translateEarthZ);
	translateEarthX += deltaTimeSeconds * trans_fact * 1;
	if (translateEarthX >= limit_right || translateEarthX <= limit_left)
		trans_fact *= -1;
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	/*Moon*/
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate((translateEarthX + moonToEarth) - (moonToEarth - moonToEarth * cos(angularStepOXMoon)), translateEarthY, translateEarthZ + (moonToEarth * sin(angularStepOZMoon)));
	angularStepOXMoon += deltaTimeSeconds;
	angularStepOZMoon += deltaTimeSeconds;
	modelMatrix *= Transform3D::Scale(moonScaleX, moonScaleY, moonScaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// TODO
	/*
	if (window->KeyHold(GLFW_KEY_W)) {
		translateZ -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		translateZ += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		translateX -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		translateX += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_R)) {
		translateY += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_F)) {
		translateY -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += deltaTime * 2;
		scaleY += deltaTime * 2;
		scaleZ += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= deltaTime * 2;
		scaleZ -= deltaTime * 2;
		scaleY -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_3)) {
		angularStepOX -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_4)) {
		angularStepOX += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_5)) { 
		angularStepOY -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_6)) {
		angularStepOY += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_7)) {
		angularStepOZ -= deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_8)) {
		angularStepOZ += deltaTime * 2;
	}
	*/
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
