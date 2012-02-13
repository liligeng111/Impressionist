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
static Point	coord;

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
	current_pic = -1;
	size_pic = 0;
	max_pic = 50;
	pics = new unsigned char*[max_pic];
	//savePic();
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

		
		// This is the event handler
		switch (eventToDo) 
		{

		default:
			printf("Unknown event!!\n");		
			break;
		case LEFT_MOUSE_DOWN:
			if (current_pic < size_pic)
			{				
				int n = m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3;
				m_pDoc->m_ucPainting = new unsigned char[n];
				for (int i = 0; i < n; i++)	
					m_pDoc->m_ucPainting[i] = pics[current_pic][i];
			}	
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
			
			glBegin( GL_LINES );
				glColor3f(0.8f, 0.05f, 0.1f);
				glVertex2d(start.x, start.y);
				glVertex2d(target.x, target.y);
			glEnd();

			break;
		case RIGHT_MOUSE_UP:
			RestoreContent();
			int d = sqrt((float)((start.x - target.x) * (start.x - target.x) + (start.y - target.y) * (start.x - target.y)));
			if (d > 40) d = 40;
			m_pDoc->setSize(d);

			if (target.x == start.x)
			{
				m_pDoc->setAngle(90);
				break;
			}

			m_pDoc->setAngle(180 - abs((atan2(float(target.y - start.y) , (target.x - start.x)))) * 180 / M_PI);

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
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		// update the cursor for OriginalView
		m_pDoc->m_pUI->m_origView->update_cursor(coord.x, m_nWindowHeight-coord.y);
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
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
		}
		size_pic = current_pic;
	}	
	size_pic++;

	if (current_pic == max_pic)
	{
		// max number of pics stored reached
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
	
	
	OutputDebugString("save: ");
	for (int i = 0; i < current_pic; i++)
		OutputDebugString("c");
	OutputDebugString("\n");
}

void PaintView::undo()
{ 
	if (current_pic <= 0) return; //nothing to undo
	current_pic--;
	OutputDebugString("undo: ");
	for (int i = 0; i < current_pic; i++)
		OutputDebugString("c");
	OutputDebugString("\n");

	m_pDoc->m_ucPainting = pics[current_pic];
	refresh();
}

void PaintView::redo()
{
	if (current_pic >= size_pic - 1) return; //nothing to redo
	current_pic++;
	OutputDebugString("redo: ");
	for (int i = 0; i < current_pic; i++)
		OutputDebugString("c");
	OutputDebugString("\n");
	m_pDoc->m_ucPainting = pics[current_pic];
	refresh();
}