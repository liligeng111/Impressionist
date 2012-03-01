//
// AlphaMappedBrush.h
//
// The header file for Alpha Mapped Brush. 
//

#ifndef ALPHAMAPPEDBRUSH_H
#define ALPHAMAPPEDBRUSH_H

#include "impbrush.h"

class AlphaMappedBrush : public ImpBrush
{
public:
	AlphaMappedBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const Point source, const Point target );
	void BrushMove( const Point source, const Point target );
	void BrushEnd( const Point source, const Point target );
	char* BrushName( void );

private:
	void SetAlphaColor (const Point source);
};

#endif
