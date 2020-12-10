#include "CH16Demo.h"
#include "FixedFunctionPrimitives.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/ImGuizmo.h"
#include <iostream>
#include "ObjLoader.h"
#include "../Projects/ConstructiveSolidGeometry.h"

Mesh* sphere = new Mesh();
Mesh* movingSphere = new Mesh();
Mesh* cube = new Mesh();
Mesh* subtraction = new Mesh();

void CH16Demo::Initialize(int width, int height) {
	DemoBase::Initialize(width, height);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float val[] = { 0.5f, 1.0f, -1.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	size_imgui_window = true;

	camera.SetTarget(vec3(3.75622f, 2.98255f, 0.0f));
	camera.SetZoom(9.0f);
	camera.SetRotation(vec2(-67.9312f, 19.8f));

	LoadMesh("D:\\Meshes\\sphere.obj", sphere);
	LoadMesh("D:\\Meshes\\sphere.obj", movingSphere);
	LoadMesh("D:\\Meshes\\cube.obj", cube);


	ResetDemo();
}

float CH16Demo::Random(float min, float max) {
	if (max < min) {
		float t = min;
		min = max;
		max = t;
	}

	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

void CH16Demo::ResetDemo() {


}

void CH16Demo::ImGUI() {
	DemoBase::ImGUI();

}

void CH16Demo::Render() {
	DemoBase::Render();

	// Mesh bending is not implemented, so we render the 
	// obstacle which the cloth is resting on with a cheat

	static const float constraintDiffuse[]{ 0.0f, 200.0f / 255.0f, 0.0f, 0.0f };
	static const float constraintAmbient[]{ 50.0f / 255.0f, 200.0f / 255.0f, 50.0f / 255.0f, 0.0f };
	static const float groundDiffuse[]{ 0.0f, 0.0f, 200.0f / 255.0f, 0.0f };
	static const float groundAmbient[]{ 50.0f / 255.0f, 50.0f / 255.0f, 200.0f / 255.0f, 0.0f };
	static const float val[] = { 1.0f, 2.0f, 3.0f, 0.0f };
	static const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, val);

	glColor3f(constraintDiffuse[0], constraintDiffuse[1], constraintDiffuse[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, constraintAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, constraintDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

	if (subtraction->triangles != nullptr) {
		delete[] subtraction->triangles;
		delete subtraction;
	}

	subtraction = ConstructiveSolidGeometry().RelativeComplement(cube, movingSphere);

	::Render(*subtraction);

	glColor3f(groundDiffuse[0], groundDiffuse[1], groundDiffuse[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, groundAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, groundDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
	

	//physicsSystem.Render();
	
	/*Line l(vec3(0, -8, 0), vec3(0, 8, 0));
	::Render(l);*/
}

float t = 0;
void CH16Demo::Update(float dt) {
	DemoBase::Update(dt);
	t += dt;

	for (int i = 0; i < sphere->numTriangles * 3; i++) {
		movingSphere->vertices[i] = sphere->vertices[i] + Point(sinf(t / 10.0f) - 0.5, 0, 0);
	}

	physicsSystem.Update(dt);
}