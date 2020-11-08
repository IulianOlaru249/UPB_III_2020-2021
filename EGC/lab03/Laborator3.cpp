#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

float squareSide = 100;
void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;

	//init car coords
	translateXCar = squareSide;
	translateYCar = squareSide;
	angularStepCar = 0;
	frontAngle = 0;
	

	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(square3);

	/* BONUS */
	Mesh* front = Object2D::CreateSquare("front", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(front);

	Mesh* body = Object2D::CreateSquare("body", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(body);
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

int clock = 0;
int factor = 1;
void Laborator3::Update(float deltaTimeSeconds)
{
	int window_width = (window->GetResolution())[0];
	// TODO: update steps for translation, rotation, scale, in order to create animations
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(0, 250);

	/*Compute scale */
	scaleX += deltaTimeSeconds * factor;
	scaleY += deltaTimeSeconds * factor;
	clock++;
	if (clock == 20) {
		clock = 0;
		factor *= -1;
	}

	/* Compute translation */
	angularStep += deltaTimeSeconds;

	/* Compute rotation */
	translateX += deltaTimeSeconds * 20;
	if (translateX > window_width)
		translateX = -squareSide;
	else
		translateX += deltaTimeSeconds * 100;

	// TODO: create animations by multiplying current transform matrix with matrices from Transform 2D
	modelMatrix *= Transform2D::Translate(translateX, translateY);
	modelMatrix *= Transform2D::Translate(squareSide / 2, squareSide / 2);
	modelMatrix *= Transform2D::Scale(scaleX, scaleY);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-squareSide / 2, -squareSide / 2);

	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(400, 250);

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	modelMatrix *= Transform2D::Translate( squareSide / 2, squareSide / 2);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-squareSide / 2, -squareSide / 2);
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(650, 250);

	//TODO create animations by multiplying current transform matrix with matrices from Transform 2D
	modelMatrix *= Transform2D::Translate(squareSide / 2, squareSide / 2);
	modelMatrix *= Transform2D::Scale(scaleX, scaleY);
	modelMatrix *= Transform2D::Translate(-squareSide / 2, -squareSide / 2);

	RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix); 

	/* BONUS */
	modelMatrixFront = glm::mat3(1);
	modelMatrixBody = glm::mat3(1);

	/*Move car*/
	modelMatrixBody *= Transform2D::Translate(translateXCar, translateYCar);
	modelMatrixFront *= Transform2D::Translate((translateXCar + squareSide) - (squareSide - cos(angularStepCar) * squareSide), translateYCar + squareSide * sin(angularStepCar));

	/*Rotate car*/
	modelMatrixBody *= Transform2D::Translate(squareSide / 2, squareSide / 2);
	modelMatrixFront *= Transform2D::Translate(squareSide / 2, squareSide / 2);
	modelMatrixBody *= Transform2D::Rotate(angularStepCar);
	modelMatrixFront *= Transform2D::Rotate(angularStepCar);
	modelMatrixBody *= Transform2D::Translate(-squareSide / 2, -squareSide / 2);
	modelMatrixFront *= Transform2D::Translate(-squareSide / 2, -squareSide / 2);

	RenderMesh2D(meshes["front"], shaders["VertexColor"], modelMatrixFront);
	RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrixBody);
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		int distance = deltaTime * 200;
		translateXCar += distance * cos(angularStepCar);
		translateYCar += distance * sin(angularStepCar);
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		int distance = deltaTime * 200;
		translateXCar -= distance * cos(angularStepCar);
		translateYCar -= distance * sin(angularStepCar);
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		angularStepCar += deltaTime * 2;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		angularStepCar -= deltaTime * 2;
	}
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
