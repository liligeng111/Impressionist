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

};

#endif
