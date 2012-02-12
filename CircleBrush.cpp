//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "circlebrush.h"
#include <math.h>


extern float frand();

CircleBrush::CircleBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void CircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void CircleBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	int* half_size = new int;
	glGetIntegerv(GL_POINT_SIZE, half_size);
	*half_size /= 2;

	glBegin( GL_POLYGON );
		SetColor( source );

		for (int i = 0; i < NUMBER_OF_EDGES_; i++)
			glVertex2d( target.x + *half_size * cos(2 * M_PI * i / NUMBER_OF_EDGES_), target.y + *half_size * sin(2 * M_PI * i / NUMBER_OF_EDGES_));

	glEnd();
}

void CircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

