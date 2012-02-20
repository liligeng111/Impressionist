// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistdoc.h"
#include "impressionistui.h"

#include "impbrush.h"

// Include individual brush headers here.
#include "pointbrush.h"
#include "linebrush.h"
#include "circlebrush.h"
#include "scatteredpointbrush.h"
#include "scatteredlinebrush.h"
#include "scatteredcirclebrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucEdge		= NULL;
	m_ucPainting	= NULL;
	m_ucDissolve	= NULL;
	m_ucAnother		= NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
	UIColor = new unsigned char[3];
	Fl::get_color(ui->m_mainWindow->color(), UIColor[0], UIColor[1], UIColor[2]);
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];

	if (type == BRUSH_LINES || type == BRUSH_SCATTERED_LINES)
	{
		m_pUI->m_BrushWidthSlider->activate(); // enable
		m_pUI->m_BrushAngleSlider->activate(); // enable
		m_pUI->m_LineDirectionChoice->activate();
	}
	else
	{
		m_pUI->m_BrushWidthSlider->deactivate(); // disable
		m_pUI->m_BrushAngleSlider->deactivate(); // disable
		m_pUI->m_LineDirectionChoice->deactivate();
	}
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Set the size of the brush.
//---------------------------------------------------------
void ImpressionistDoc::setSize(int size)
{
	return m_pUI->setSize(size);
}

//---------------------------------------------------------
// Set the size of the brush.
//---------------------------------------------------------
void ImpressionistDoc::setAngle(int angle)
{
	return m_pUI->setAngle(angle);
}

//---------------------------------------------------------
// Returns the width of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

//---------------------------------------------------------
// Returns the angle of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

//---------------------------------------------------------
// Returns the alpha of the brush.
//---------------------------------------------------------
float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}


//  this may not a good place to put resize function
//	what do you think of images.cpp ?
void resize_image(unsigned char* &source, int source_height, int source_width, unsigned char* &target, int target_height, int target_width)
{
	if(target) delete []target;
	target = new unsigned char[target_width * target_height * 3];
	float height_rate = 1.0f * source_height / target_height;
	float width_rate = 1.0f * source_width / target_width;
	for (int i = 0; i < target_height; i++)
	{
		int I = height_rate * i;
		for (int j = 0; j < target_width; j++)
		{
			target[3 * (i * target_width+ j)] = source[3 * (source_width * I + j * source_width / target_width)];
			target[3 * (i * target_width+ j) + 1] = source[3 * (source_width * I + j * source_width / target_width) + 1];
			target[3 * (i * target_width+ j) + 2] = source[3 * (source_width * I + j * source_width / target_width) + 2];
		}
	}
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------

int ImpressionistDoc::loadImage(const char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=load_image(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucEdge ) delete [] m_ucEdge;
	if ( m_ucPainting ) delete [] m_ucPainting;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	m_ucEdge	= new unsigned char [width*height*3];
	m_ucAnother	= new unsigned char [width*height*3];
	m_ucDissolve	= new unsigned char [width*height*3];
	m_pUI->m_origView->setView(0);
	// here we can see how the image is stored
	// by rows

	memset(m_ucPainting, 0, width*height*3);

	// enable save
	m_pUI->activeSaveFunc();

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);
								// 25 is for menubar 

	/*
	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();
	*/
	// new way
	m_pUI->resize_windows(width, height);
	m_pUI->m_paintView->init();

	return 1;
}
//---------------------------------------------------------
// load another image for various purpose
// This is called by the UI when the load another image button is pressed.
//---------------------------------------------------------

int ImpressionistDoc::loadAnotherImage(const char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;
	
	if ( (data=load_image(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	
	/*
	if (width != m_nPaintWidth || height != m_nPaintHeight)
	{
		//char msg[255];
		//sprintf(msg, "%d %d %d %d", width, height, m_nPaintWidth, m_nPaintHeight);

		fl_message("Two images are of different size");
		//fl_message(msg);
		//system("PAUSE");
		return 0;
	}
	*/

	// release old storage
	if ( m_ucAnother ) delete []m_ucAnother;
	m_ucAnother = 0;
	resize_image_bilinear(data, height, width, m_ucAnother, m_nHeight, m_nWidth);
	delete []data;
	data = 0;

	//m_pUI->m_paintView->init();
	return 1;
}

//---------------------------------------------------------
// Change to specified image
// This is called by the UI when the change image button is 
// pressed.
//---------------------------------------------------------

int ImpressionistDoc::changeImage(const char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;
	
	if ( (data=load_image(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// release old storage
	if ( m_ucBitmap ) delete []m_ucBitmap;
	m_ucBitmap = 0;
	resize_image(data, height, width, m_ucBitmap, m_nHeight, m_nWidth);
	delete []data;
	data = 0;
	//m_pUI->m_paintView->init();
	return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(const char *iname) 
{
	save_image(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);
	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}


//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}


GLubyte* ImpressionistDoc::getPaintingPixelFromPics(int x, int y) {
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nPaintWidth) 
		x = m_nPaintWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nPaintHeight ) 
		y = m_nPaintHeight-1;

	return (GLubyte*)(this->m_pUI->m_paintView->getPaintingFromPics() + 3 * (y* m_nPaintWidth + x));
}


void ImpressionistDoc::dissolve_image(float alpha)
{
	int n = m_nPaintWidth * m_nPaintHeight * 3;
	float beta = 1.0f - alpha;

	for (int i = 0; i < n; i++)
	{		
		m_ucDissolve[i] = m_ucBitmap[i] * beta + m_ucAnother[i] * alpha;
	}
}