//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistdoc.h"
#include "impressionistui.h"
#include "paintview.h"
#include "impbrush.h"
#include <math.h>

#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
Point PaintView::coord;
Point PaintView::lastCoord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	start = Point(0 ,0);
	pics = 0;
	max_pic = 50;
	current_depth = 0;
	// The default mode is FL_RGB|FL_DOUBLE|FL_DEPTH.
	// this is good enought
	// this->mode(FL_DOUBLE | FL_DEPTH);

	// why this line is commented ?
	// debug here !! needed
	// init();
}

void PaintView::init()
{	
	if (pics)
	{
		for (int i = 0; i < max_pic; i++)
		{
			if (pics[i])
			{
			delete []pics[i];
			pics[i] = 0;
			}
		}
		delete []pics;
		pics = 0;
	}
	red_line = false;
	current_pic = -1;
	size_pic = 0;
	pics = new unsigned char*[max_pic];
	for (int i = 0; i < max_pic; i++)
		pics[i] = 0;
	savePic();
}

void PaintView::draw()
{
	if (this->m_pDoc->m_ucPainting) {
		// RestoreContent();
	}

	// glGetIntegerv(GL_AUX_BUFFERS, &num);
	// we have 4 auxiliary buffers

	// #ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	// #endif // !MESA
	/* Windows platform cannot possibly use this Open Source Implementation of OpenGL
	So, let's assume this function is always called */

	if(!valid())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glClearDepth(1.0f); // from 0 to 1
		// measured as the distance to the eye?
		// so 0.0 means nearest to the eye

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND); 
		//enable alpha and depth test

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		glDepthFunc(GL_LEQUAL);
		// default GL_LESS measures the distance to your eys
		// objects with less distance will be drawn
		// in this way, we can assign a random float in [0, 1] to every stroke
		// so the auto painted strokes will overlap each other, 
		// but everything else is not affected

		ortho();
		// what does this do?

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// clear both buffers

		// test
		/*
		// not clear about direction of z-axis
		// in this way, it should be from nearest to farthest
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		// internally all vertex are saved as float numbers
			glVertex3f(100, 100, 1.0);
			glVertex3f(150, 100, 1.0);
			glVertex3f(100, 150, 1.0);
			// not surprisingly, the range of z-axis is [-1, 1]
		glEnd();
		*/
	}

	// this refers to the FL_Gl_Window, 
	// since this Gl_Window is not resizable
	// the value is almost fixed
	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	// to ensure that drawBuffer doesn't draw outside the border
	// but draw outside is not possible anyway,
	// window manager will not allow it.
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );
	// should note that, in this certain implementation,
	// PaintDimension is alway equal to WindowDimension
	// window is not resizable, so it paint

	// so startrow is also alway 0
	int startrow = m_pDoc->m_nPaintHeight - (drawHeight);
	// lower left is the origin
	// start column is alway 0
	if ( startrow < 0 ) startrow = 0;

	// paintbitstart is always m_ucPainting
	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow));

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;
	// why do you always do these meaningless assignment

	// the two points of the actually drawn image
	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= 0;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting ) {
		if ( !isAnEvent) {
			// if no event happen, simply copy the RAM buffer to GL buffer
			RestoreContent();
		} else  {
			// isAnEvent
		// so it could only handle one event for each event loop iteration
		// but possible capture more than this handled event, anyway..
		// Clear it after processing.
			isAnEvent	= 0;	

			// transform FLTK coordinate to OpenGL coordinate
			Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
			Point target( coord.x, m_nWindowHeight - coord.y );

			// every event correspond to one swap buffer
			switch (eventToDo) 
			{
				// show dimmed version is very costly
			case LEFT_MOUSE_DOWN:
				RestoreContent();
				m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
				SaveCurrentContent();
				break;
			case LEFT_MOUSE_DRAG:
				RestoreContent();
				m_pDoc->m_pCurrentBrush->BrushMove( source, target );
				SaveCurrentContent();
				break;
			case LEFT_MOUSE_UP:
				RestoreContent();
				m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

				/*
				no need to consider these draws with swap buffer
				both front and end are drawed to
				even drawing lines is single action
				*/

				// save back buffer to RAM buffer
				SaveCurrentContent();
				savePic();
				// then copy RAM buffer to back buffer ?
				// RestoreContent();
				// and after this draw(), front and back will swap
				// this means, RestoreContent() is useless here
				break;
			
			// below is trick part
			case RIGHT_MOUSE_DOWN:
				start = target;
				break;
			case RIGHT_MOUSE_DRAG:
				RestoreContent();
				// get fresh new image from RAM buffer to back buffer
				glDrawBuffer(GL_BACK);
				// this is useless, RestoreContent() has done it

				glLineWidth(5);
				red_line = true;
				glBegin( GL_LINES );
					glColor3f(0.8f, 0.05f, 0.1f);
					glVertex2d(start.x, start.y);
					glVertex2d(target.x, target.y);
				glEnd();
				// draw the line to the back buffer, which will be seen after swapping buffers
				break;
			case RIGHT_MOUSE_UP:
				RestoreContent();
				// to erase the red line
				// the red line cannot be in the backbuffer

				//d = sqrt((float)((start.x - target.x) * (start.x - target.x) + (start.y - target.y) * (start.x - target.y)));
				//if (d > 40) d = 40;
				//m_pDoc->setSize(d);

				if (target.x == start.x)
				{
					m_pDoc->setAngle(90);
					break;
				}

				m_pDoc->setAngle(180 - abs((atan2(float(target.y - start.y) , (target.x - start.x)))) * 180 / M_PI);

				break;
			default:
				printf("Unknown event!!\n");		
				break;
			}
		}
		dim_image_to_paint_view();
	}
	glFlush();
}

