#define WIN32_LEAN_AND_MEAN 
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>

#ifdef USE_GLEW
#include <GL\glew.h>
#include <GL\wglew.h>
#endif
#include <gl\GL.h>

#include "GLWindow.h"


#define Clamp01(x) \
	(x < 0.0f? 0.0f : (x > 1.0f? 1.0f : x))

#define Bit(number) (1 << number)
#define BitValue(variable, bitmask) (variable & bitmask)
#define BitOn(variable, bitmask) (variable |= bitmask)
#define BitOff(variable, bitmask) (variable &= ~bitmask)
#define BitToggle(variable, bitmask) (variable ^ bitmask)

GLWindow::GLWindow(const char* title, int width, int height) 
	: IWindow(title, width, height) {
	mouseButtonState = 0;
	memset(keyboardState, false, sizeof(bool) * 256);
	m_vecMousePos = vec2(0.0f, 0.0f);
	m_bDrawOrigin = true;
	m_nFOV = 60.0f;
	m_nNear = 0.01f;
	m_nFar = 1000.0f;
	matView = LookAt(vec3(5.0f, 5.0f, -5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

void GLWindow::SetPerspective(float fov, float zNear, float zFar) {
	m_nFOV = fov;
	m_nNear = zNear;
	m_nFar = zFar;

	OnResize(m_nWidth, m_nHeight);
}

void GLWindow::SetClearColor(float r, float g, float b) {
	r = Clamp01(r);
	b = Clamp01(b);
	g = Clamp01(g);
	glClearColor(r, g, b, 1.0f);

	SetInt("glMajor", 2);
	SetInt("glMinor", 1);
}

void GLWindow::SetDrawOrigin(bool value) {
	m_bDrawOrigin = value;
}

void GLWindow::OnInitialize() {
	glClearColor(0.5f, 0.6f, 0.7f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// TODO: Set Default GL States!
	OnResize(m_nWidth, m_nHeight);
}

void GLWindow::OnRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matView.asArray);

	if (m_bDrawOrigin) {
		bool isLit = glIsEnabled(GL_LIGHTING);
		if (isLit) {
			glDisable(GL_LIGHTING);
		}
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		if (isLit) {
			glEnable(GL_LIGHTING);
		}
	}
}

void GLWindow::OnResize(int width, int height) {
	glViewport(0, 0, width, height);
	float aspect = (float)width / (float)height;
	mat4 projection = Projection(m_nFOV, aspect, m_nNear, m_nFar);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.asArray);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matView.asArray);
}

void GLWindow::OnMouseMove(int x, int y) { 
	m_vecMousePos.x = x;
	m_vecMousePos.y = y;
}

void GLWindow::OnMouseDown(int mouseCode) { 
	BitOn(mouseButtonState, Bit(mouseCode));
}

void GLWindow::OnMouseUp(int mouseCode) {
	BitOff(mouseButtonState, Bit(mouseCode));
}

void GLWindow::OnKeyDown(int keyCode) { // TODO: THIS IS BUSTEDDDD!!!
	keyboardState[keyCode] = true;
}

void GLWindow::OnKeyUp(int keyCode) {
	keyboardState[keyCode] = false;
}

void GLWindow::SetView(const mat4& view) {
	matView = view;
}

mat4 GLWindow::GetView() {
	return matView;
}

vec2 GLWindow::GetMousePosition() {
	return m_vecMousePos;
}

bool GLWindow::MouseButonDown(int button) {
	if (button <= 0 || button >= 4) {
		return false;
	}

	return BitValue(mouseButtonState, Bit(button));
}

bool GLWindow::KeyDown(int key) {
	if (key < 0 || key >= 256) {
		return false;
	}
	return keyboardState[key];
}