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
	// init();
}

void PaintView::init()
{	
	if (pics)
	{
		for (int i = 0; i < current_pic; i++)
		{
			if (pics[i]) delete []pics[i];
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
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid())
	{

		glClearColor(0.9f, 0.9f, 0.9f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		glEnable( GL_BLEND ); //enable alpha

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();
	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );

		
		// temp 
		//int d;
		// This is the event handler
		switch (eventToDo) 
		{

		case LEFT_MOUSE_DOWN:
			creatPic();	
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			start = target;

			break;
		case RIGHT_MOUSE_DRAG:
			RestoreContent();
			glDrawBuffer(GL_BACK);
			glLineWidth(5);
			red_line = true;
			glBegin( GL_LINES );
				glColor3f(0.8f, 0.05f, 0.1f);
				glVertex2d(start.x, start.y);
				glVertex2d(target.x, target.y);
			glEnd();

			break;
		case RIGHT_MOUSE_UP:
			RestoreContent();
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

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		lastCoord = coord;
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
		{

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
		m_pDoc->m_pUI->m_origView->update_cursor(coord.x, m_nWindowHeight-coord.y);
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
		m_pDoc->m_pUI->m_origView->update_cursor(coord.x, m_nWindowHeight-coord.y);
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
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	if (red_line)
	{
		RestoreContent();
		return;
	}
	glReadBuffer(GL_FRONT);
	
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
	savePic();
}


void PaintView::RestoreContent()
{
	red_line = false;
	glDrawBuffer(GL_BACK);

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
	for (int i = 0; i < n; i++)	
		pics[current_pic][i] = m_pDoc->m_ucPainting[i];	
	
}

void PaintView::undo()
{ 
	if (current_pic <= 0) return; //nothing to undo
	current_pic--;
	m_pDoc->m_ucPainting = pics[current_pic];
	refresh();
}

void PaintView::redo()
{
	if (current_pic >= size_pic - 1) return; //nothing to redo
	current_pic++;
	m_pDoc->m_ucPainting = pics[current_pic];
	refresh();
}

unsigned char rgb2grayscale(GLubyte* color) {
	return (unsigned char)(color[0] * 0.3 + color[1] * 0.59 + color[2] * 0.11);
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

	/*
	char msg[255];
	sprintf(msg, "color : %uc, x: %d, y: %d\n",color[1][1], gxsum, gysum);
	OutputDebugString(msg);
	*/
	return (int)(atan2((double)gxsum, (double)gysum) * 180 / M_PI);
}

void PaintView::creatPic()
{
	if (current_pic < size_pic - 1)
	{				
		// happens then paint sth after severral undo;
		int n = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3;
		m_pDoc->m_ucPainting = new unsigned char[n];
		for (int i = 0; i < n; i++)	
			m_pDoc->m_ucPainting[i] = pics[current_pic][i];
	}

}

void PaintView::autoPaint() {
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	

	int spacing = m_pDoc->m_pUI->m_AutoPaintDistanceSlider->value();
	bool randp = m_pDoc->m_pUI->m_AutoPaintRandButton->value();
	int	size = m_pDoc->getSize();
	Point p;

	/*
	ImpBrush::c_pBrushes[0]->BrushMove(Point(300, 180), Point(100, 100));
	this->m_pCurrentBrush->BrushMove(Point(100, 100), Point(100, 100));
	this->m_pUI->m_paintView->refresh();
	*/



	for (int i = 0; i < this->m_pDoc->m_nPaintHeight; i += spacing) {
		for (int j = 0; j < this->m_pDoc->m_nPaintWidth; j+= spacing) {
			size = m_pDoc->getSize();			
			if (randp) {
				size = ( size + irand(20) - 10 + 39) % 40 + 1;
			}
			glPointSize((float)size);
			p.x = j; 
			p.y = i;
			this->m_pDoc->m_pCurrentBrush->BrushMove(p, p);
		}
	}
	SaveCurrentContent();
	glFlush();
	refresh();
	m_pDoc->m_pUI->m_origView->refresh();
}
