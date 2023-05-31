#include "glDraw.h"

namespace rgb {
	GLubyte red[] = { 255, 0, 0 };
	GLubyte lime[] = { 0, 255, 0 };
	GLubyte aqua[] = { 0, 255, 255 };
	GLubyte black[] = { 0, 0, 0 };
	GLubyte white[] = { 255, 255, 255 };
	GLubyte purple[] = { 204, 0, 204 };
	GLubyte green[] = { 0, 255, 0};
}

void GL::SetupOrtho() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}


void GL::RestoreGL() {
	glPopMatrix();
	glPopAttrib();
}


void GL::DrawRect(float x, float y, float width, float height, const GLubyte color[3]) {
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y + height);
		glVertex2f(x, y + height);
	glEnd();
}

void GL::DrawOutLine(float x1, float y1, float x2, float y2, GLfloat thickness, const GLubyte color[3]) {
	
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glColor3ub(color[0], color[1], color[2]);
		glVertex2f(x1, y1); 
		glVertex2f(x2, y2);
	glEnd();
	
}

void GL::DrawBox(float x1, float y1, float x2, float y2, GLfloat thickness, const GLubyte color[3]) {
	
	glLineWidth(thickness);
	glBegin(GL_LINE_LOOP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x1-x2, y1+y2); // top left
	glVertex2f(x1+x2, y1+y2); // top right 
	glVertex2f(x1+x2, y1-y2); // bottom right
	glVertex2f(x1-x2, y1-y2); // bottom left
	glEnd();
}

void GL::DrawCircle(int k, int h, int r, GLfloat thickness, GLubyte color[3])
{
	GLfloat x, y, angle;
	glBegin(GL_POINTS);
	glColor3ub(color[0], color[1], color[2]);
	glLineWidth(thickness);
	float PI = 3.141592653589793;
	for (angle = 0.0f; angle <= (2.0f * PI); angle += 0.01f)
	{
		 x = r * sin(angle) + k;
		 y = r * cos(angle) + h;
		 glVertex2f(x, y);
	}
	glEnd();
}