void PaintView::dim_image_to_paint_view() {
	glDrawBuffer(GL_BACK);

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGBA, 
				  GL_UNSIGNED_BYTE, 
				  this->m_pDoc->m_ucDim);

}


int PaintView::handle(int event)
{
	// this only deals with events
	// no manipulation of buffers here
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		lastCoord = coord;
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1) {
			eventToDo=RIGHT_MOUSE_DOWN;
		} else {
			eventToDo=LEFT_MOUSE_DOWN;
		}
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		lastCoord = coord;
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		// update the cursor for OriginalView
		m_pDoc->m_pUI->m_origView->update_cursor(coord.x, this->m_pDoc->m_pUI->m_origView->h() - coord.y);
		if (Fl::event_button() == 2 || Fl::event_button() == 3)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		lastCoord = coord;
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
		{
			eventToDo=LEFT_MOUSE_UP;
		}
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		lastCoord = coord;
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		// update the cursor for OriginalView
		m_pDoc->m_pUI->m_origView->update_cursor(coord.x, this->m_pDoc->m_pUI->m_origView->h() - coord.y);
		break;
	case FL_LEAVE:
		this->m_pDoc->m_pUI->m_origView->update_cursor(-1, -1);
		break;
	default:
		return 0;
		break;
	}

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// save what is being display to buffer in RAM

	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	if (red_line)
	{
		RestoreContent();
		return;
	}

	// try
	// glReadBuffer(GL_FRONT);
	glReadBuffer(GL_BACK);
	// if everything is okay after this change, 
	// then show dimmed version is easy..?
	// consider, everytime before drawing to the back buffer
	// and the behaviour of swapping buffers
	
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}


void PaintView::RestoreContent()
{
	red_line = false;
	glDrawBuffer(GL_BACK);
	// draw what is in RAM buffer to the backbuffer

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}


