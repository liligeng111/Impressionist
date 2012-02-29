//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

class ImpressionistDoc;

struct PainterlyParameter {
	int brushsize;
	int widht, height;
	int gridsize;
};


class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

	int getGradient();
	int getBrushDirection();

	void undo();

	void redo();

	void init();

	void creatPic(); // creat another copy for the painting
	void savePic();  // save current shown pic (m_ucPainting) to pics list
	// need to use this in filter, so that filter effect can be undo

	ImpressionistDoc *m_pDoc;

	void autoPaint();

	// current or last coord of the mouse cursor
	static Point coord;
	static Point lastCoord;
	float current_depth;

	GLubyte* getPaintingFromPics();

	static unsigned char rgb2grayscale(GLubyte* color);

	void painterly_paint();
	void painterly_paint_layer(unsigned char* canvas, unsigned char* reference, int* difference, PainterlyParameter* param);
	void make_blurcopy(unsigned char* image, unsigned char* reference, int size, int w, int h);
	void make_difference(unsigned char* imagea, unsigned char* imageb, int* difference, int width, int height);


private:
	GLvoid* m_pPaintBitstart;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;

	unsigned char** pics;
	int current_pic; // the current pic
	int size_pic; //number of pics in the buffer
	int max_pic; //the max number of picture buffered
	
	bool red_line; // if a read line is drawn 
	Point start;

	void dim_image_to_paint_view();
};

#endif