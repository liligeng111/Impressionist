//
// FilterBrush.cpp
//
// The implementation of Filter Brush. 
// It is a kind of ImpBrush. 
// but the behaviour is quite different from other ImpBrushes
//
// currently this is nothing but duplication of point brush
// but I will implement it so
//

#include "impressionistdoc.h"
#include "impressionistui.h"
#include "filterbrush.h"

extern float frand();

FilterBrush::FilterBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
	m_matrix = new int[5 * 5];
	memset(m_matrix, 0, 5 * 5 * sizeof(int));
	m_matrix[2 * 5 + 2] = 1;
	m_divisor = 1;
	m_offset = 0;
}

void FilterBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	BrushMove( source, target );
}

void FilterBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "FilterBrush::BrushMove document is NULL\n" );
		return;
	}

	dlg->get_filter_parameters(m_matrix, m_divisor, m_offset);
	int size = pDoc->getSize();
	filter_image(pDoc->m_ucPainting, pDoc->m_nWidth, pDoc->m_nHeight, target.x - size / 2, target.y - size / 2, size, size, m_matrix, m_divisor, m_offset, pDoc);
	dlg->m_paintView->RestoreContent();
}

void FilterBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void FilterBrush::filter_image(unsigned char * image, int width, int height, int x, int y, int dx, int dy, int* matrix, int divideBy, int offset, ImpressionistDoc* pDoc) {
	int desty = y + dy;
	int destx = x + dx;
	if (x < 0) x = 0; else if (x >= width) x = width - 1;
	if (destx < 0) destx = 0; else if (destx >= width) destx = width - 1;
	if (y < 0) y = 0; else if (y >= height) y = height - 1;
	if (desty < 0) desty = 0; else if (desty >= height) desty = height - 1;

	for (int i = y; i < desty; i++) {
		for (int j = x; j < destx; j++) {
			int colorsum[3] = {0, 0, 0};
			int pixelp = (i * width + j) * 3;
			for (int k = 0; k < 5; k++) {
				for (int t = 0; t < 5; t++) {
					GLubyte* color = pDoc->getPaintingPixelFromPics(j + t - 2, i - k + 2);
					colorsum[0] += color[0] * matrix[k * 5 + t];
					colorsum[1] += color[1] * matrix[k * 5 + t];
					colorsum[2] += color[2] * matrix[k * 5 + t];
				}
			}

			// avoid divide by 0
			if (divideBy == 0) divideBy = 1;
			int res;
			for (int shift = 0; shift < 3; shift++) {
				res = ((colorsum[shift] / divideBy) + offset);
				// to avoid ... (how to name it? )
				if (res < 0) res = 0;
				else if (res > 255) res = 255;
				image[pixelp + shift] = (GLubyte)res;
			}
		}
	}
}