void PaintView::savePic()
{	
	current_pic++;
	if (current_pic < size_pic)
	{
		// happens when do something after several undo
		for (int i = current_pic; i < size_pic; i++)
		{
			delete []pics[i]; //delete the useless pics
			pics[i] = 0;
		}
		size_pic = current_pic;
	}	
	size_pic++;

	if (current_pic == max_pic)
	{
		// max number of pics stored reached
		delete []pics[0];
		for (int i = 0; i < size_pic - 1; i++)
		{
			pics[i] = pics[i + 1];
		}
		current_pic--;
		size_pic--;
	}

	int n = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3;
	pics[current_pic] = new unsigned char[n];
	// use memcpy
	memcpy(pics[current_pic], m_pDoc->m_ucPainting, n);
	/*
	for (int i = 0; i < n; i++)	
		pics[current_pic][i] = m_pDoc->m_ucPainting[i];	
	*/
	
}

void PaintView::undo()
{ 
	if (current_pic <= 0) return; //nothing to undo
	current_pic--;
	int n = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3;
	// use memcpy
	memcpy(m_pDoc->m_ucPainting, pics[current_pic], n);
	refresh();
}

void PaintView::redo()
{
	if (current_pic >= size_pic - 1) return; //nothing to redo
	current_pic++;
	int n = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3;
	// use memcpy
	memcpy(m_pDoc->m_ucPainting, pics[current_pic], n);
	refresh();
}

unsigned char PaintView::rgb2grayscale(GLubyte* color) {
	return (unsigned char)(color[0] * 0.299 + color[1] * 0.587 + color[2] * 0.114);
}

int PaintView::getBrushDirection() {
	return (int) (atan2((double)(lastCoord.y - coord.y), (double)(lastCoord.x - coord.x)) * 180 / M_PI);
}

int PaintView::getGradient() {
	// more general functions should be better
	// but currently let's just implement one by one
	static const char gx[][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	static const char gy[][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
	int gxsum = 0, gysum = 0;
	// larget convolution size ?
	unsigned char color[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			color[i][j] = rgb2grayscale(this->m_pDoc->GetOriginalPixel(coord.x - 1 + j, coord.y - 1 + i));
			gxsum += color[i][j] * gx[i][j];
			gysum += color[i][j] * gy[i][j];
		}
	}

	return (int)(atan2((double)gxsum, (double)gysum) * 180 / M_PI);
}

/*void PaintView::creatPic()
{
	if (current_pic < size_pic - 1)
	{				
		// happens then paint sth after severral undo;
		int n = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3;
		// don't we need garbege collection here?
		// let me do it!
		if (!m_pDoc->m_ucPainting) {
			delete []m_pDoc->m_ucPainting;
		}

		m_pDoc->m_ucPainting = new unsigned char[n];

		// why not use memcpy here?
		/*
		for (int i = 0; i < n; i++)	
			m_pDoc->m_ucPainting[i] = pics[current_pic][i];
		*/
		/*memcpy(m_pDoc->m_ucPainting, pics[current_pic], n);
	}

}*/

void PaintView::autoPaint() 
{
	// this is the magic
	// gei hengye guile
	this->make_current();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA
	
	int spacing = m_pDoc->m_pUI->m_AutoPaintDistanceSlider->value();
	bool randp = m_pDoc->m_pUI->m_AutoPaintRandButton->value();
	int	size = m_pDoc->getSize();
	Point p;

	const int y = m_pDoc->m_nPaintHeight / spacing + 1;
	const int x = m_pDoc->m_nPaintWidth / spacing + 1;

	glClearDepth(0.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_GEQUAL);
	// Oh, please geng ge, I'm not sure why it works.
	// but these three lines are important !
	// more debug needed
	for (int i = 0; i < x; i++)
	{	
		for (int j = 0; j < y; j++) 
		{
			this->current_depth = frand();
			coord.x = i * spacing;
			coord.y = m_pDoc->m_nPaintHeight - j * spacing;
			size = m_pDoc->getSize();       
			if (randp) 
			{ 
				size = size * (0.75 + frand() / 2);
			}
			glPointSize((float)size); 
			p.x = i * spacing;
			p.y = j * spacing; 
			m_pDoc->m_pCurrentBrush->BrushMove(p, p); 
		}
		glFlush();
	}

	refresh();
	SaveCurrentContent();
	// savePic();
	this->current_depth = 0;

	#ifndef MESA
	// To avoid flicker on some machines.
	// glDrawBuffer(GL_BACK);
	#endif // !MESA
	// this->swap_buffers();
}

GLubyte* PaintView::getPaintingFromPics()
{
	return this->pics[this->current_pic];
}


//---------------------------------------------------------
// make painterly style image
// This is called by the callbacks in UI
//---------------------------------------------------------

void PaintView::make_blurcopy(unsigned char* image, unsigned char* reference, int brushsize, int w, int h) {
	// gaussian blurring according to the brushsize 
	int kernelsize = 0;
	int *matrix = new int[kernelsize * kernelsize];
	int divideBy = 0;
	// okay, I decided not to reuse the code in filterbrush
	// but rather, copy them here.

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int colorsum[3] = {0, 0, 0};
			int pixelp = (i * w + j) * 3;
			int half_kernel_size = (int)(kernelsize / 2);
			for (int k = 0; k < kernelsize; k++) {
				for (int t = 0; t < kernelsize; t++) {
					GLubyte* color = image + 3 * ((j + t - half_kernel_size) * w + (i - k + half_kernel_size));
					colorsum[0] += color[0] * matrix[k * kernelsize + t];
					colorsum[1] += color[1] * matrix[k * kernelsize + t];
					colorsum[2] += color[2] * matrix[k * kernelsize + t];
				}
			}

			// avoid divide by 0
			if (divideBy == 0) divideBy = 1;
			int res;
			for (int shift = 0; shift < 3; shift++) {
				res = (colorsum[shift] / divideBy);
				if (res < 0) res = 0;
				else if (res > 255) res = 255;
				blurred[pixelp + shift] = (GLubyte)res;
			}
		}
	}
}

