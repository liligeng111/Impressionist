//
// FilterBrush.h
//
// The header file for Filter Brush. 
//

#ifndef FILTERBRUSH_H
#define FILTERBRUSH_H

#include "impbrush.h"

class FilterBrush : public ImpBrush
{
public:
	FilterBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
	static void filter_image(unsigned char * image, int width, int height, int x, int y, int dx, int dy, int* matrix, int divideBy, int offset, ImpressionistDoc* pDoc, int kernelsize = 5);

private:

	int* m_matrix;
	int m_divisor, m_offset;

};

#endif
