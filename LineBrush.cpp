//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "linebrush.h"
#include <math.h>

extern float frand();

LineBrush::LineBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize( (float)size );

	BrushMove( source, target );
}

void LineBrush::BrushMove( const Point source, const Point target )
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

	int width = ceil(pDoc->getWidth() / 2.0); //make sure it works when width is 1
	int angle = pDoc->getAngle();

	float m_sin = sin(2.0f * M_PI * angle / 360);
	float m_cos = cos(2.0f * M_PI * angle / 360);

	glBegin( GL_POLYGON );
		SetColor( source );
		
		if (true)
		{
			int nLength;
			int pLength;
			for (nLength = 0; nLength <= *half_size; nLength++)
			{
				if (pDoc->isEdge(target.x - (int)(nLength * m_cos), target.y - (int)(nLength * m_sin)))
				{
					break;
				}
			}

			for (pLength = 0; pLength <= *half_size; pLength++)
			{
				if (pDoc->isEdge(target.x + (int)(pLength * m_cos), target.y + (int)(pLength * m_sin)))
				{
					break;
				}
			}
			
			glVertex2d( target.x - nLength * m_cos - width * m_sin, target.y - nLength * m_sin + width * m_cos);
			glVertex2d( target.x + pLength * m_cos - width * m_sin, target.y + pLength * m_sin + width * m_cos);
			glVertex2d( target.x + pLength * m_cos + width * m_sin, target.y + pLength * m_sin - width * m_cos);
			glVertex2d( target.x - nLength * m_cos + width * m_sin, target.y - nLength * m_sin - width * m_cos);
		}
		else
		{
			glVertex2d( target.x - *half_size * m_cos - width * m_sin, target.y - *half_size * m_sin + width * m_cos);
			glVertex2d( target.x + *half_size * m_cos - width * m_sin, target.y + *half_size * m_sin + width * m_cos);
			glVertex2d( target.x + *half_size * m_cos + width * m_sin, target.y + *half_size * m_sin - width * m_cos);
			glVertex2d( target.x - *half_size * m_cos + width * m_sin, target.y - *half_size * m_sin - width * m_cos);
		}

	glEnd();
}

void LineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