void PaintView::make_difference(unsigned char* a, unsigned char* b, int* dif, int w, int h) {
	int pp;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			pp = i * w + j;
			dif[pp] = 0;
			for (int k = 0; k < 3; k++) {
				dif[pp] += (a[3 * pp + k] - b[3 * pp + k]) * (a[3 * pp + k] - b[3 * pp + k]);
			}
			dif[pp] = sqrt((double)dif[pp]);
		}
	}
}

void PaintView::painterly_paint() {
	// this should paint directly to the paint view
	// first clear all existed painting
	ImpressionistDoc* pUI = this->m_pDoc->m_pUI;
	int width = this->m_pDoc->m_nWidth;
	int height = this->m_pDoc->m_nHeight;
	unsigned char* canvas = this->m_pDoc->m_ucPainting;
	unsigned char* reference = new unsigned char [width * height * 3];
	int* difference = new int [width * height];

	if (canvas == NULL) {
		canvas = new unsigned char [width * height * 3];
	}

	memset(canvas, -1, width * height * 3);
	// now all white .. 

	// get all parameters here
	int threshold, maxbrush, minbrush, layer, step, maxstroke, minstroke;
	float curvature, blur, alpha;
	PainterlyParameter param;
	// boring and tedious coding, here, to be implemented when I'm sleeping
	//
	//

	int brushSize;
	for (brushSize = maxbrush; brushSize >= minbrush; brushSize -= step) {
		reference = make_blurcopy(canvas, brushSize, width, height);
		difference = make_difference(canvas, reference, width, height);
		painterly_paint_layer(canvas, reference, difference, &param);
	}

	// save current pic to stack so as to enable undo
	savePic();

	// clean ups
	if (reference) { delete [] reference; }
	if (difference) { delete [] difference; }
}

void PaintView::painterly_paint_layer(unsigned char* canvas, unsigned char* reference, int* difference, PainterlyParameter * param) {
	// painting to canvas with reference as reference
	// more parameters needed 
	make_current();
	RestoreContent();
	// all the OpenGL calls should be made here

	for (int i = 0; i < param->height; i += param->gridsize) {
		for (int j = 0; j < param->width; j += param->gridsize) {
			this->current_depth = frand(); // random every stroke
		}
	}
	SaveCurrentContent();
}