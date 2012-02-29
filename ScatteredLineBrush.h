//
// ScatteredLineBrush.h
//
// The header file for Scattered Line Brush. 
//

#ifndef SCATTEREDLINEBRUSH_H
#define SCATTEREDLINEBRUSH_H

#include "impbrush.h"

class ScatteredLineBrush : public ImpBrush
{
public:
	ScatteredLineBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );
};

#endif
