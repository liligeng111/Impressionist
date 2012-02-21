//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistdoc.h"
#include "impressionistui.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	cursor_x = -1;
	cursor_y = -1;
	view = 0;
}

void OriginalView::update_cursor(int x, int y) {
	cursor_x = x; cursor_y = y;
	refresh();
}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		/// what is depth ?
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();
	}

	glClear( GL_COLOR_BUFFER_BIT );

	unsigned char* to_load = 0;
	switch (view)
	{
	case EDGE_VIEW:
		to_load = m_pDoc->m_ucEdge;
		break;
	case ANOTHER_VIEW:
		to_load = m_pDoc->m_ucAnother;
		break;
	case DISSOLVE_VIEW:
		to_load = m_pDoc->m_ucDissolve;
		break;
	default:
		to_load = m_pDoc->m_ucBitmap;
		break;
	}

	if ( to_load ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
	// 	scrollpos.x = this->m_pDoc->m_pUI->wrapper_group_origiview->xposition();
	//	scrollpos.y = this->m_pDoc->m_pUI->wrapper_group_origiview->yposition();
		scrollpos.x = scrollpos.y = 0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int the_true_window_size = this->m_pDoc->m_pUI->wrapper_group_origiview->w();

		// debug
		char msg[222];
		sprintf(msg, "%d\n", the_true_window_size);
		OutputDebugString(msg);

		drawWidth = min(the_true_window_size, drawWidth);

		// then need to get scroll group width and height
		// int scroll_height = this->m_pDoc->m_pUI->wrapper_group_origiview->h();
		// int scroll_width = this->m_pDoc->m_pUI->wrapper_group_origiview->w();
		// drawWidth = min( drawWidth, scroll_width);
		// drawHeight = min( drawHeight, scroll_height);
		/*
		Cannot use FL_Window inside a scroll group, 
		The draw function of Fl_Window could draw outside of the scroll area
		so this is just failed attempt
		*/

		// drawWidth += scrollpos.x;

		// can set startrow, but now start column
		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;

	//	bitstart = to_load + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
	//	are you stupid to use scrollpos.x when it is always 0 ?
		bitstart = to_load + 3 * ((m_pDoc->m_nWidth * startrow));

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );

		glColor3f(0.9f, 0.05f, 0.1f);
		glBegin(GL_POLYGON);
		// to mock mouse cursor
		glVertex2f(cursor_x, cursor_y); 
		glVertex2f(cursor_x + 15, cursor_y - 7); 
		glVertex2f(cursor_x + 5, cursor_y - 15); 
		glEnd();
	}

	glFlush();
}

void OriginalView::setView(int n)
{
	view = n;
	if (n == DISSOLVE_VIEW) {
		this->m_pDoc->dissolve_image(this->m_pDoc->m_pUI->m_DissolveAlphaSlider->value());
	}
	refresh();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}

