#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include "stdafx.h"
#include <gl/GL.h>

namespace rgb {
	GLubyte red[];
	GLubyte lime[];
	GLubyte aqua[];
	GLubyte black[];
	GLubyte white[];
	GLubyte purple[];
	GLubyte green[];
}

namespace GL {
	void SetupOrtho();
	void RestoreGL();
	void DrawRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutLine(float x1, float y1, float x2, float y2, GLfloat thickness, const GLubyte color[3]);
	void DrawBox(float x1, float y1, float x2, float y2, GLfloat thickness, const GLubyte color[3]);
	void DrawCircle(int k, int h, int r, GLfloat thickness, GLubyte color[3]);
}
