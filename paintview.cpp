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
	// for debug
	// system("PAUSE");

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
	return (unsigned char)(color[0] * 0.299 + color[1] * 0.587 + color[2] * 0.144);
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

	int max_preority = 70;
	const int y = m_pDoc->m_nPaintHeight / spacing + 1;
	const int x = m_pDoc->m_nPaintWidth / spacing + 1;

	char** preority;
	// notes to geng ge, 
	// as priority is 70 at most, should better make it char** or something to save memory
	preority = new char*[x];
	for (int i = 0; i < x; i++)
	{
		preority[i] = new char[y]; //preority to indicate layer
	}
	// should delete priority after using it


	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++) 
		{
			preority[i][j] = irand(max_preority); //asign each point a random preority
		}
	}

	for (int l = 0; l < max_preority; l++)
	{   
		//draw all points accoring to their preority
		for (int i = 0; i < x; i++)
		{	
			for (int j = 0; j < y; j++) 
			{
				if (preority[i][j] == l)
				{
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
			}
			glFlush();
		}
	}

	m_pDoc->m_pUI->m_origView->refresh();
	refresh();
	SaveCurrentContent();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

	// delete priority
	for (int i = 0; i < x; i++) {
		delete [] preority[i];
	}
	delete [] preority;
}

GLubyte* PaintView::getPaintingFromPics()
{
	return this->pics[this->current_pic];
}
