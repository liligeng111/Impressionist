//
// originalview.h
//
// The header file for original view of the input images
//

#ifndef ORIGINALVIEW_H
#define ORIGINALVIEW_H

#include <FL/Fl.H>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

#include "impbrush.h"

class ImpressionistDoc;

typedef enum
{
	ORIGIN_VIEW = 0,
	EDGE_VIEW,
	ANOTHER_VIEW,
	DISSOLVE_VIEW,
}  VIEW_TYPE;

class OriginalView : public Fl_Gl_Window
{
public:
	OriginalView(int x, int y, int w, int h, const char *l);
	
	void draw();
	void refresh();

	void update_cursor(int x, int y);

	void resizeWindow(int width, int height);

	ImpressionistDoc*	m_pDoc;

	void setView(int);

	bool refreshed;

private:
	int cursor_x, cursor_y;
	int	m_nWindowWidth, 
		m_nWindowHeight;
	int view;
	// coord for cursor in PaintView

};

#endif
