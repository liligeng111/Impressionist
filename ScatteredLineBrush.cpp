//
// ScatteredLineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "scatteredlinebrush.h"

extern float frand();

ScatteredLineBrush::ScatteredLineBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void ScatteredLineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void ScatteredLineBrush::BrushMove( const Point source, const Point target )
{
	int* size = new int;
	glGetIntegerv(GL_POINT_SIZE, size);

	for (int j = 0; j < 4; j++)
	{
		int x = (frand() - 0.5f) * *size; 
		int y = (frand() - 0.5f) * *size; // a random point round the target point
		
		Point s = Point(source, x, y);
		Point t = Point(target, x, y); // a random point around the original one

		ImpBrush::c_pBrushes[BRUSH_LINES]->BrushMove(s, t);
	}
}

void ScatteredLineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

