//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "scatteredcirclebrush.h"
#include <math.h>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatteredCircleBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void ScatteredCircleBrush::BrushMove( const Point source, const Point target )
{
	int* size = new int;
	glGetIntegerv(GL_POINT_SIZE, size);

	for (int j = 0; j < 3; j++)
	{
		int x = (frand() - 0.5f) * *size; 
		int y = (frand() - 0.5f) * *size; // a random point round the target point
		
		Point s = Point(source, x, y);
		Point t = Point(target, x, y); // a random point around the original one

		ImpBrush::c_pBrushes[BRUSH_CIRCLES]->BrushMove(s, t);
	}
}

void ScatteredCircleBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

