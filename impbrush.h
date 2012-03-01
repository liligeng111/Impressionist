#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// impBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
typedef enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	BRUSH_FILTER,
	BRUSH_ALPHA_MAPPED,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
} TYPE_BRUSH_TYPE;

typedef enum
{
	LDIRECTION_SLIDER_RIGHT_MOUSE = 0,
	LDIRECTION_GRADIENT,
	LDIRECTION_BRUSH_DIRECTION,
	NUM_LDIRECTION_TYPE
} TYPE_LINE_DIRECTION;

// for painterly style
typedef enum
{
	STYLE_IMPRESSIONIST = 0,
	STYLE_EXPRESSIONIST,
	NUM_PAINTERLY_STYLE
} TYPE_PAINTERLY_STYLE;

// this is certainly not the good place to put this defination for painterly style and brush
// but other enum types are here, 
// so I cannot help also putting it here at this dirty place
typedef enum
{
	PAINTERLY_BRUSH_CIRCLE = 0,
	PAINTERLY_BRUSH_LINE,
	PAINTERLY_BRUSH_CURVE,
	PAINTERLY_BRUSH_BSPLINE,
	NUM_PAINTERLY_BRUSH
} TYPE_PAINTERLY_BRUSH;

class ImpressionistDoc; // Pre-declaring class

class Point 
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };
	Point(const Point& p, int xx, int yy) { x = p.x + xx; y = p.y + yy; };

	int x, y;
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point source, const Point target ) = 0;
	virtual void BrushMove( const Point source, const Point target ) = 0;
	virtual void BrushEnd( const Point source, const Point target ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const Point source , GLubyte alpha = GLubyte(100));

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	// then why do you still include this stupid useless function?
	char* BrushName( void );
	
	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	char*				m_pBrushName;
};

#endif
