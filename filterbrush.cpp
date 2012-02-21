//
// FilterBrush.cpp
//
// The implementation of Filter Brush. 
// It is a kind of ImpBrush. 
// but the behaviour is quite different from other ImpBrushes
//
// currently this is nothing but duplication of point brush
// but I will implement it so
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "filterbrush.h"

extern float frand();

FilterBrush::FilterBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void FilterBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize( (float)size );

	BrushMove( source, target );
}

void FilterBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "FilterBrush::BrushMove  document is NULL\n" );
		return;
	}

	glBegin( GL_POINTS );
		SetColor( source );

		glVertex2d( target.x, target.y );

	glEnd();
}

void FilterBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}
