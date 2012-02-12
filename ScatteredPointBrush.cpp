//
// ScatteredPointBrush.cpp
//
// The implementation of Scattered Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "scatteredpointbrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatteredPointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void ScatteredPointBrush::BrushMove( const Point source, const Point target )
{
	int* size = new int;
	glGetIntegerv(GL_POINT_SIZE, size);
	
	glPointSize( 1 ); // set size to 1

	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < *size; j++)
		{
			if (frand() > 0.15f) continue; // only keep some of the points
					
			Point s = Point(source, - *size / 2 + i, - *size / 2 + j);
			Point t = Point(target, - *size / 2 + i, - *size / 2 + j); // a the new point

			ImpBrush::c_pBrushes[BRUSH_POINTS]->BrushMove(s, t);
		}
	}

	glPointSize( (float)*size );
}

void ScatteredPointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

