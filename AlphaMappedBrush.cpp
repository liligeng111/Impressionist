//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "alphamappedbrush.h"

extern float frand();

AlphaMappedBrush::AlphaMappedBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void AlphaMappedBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize( (float)size );

	BrushMove( source, target );
}

void AlphaMappedBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	// debug
	/*
	char msg[355];
	sprintf(msg, "depth : %f \n\n", dlg->m_paintView->current_depth); 
	OutputDebugString(msg);
	*/

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	glBegin( GL_POINTS );
		SetAlphaColor( source );

		glVertex3f( target.x, target.y, dlg->m_paintView->current_depth);

	glEnd();
}

void AlphaMappedBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void AlphaMappedBrush::SetAlphaColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc->m_ucAlpha == 0)
	{
		SetColor(source);
		return;
	}
	GLubyte color[4];

	memcpy ( color, pDoc->GetAlphaPixel( source ), 4);

	for (int i = 0; i < 3; i++) {
		if (pDoc->m_pUI->m_ReverseColorButton->value()) color[i] = (GLubyte) (255 - color[i] * pDoc->m_pUI->blendColor[i]);
		else color[i] = (GLubyte) (color[i] * pDoc->m_pUI->blendColor[i]);
	}
 
	glColor4ubv( color );
}