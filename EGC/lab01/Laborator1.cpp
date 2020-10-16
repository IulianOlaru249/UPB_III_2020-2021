#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

/*	EXPLICATII
* 
*	Miscare pe axe: W - inainte, 
*					S - inapoi,
*					Z - in sus, 
*					C - in jos;
* 
*	Miscare pe diagonale: W + E - dreapta sus,
*						  W + Q - stanga sus;
* 
*	Schimbare background : F - galben,
*						   G - negru;
* 
*   Schimbare obiect : P;
* 
*	Miscare incrementala: Left Control + taste miscare pe axe;
* 
*/

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		Mesh* mesh2 = new Mesh("sphere");
		Mesh* mesh3 = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;
		meshes[mesh2->GetMeshID()] = mesh2;
		meshes[mesh3->GetMeshID()] = mesh3;
	}
}

void Laborator1::FrameStart()
{

}

bool f_pressed = false;
bool g_pressed = false;
bool p_pressed = true;
bool ctrl_pressed = false;
int i = 0;
float pos_x = 0;
float pos_y = 0;
float pos_z = 0.5;

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(0, 0, 0, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(pos_x, pos_z, pos_y), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0));

	// render the object again but with different properties
	RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0));

	if (g_pressed == true) {
		f_pressed = false;
		g_pressed = false;
	}

	if (f_pressed == true) {

		g_pressed = false;

		// sets the clear color for the color buffer
		glClearColor(1, 1, 0, 1);

		// clears the color buffer (using the previously set color) and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the object
		RenderMesh(meshes["box"], glm::vec3(pos_x, pos_z, pos_y), glm::vec3(0.5f));


		// render the object again but with different properties
		RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0));

		// render the object again but with different properties
		RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0));

	}


	if (i == 0) {
		// clears the color buffer (using the previously set color) and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the object
		RenderMesh(meshes["box"], glm::vec3(pos_x, pos_z, pos_y), glm::vec3(0.5f));

		// render the object again but with different properties
		RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0));

		// render the object again but with different properties
		RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0));
	}
	else if (i == 1) {
		// clears the color buffer (using the previously set color) and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the object again but with different properties
		RenderMesh(meshes["teapot"], glm::vec3(pos_x, pos_z, pos_y));

		// render the object again but with different properties
		RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0));

		// render the object again but with different properties
		RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0));
	}
	else if (i == 2) {
		// clears the color buffer (using the previously set color) and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the object again but with different properties
		RenderMesh(meshes["sphere"], glm::vec3(pos_x, pos_z, pos_y));

		// render the object again but with different properties
		RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0));

		// render the object again but with different properties
		RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0));
	}
}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input

	if (window->KeyHold(GLFW_KEY_LEFT_CONTROL)) {
		ctrl_pressed = true;
	}
	else {
		ctrl_pressed = false;
	}

	if (ctrl_pressed == false) {
		if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_E)) {
			pos_x += 0.01;
			pos_z += 0.01;
		}
		else if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_Q)) {
			pos_x -= 0.01;
			pos_z += 0.01;
		}
		else if (window->KeyHold(GLFW_KEY_W)) {
			pos_y -= 0.01;
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			pos_x -= 0.01;
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			pos_y += 0.01;
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			pos_x += 0.01;
		}

		if (window->KeyHold(GLFW_KEY_Z)) {
			pos_z += 0.01;
		}

		if (window->KeyHold(GLFW_KEY_C)) {
			pos_z -= 0.01;
		}
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		// do something
		f_pressed = true;
	}

	// add key press event
	if (key == GLFW_KEY_G) {
		// do something
		g_pressed = true;
	}

	if (key == GLFW_KEY_P) {
		// do something
		i++;
		if (i == 3) i = 0;
	}

	if (ctrl_pressed == true) {
		if (key == GLFW_KEY_W) {
			pos_y -= 0.01;
		}

		if (key == GLFW_KEY_A) {
			pos_x -= 0.01;
		}
		if (key == GLFW_KEY_S) {
			pos_y += 0.01;
		}
		if (key == GLFW_KEY_D) {
			pos_x += 0.01;
		}

		if (key == GLFW_KEY_Z) {
			pos_z += 0.01;
		}

		if (key == GLFW_KEY_C) {
			pos_z -= 0.01;
		}
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
