// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "images.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(const char *iname);			// called by the UI to load image
	int		saveImage(const char *iname);			// called by the UI to save image
	int		changeImage(const char *iname);			// called by the UI to change image
	int		loadAlphaImage(const char *iname);			// called by the UI to load alpha-mapped image
	int		loadAnotherImage(const char *iname);			// called by the UI to load another image
	void	make_dim(int alpha);
	int		createMosaic(const char** iname, const int count);			// called by the UI to create image mosaic

	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	setBrushType(int type);			// called by the UI to set the brushType
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getWidth();						// get the UI width
	//void	setWidth(int width);			// set the UI width
	int		getAngle();						// get the UI angle
	void	setAngle(int angle);			// set the UI angle
	float	getAlpha();						// get the UI alpha
	char*	getImageName();					// get the current image name


// Attributes
public:
	/// why all start with m_n ?

	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	/// bitmap image size 
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucEdge;
	unsigned char*	m_ucPainting;
	unsigned char*	m_ucDissolve;
	unsigned char*	m_ucAnother;
	unsigned char*	m_ucAlpha;
	unsigned char*	m_ucDim;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	GLubyte* GetAnotherPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	GLubyte* GetAlphaPixel( const Point p );  
	GLubyte* getPaintingPixelFromPics(int x, int y);
	bool isEdge(int x, int y);
	
	void dissolve_image(float alpha);
	void edge_view();

private:
	char	 m_imageName[256];
	GLubyte* UIColor;

};

extern void MessageBox(char *message);

#endif
