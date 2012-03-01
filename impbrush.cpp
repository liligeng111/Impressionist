//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "impbrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source, GLubyte optional_alpha)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];

	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
	color[3] = static_cast<GLubyte>(255.0f * m_pDoc->getAlpha());

	// dirty patch
	// just bear it, this is life
	if (optional_alpha != 256) {
		color[3] = optional_alpha;
	}

	for (int i = 0; i < 3; i++) {
		if (m_pDoc->m_pUI->m_ReverseColorButton->value()) color[i] = (GLubyte) (255 - color[i] * m_pDoc->m_pUI->blendColor[i]);
		else color[i] = (GLubyte) (color[i] * m_pDoc->m_pUI->blendColor[i]);
	}
 
	glColor4ubv( color );
}
