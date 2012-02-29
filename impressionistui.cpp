//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <math.h>
#include <stdio.h>

#include "impressionistui.h"
#include "impressionistdoc.h"
#include "filterbrush.h"


/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
	m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
	mySlider->labelfont(FL_COURIER);
	mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

	//---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	const char* newfile;
	// this will get the file path relative to the application itself
	Fl_Native_File_Chooser *chooser = new Fl_Native_File_Chooser();
	chooser->type(Fl_Native_File_Chooser::BROWSE_FILE);   // let user browse a single file
	chooser->title("Open an image file");                        // optional title
	chooser->directory(".");
	chooser->filter("RGB Image Files\t*.{bmp,png,jpg,jpeg}");                 // optional filter
	switch ( chooser->show() ) {
		case -1:    // ERROR
			fprintf(stderr, "*** ERROR show() failed:%s\n", chooser->errmsg());
			break;
		case 1:     // CANCEL
			fprintf(stderr, "*** CANCEL\n");
			break;
		default:    // USER PICKED A FILE
			newfile = chooser->filename();
			fprintf(stderr, "Filename was '%s'\n", newfile);
			pDoc->loadImage(newfile);
			break;
	}
	/*
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
	*/
}

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the change image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_change_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	const char* newfile;
	// this will get the file path relative to the application itself
	Fl_Native_File_Chooser *chooser = new Fl_Native_File_Chooser();
	chooser->type(Fl_Native_File_Chooser::BROWSE_FILE);   // let user browse a single file
	chooser->title("Open an image file");                        // optional title
	chooser->directory(".");
	chooser->filter("RGB Image Files\t*.{bmp,png,jpg,jpeg}");                 // optional filter
	switch ( chooser->show() ) {
		case -1:    // ERROR
			fprintf(stderr, "*** ERROR show() failed:%s\n", chooser->errmsg());
			break;
		case 1:     // CANCEL
			fprintf(stderr, "*** CANCEL\n");
			break;
		default:    // USER PICKED A FILE
			newfile = chooser->filename();
			fprintf(stderr, "Filename was '%s'\n", newfile);
			pDoc->changeImage(newfile);
			break;
	}
}

// load another image
void ImpressionistUI::cb_another_image(Fl_Menu_* o, void* v)  {
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	const char* newfile;
	// this will get the file path relative to the application itself
	Fl_Native_File_Chooser *chooser = new Fl_Native_File_Chooser();
	chooser->type(Fl_Native_File_Chooser::BROWSE_FILE);   // let user browse a single file
	chooser->title("Load another image file");                        // optional title
	chooser->directory(".");
	chooser->filter("RGB Image Files\t*.{bmp,png,jpg,jpeg}");                 // optional filter
	switch ( chooser->show() ) {
		case -1:    // ERROR
			fprintf(stderr, "*** ERROR show() failed:%s\n", chooser->errmsg());
			break;
		case 1:     // CANCEL
			fprintf(stderr, "*** CANCEL\n");
			break;
		default:    // USER PICKED A FILE
			newfile = chooser->filename();
			fprintf(stderr, "Filename was '%s'\n", newfile);
			pDoc->loadAnotherImage(newfile);
			break;
	}
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	const char* newfile = NULL;
	Fl_Native_File_Chooser *chooser = new Fl_Native_File_Chooser();
	chooser->type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);   // let user browse a single file
	chooser->title("Save image");                        // optional title
	chooser->directory(".");
	chooser->preset_file("untitled.bmp");
	chooser->filter("RGB Image Files\t*.bmp");                 // optional filter
	switch ( chooser->show() ) {
		case -1:    // ERROR
			fprintf(stderr, "*** ERROR show() failed:%s\n", chooser->errmsg());
			break;
		case 1:     // CANCEL
			fprintf(stderr, "*** CANCEL\n");
			break;
		default:    // USER PICKED A FILE
			newfile = chooser->filename();
			fprintf(stderr, "Filename was '%s'\n", newfile);
			break;
	}

	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_FilterDialog->hide();
	whoami(o)->m_brushDialog->hide();
	whoami(o)->m_mainWindow->hide();
	// remember to add more hide() functions if there are

	// what not directly exit the program?
	exit(0);
}

//-------------------------------------------------------------
// Undo 
//-------------------------------------------------------------
void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_paintView->undo();
}

//-------------------------------------------------------------
// Redo 
//-------------------------------------------------------------
void ImpressionistUI::cb_redo(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_paintView->redo();
}


//-------------------------------------------------------------
// Callbacks for display menu 
//-------------------------------------------------------------
void ImpressionistUI::cb_show_edge_image(Fl_Menu_ *o, void* v) {
	ImpressionistUI *ui = whoami(o);
	ui->m_origView->setView(EDGE_VIEW);
}
void ImpressionistUI::cb_show_original_image(Fl_Menu_ *o, void* v) {
	ImpressionistUI *ui = whoami(o);
	ui->m_origView->setView(ORIGIN_VIEW);
}
void ImpressionistUI::cb_show_another_image(Fl_Menu_ *o, void* v) {
	ImpressionistUI *ui = whoami(o);
	ui->m_origView->setView(ANOTHER_VIEW);
}
void ImpressionistUI::cb_show_dissolve_image(Fl_Menu_ *o, void* v) {
	ImpressionistUI *ui = whoami(o);
	ui->m_origView->setView(DISSOLVE_VIEW);
}

void ImpressionistUI::cb_dissolve_one_to_another(Fl_Menu_ *o, void* v) {
	ImpressionistUI *ui = whoami(o);
	ImpressionistDoc* pDoc = ui->m_pDoc;
	if ( !(pDoc->m_ucAnother &&  pDoc->m_ucBitmap)) {
		fl_message("Please load \"Another Image\" first !\n");
		return;
	}

	ui->cb_show_dissolve_image(o, v);

	int width = pDoc->m_nWidth;
	int height = pDoc->m_nHeight;
	GLubyte color[3];

	if (pDoc->m_ucDissolve == NULL) 
		pDoc->m_ucDissolve = new unsigned char [width * height *3];

	int total_time = 3;
	int fps = 30;
	int nframe = total_time * fps;
	int pixelp;

	for (int i = 0; i < nframe; i++) {

		for (int w = 0; w < width; w++) {
			for (int h = 0; h < height; h++) {
				pixelp = 3 * (h * width + w);
				for (int k = 0; k < 3; k++) {
					color[k] = (GLubyte) (
						(pDoc->m_ucBitmap[pixelp + k] * (nframe - i) 
							+ pDoc->m_ucAnother[pixelp + k] * (i)) / nframe );
				}
				memcpy(pDoc->m_ucDissolve + pixelp, color, 3);
			}
		}

		ui->m_origView->refreshed = false;
		ui->m_origView->refresh();
		while(!ui->m_origView->refreshed)
			ui->m_origView->refresh();
		// why refresh doesn't work ?
		// this sleep will cause all the program to suspend for this long time..?
	}
}

void ImpressionistUI::cb_dissolve_another_to_one(Fl_Menu_ *o, void* v) {
	ImpressionistUI *ui = whoami(o);
	ImpressionistDoc* pDoc = ui->m_pDoc;
	unsigned char* temp;
	// swap bitmap and another
	temp = pDoc->m_ucBitmap; pDoc->m_ucBitmap = pDoc->m_ucAnother; pDoc->m_ucAnother = temp;

	ui->cb_dissolve_one_to_another(o, v);

	// swap back
	temp = pDoc->m_ucBitmap; pDoc->m_ucBitmap = pDoc->m_ucAnother; pDoc->m_ucAnother = temp;
}

// callback for swapping the two images
void ImpressionistUI::cb_swap_two_images(Fl_Menu_ *o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->m_pDoc;
	unsigned char* temp;
	temp = pDoc->m_ucPainting;
	pDoc->m_ucPainting = pDoc->m_ucBitmap;
	pDoc->m_ucBitmap = temp;
	pDoc->m_pUI->m_paintView->refresh();
	pDoc->m_pUI->m_origView->refresh();
}
// callback for copy from original view to paint view
void ImpressionistUI::cb_copy_to_paintview(Fl_Menu_ *o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->m_pDoc;
	memcpy(pDoc->m_ucPainting, pDoc->m_ucBitmap, pDoc->m_nHeight * pDoc->m_nWidth * 3);
	pDoc->m_pUI->m_paintView->refresh();
}
// callback for copy from paint view to original view
void ImpressionistUI::cb_copy_to_originalview(Fl_Menu_ *o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->m_pDoc;
	memcpy(pDoc->m_ucBitmap, pDoc->m_ucPainting, pDoc->m_nHeight * pDoc->m_nWidth * 3);
	pDoc->m_pUI->m_origView->refresh();
}

//-----------------
// callback for filter dialog
//-----------------
void ImpressionistUI::cb_filter_dialog(Fl_Menu_ *o, void* v) {
	whoami(o)->m_paintView->make_current();
	whoami(o)->m_FilterDialog->show();
	whoami(o)->m_paintView->SaveCurrentContent();
	whoami(o)->m_paintView->savePic();
}

void ImpressionistUI::get_filter_parameters(int *matrix, int& divisor, int& offset) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			matrix[j * 5 + i] = atoi(this->m_FilterInput[i][j]->value());
		}
	}
	divisor = atoi(this->m_FilterDivideByInput->value());
	offset = atoi(this->m_FilterOffsetInput->value());
}
void ImpressionistUI::cb_filter_preview(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	// this is where filtering is implemented, I knwo this is ugly, but...
	int *matrix = new int[5 * 5];
	int divideBy, offset;
	pUI->get_filter_parameters(matrix, divideBy, offset);

	int width = pUI->m_pDoc->m_nWidth;
	int height = pUI->m_pDoc->m_nHeight;
	unsigned char* image = pUI->m_pDoc->m_ucPainting;


	// do the filtering
	pUI->m_paintView->make_current();
	pUI->m_paintView->RestoreContent();
	FilterBrush::filter_image(image, width, height, 0, 0, width, height, matrix, divideBy, offset, pUI->m_pDoc);

	pUI->m_paintView->refresh();
}
void ImpressionistUI::cb_filter_apply(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	pUI->cb_filter_preview(o, v);
	// save the current content to pics list
	pUI->m_paintView->savePic();
	pUI->m_FilterDialog->hide();
	pUI->m_paintView->refresh();
}
void ImpressionistUI::cb_filter_cancel(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	pUI->m_paintView->RestoreContent(); // discard current canvas, get fresh new pic from pics
	pUI->m_paintView->refresh();
	pUI->m_FilterDialog->hide();
}
void ImpressionistUI::cb_filter_reset(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			pUI->m_FilterInput[i][j]->value("0");
		}
	}
	pUI->m_FilterInput[2][2]->value("1");
	pUI->m_FilterOffsetInput->value("0");
	pUI->m_FilterDivideByInput->value("1");
}

void ImpressionistUI::cb_filter_normalize(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			sum += atoi(pUI->m_FilterInput[i][j]->value());
		}
	}
	if (sum > 0) {
		char v[100];
		pUI->m_FilterOffsetInput->value("0");
		pUI->m_FilterDivideByInput->value(itoa(sum, v, 10));
	} else if (sum == 0) {
		pUI->m_FilterOffsetInput->value("128");
		pUI->m_FilterDivideByInput->value("0");
	} else if (sum < 0) {
		pUI->m_FilterOffsetInput->value("256");
		pUI->m_FilterDivideByInput->value("0");
	}
}

void ImpressionistUI::cb_filter_sharpen(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	pUI->m_FilterResetButton->do_callback();
	int matrix[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
	char number[5];
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			sprintf(number, "%d", matrix[j-1][i-1]);
			pUI->m_FilterInput[i][j]->value(number);
		}
	}
	pUI->m_FilterNormalizeButton->do_callback();
}

void ImpressionistUI::cb_filter_enhance(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	pUI->m_FilterResetButton->do_callback();
	int matrix[3][3] = {{0, 0, 0}, {-1, 1, 0}, {0, 0, 0}};
	char number[5];
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			sprintf(number, "%d", matrix[j-1][i-1]);
			pUI->m_FilterInput[i][j]->value(number);
		}
	}
	pUI->m_FilterNormalizeButton->do_callback();
}
void ImpressionistUI::cb_filter_blur(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	pUI->m_FilterResetButton->do_callback();
	int matrix[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	char number[5];
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			sprintf(number, "%d", matrix[j-1][i-1]);
			pUI->m_FilterInput[i][j]->value(number);
		}
	}
	pUI->m_FilterNormalizeButton->do_callback();
}
void ImpressionistUI::cb_filter_emboss(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	pUI->m_FilterResetButton->do_callback();
	int matrix[3][3] = {{-2, 1, 0}, {-1, 1, 1}, {0, 1, 2}};
	char number[5];
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			sprintf(number, "%d", matrix[j-1][i-1]);
			pUI->m_FilterInput[i][j]->value(number);
		}
	}
	pUI->m_FilterNormalizeButton->do_callback();
}
void ImpressionistUI::cb_filter_edgeenhance(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	pUI->m_FilterResetButton->do_callback();
	int matrix[3][3] = {{0, 0, 0}, {-1, 1, 0}, {0, 0, 0}};
	char number[5];
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			sprintf(number, "%d", matrix[j-1][i-1]);
			pUI->m_FilterInput[i][j]->value(number);
		}
	}
	pUI->m_FilterNormalizeButton->do_callback();
}
void ImpressionistUI::cb_filter_edgedetect(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	pUI->m_FilterResetButton->do_callback();
	int matrix[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
	char number[5];
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			sprintf(number, "%d", matrix[j-1][i-1]);
			pUI->m_FilterInput[i][j]->value(number);
		}
	}
	pUI->m_FilterNormalizeButton->do_callback();
}


//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist for COMP4411, HKUST\nBy Heng and Ligeng\nSpring, 2012. Due on March 1");
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_mosaic(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	int count;
	// this will get the file path relative to the application itself
	Fl_Native_File_Chooser *chooser = new Fl_Native_File_Chooser();
	chooser->type(Fl_Native_File_Chooser::BROWSE_MULTI_FILE);   // let user browse a single file
	chooser->title("Open an image file");                        // optional title
	chooser->directory(".");
	chooser->filter("RGB Image Files\t*.{bmp,png,jpg,jpeg}");                 // optional filter
	switch ( chooser->show() ) {
		case -1:    // ERROR
			fprintf(stderr, "*** ERROR show() failed:%s\n", chooser->errmsg());
			break;
		case 1:     // CANCEL
			fprintf(stderr, "*** CANCEL\n");
			break;
		default:    // USER PICKED A FILE
			count = chooser->count();
			const char** newfile = new const char*[count];
			for (int i = 0; i < count; i++)
			{
				newfile[i] = chooser->filename(i);
			}
			fprintf(stderr, "Filename was '%s'\n", newfile);
			pDoc->createMosaic(newfile, count);
			break;
	}
}


//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;

	pDoc->setBrushType(type);
}

void ImpressionistUI::cb_lineDirectionChoice(Fl_Widget* o, void* v)
{
	m_pLineDirectionType = (TYPE_LINE_DIRECTION)(int)v;
	// ca~~, must convert twice ?
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

// auto paint button callback
void ImpressionistUI::cb_autoPaint(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	
	pDoc->m_pUI->m_paintView->autoPaint();
}


// Edge button callback
void ImpressionistUI::cb_edge(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->edge_view();
}

// disolve slider call back
void ImpressionistUI::cb_dissolve(Fl_Widget* o, void* v) {
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->dissolve_image(pDoc->m_pUI->m_DimAlphaSlider->value());
	pDoc->m_pUI->m_origView->setView(DISSOLVE_VIEW);
}

// callback for make dimmed view in paint view
void ImpressionistUI::cb_dim(Fl_Widget* o, void* v) {
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->make_dim(pDoc->m_pUI->m_DimAlphaSlider->value());
	pDoc->m_pUI->m_paintView->refresh();
}

// callback for blend menu 
void ImpressionistUI::cb_blendcolor(Fl_Menu_* o, void* v) {
	fl_color_chooser("Please choose blend color", blendColor[0], blendColor[1], blendColor[2]);
	// ((ImpressionistUI*)(o->user_data()))->m_BlendColorChooserDialog->show();
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_origView->show();
	m_paintView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	wrapper_group_origiview->size(w, h);
	wrapper_group_paintview->size(w, h);
	m_origView->size(w,h);
	m_paintView->size(w,h);

	// order matters?
	// hope not, but there're overlap anyway
	m_origView->refresh();
	m_paintView->refresh();

}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_BrushSizeSlider->value();
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	if (size<=40) {
		m_BrushSizeSlider->value(size);
	}
}

//------------------------------------------------
// Return the brush width
//------------------------------------------------
int ImpressionistUI::getWidth()
{
	return m_BrushWidthSlider->value();
}

//-------------------------------------------------
// Set the brush width
//-------------------------------------------------
void ImpressionistUI::setWidth( int width )
{

	if (width<=40) {
		m_BrushWidthSlider->value(width);
	}
}

//------------------------------------------------
// Return the brush angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	switch (m_pLineDirectionType) {
	case LDIRECTION_SLIDER_RIGHT_MOUSE:
		return m_BrushAngleSlider->value();
	case LDIRECTION_GRADIENT:
		return m_paintView->getGradient();
	case LDIRECTION_BRUSH_DIRECTION:
		return m_paintView->getBrushDirection();
	default:
		// this is error
		return 0;
	}
}

//-------------------------------------------------
// Set the brush angle
//-------------------------------------------------
void ImpressionistUI::setAngle( int angle )
{
	if (angle<=180) {
		m_BrushAngleSlider->value(angle);
	}
}

//------------------------------------------------
// Return the brush alpha
//------------------------------------------------
float ImpressionistUI::getAlpha()
{
	return m_BrushAlphaSlider->value();
}

//-------------------------------------------------
// Set the brush alpha
//-------------------------------------------------
void ImpressionistUI::setAlpha( float alpha )
{

	if (alpha<=1.0f) {
		m_BrushAlphaSlider->value(alpha);
	}
}

// show painterly dialog
void ImpressionistUI::cb_painterly_dialog(Fl_Menu_* o, void* v) {
	whoami(o)->m_PainterlyDialog->show();
}


void ImpressionistUI::cb_painterly_stylechoice(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = (ImpressionistUI*)o->user_data();

}
void ImpressionistUI::cb_painterly_brushchoice(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = (ImpressionistUI*)o->user_data();

}

void ImpressionistUI::cb_painterly_do(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI=(ImpressionistUI*)(o->user_data());
	// get the parameters from ui here
	// then call function in doc
	pUI->m_paintView->painterly_paint();
}


// Main menu definition
/// need to set callback during initialization
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image, 0, FL_MENU_INACTIVE},
		{ "&Change Image",	FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_change_image, 0, FL_MENU_INACTIVE},
		{ "&Load Another Image",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_another_image, 0, FL_MENU_DIVIDER | FL_MENU_INACTIVE},

		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Edit",		0, 0, 0, FL_SUBMENU | FL_MENU_INACTIVE },
		{ "&Undo",	FL_CTRL + 'z', (Fl_Callback *)ImpressionistUI::cb_undo},
		{ "&Redo",	FL_CTRL + 'x', (Fl_Callback *)ImpressionistUI::cb_redo},
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
		{ "&Brush Panel...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Filter Kernel",	FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_filter_dialog}, 
		{ "B&lend", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_blendcolor},
		{ "&Painterly", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_painterly_dialog},
		{ "&Create Image Mosaic", FL_ALT + 'i', (Fl_Callback *)ImpressionistUI::cb_mosaic, 0, FL_MENU_DIVIDER },
		{ "&Swap Two View",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_swap_two_images},
		{ "C&opy Paint2Orig",	FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_copy_to_originalview},
		{ "Cop&y Orig2Paint",	FL_ALT + 'y', (Fl_Callback *)ImpressionistUI::cb_copy_to_paintview},
		{ 0 },

	{ "&Display",		0, 0, 0, FL_SUBMENU },
		{ "&Original Image",	FL_ALT + 'o', (Fl_Callback *)ImpressionistUI::cb_show_original_image},
		{ "&Edge Image",	FL_ALT + 'e', (Fl_Callback *)ImpressionistUI::cb_show_edge_image},
		{ "Another Image",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_show_another_image, 0, FL_MENU_DIVIDER},
		{ "Dissolved Image",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_show_dissolve_image},
		{ "Dissolve Orig2Another",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_dissolve_one_to_another},
		{ "Dissolve Another2Orig",	FL_ALT + 'n', (Fl_Callback *)ImpressionistUI::cb_dissolve_another_to_one},
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Customized Filter",	FL_ALT+'f', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_FILTER},
  {0}
};

Fl_Menu_Item ImpressionistUI::lineDirectionMenu[NUM_LDIRECTION_TYPE + 1] = {
  {"Slider / Right Mouse",	FL_ALT+'s', (Fl_Callback *)ImpressionistUI::cb_lineDirectionChoice, (void *)LDIRECTION_SLIDER_RIGHT_MOUSE},
  {"Gradient (perpendicular)",	FL_ALT+'g', (Fl_Callback *)ImpressionistUI::cb_lineDirectionChoice, (void *)LDIRECTION_GRADIENT},
  {"Brush Direction",	FL_ALT+'b', (Fl_Callback *)ImpressionistUI::cb_lineDirectionChoice, (void *)LDIRECTION_BRUSH_DIRECTION},
  {0}
};

Fl_Menu_Item ImpressionistUI::painterlyStyleMenu[NUM_PAINTERLY_STYLE + 1] = {
  {"Impressionist",	FL_ALT+'g', (Fl_Callback *)ImpressionistUI::cb_painterly_stylechoice, (void *)STYLE_IMPRESSIONIST},
  {"Expressionist",	FL_ALT+'b', (Fl_Callback *)ImpressionistUI::cb_painterly_stylechoice, (void *)STYLE_EXPRESSIONIST},
  {0}
};

Fl_Menu_Item ImpressionistUI::painterlyBrushMenu[NUM_PAINTERLY_BRUSH + 1] = {
  {"Circle Brush",	FL_ALT+'i', (Fl_Callback *)ImpressionistUI::cb_painterly_brushchoice, (void *)PAINTERLY_BRUSH_CIRCLE},
  {"Line Brush",	FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_painterly_brushchoice, (void *)PAINTERLY_BRUSH_LINE},
  {"Curve Brush",	FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_painterly_brushchoice, (void *)PAINTERLY_BRUSH_CURVE},
  {"B-Spline Brush",	FL_ALT+'b', (Fl_Callback *)ImpressionistUI::cb_painterly_brushchoice, (void *)PAINTERLY_BRUSH_BSPLINE},
  {0}
};

void ImpressionistUI::activeMenus() {
	// remember to modify these magic numbers when new menu items are added
	menuitems[2].activate();
	menuitems[3].activate();
	menuitems[4].activate();
	menuitems[7].activate();
	menuitems[21].activate();
}

// I know this is ugle, but don't have better idea
// initialize the static member variables here
double ImpressionistUI::blendColor[3] = {1, 1, 1};
TYPE_LINE_DIRECTION ImpressionistUI::m_pLineDirectionType = LDIRECTION_SLIDER_RIGHT_MOUSE;

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	Fl::scheme("gtk+");
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
	m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
	// using user_data is convention

		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install original view window
			wrapper_group_origiview = new Fl_Group(0, 25, 300, 275);
				m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
				m_origView->box(FL_DOWN_FRAME);
				m_origView->deactivate();
				/*
				Many years after p1 of comp152h, finally understand that
				should use point and new to create widget
				to avoid automatic gabarge collection
				*/
			wrapper_group_origiview->end();
			// wrapper_group_origiview->resizable(0);
			wrapper_group_origiview->box(FL_NO_BOX);

			// install paint view window
			wrapper_group_paintview = new Fl_Group(300, 25, 300, 275);
				m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");
				m_paintView->box(FL_DOWN_FRAME);
				m_paintView->show();
			wrapper_group_paintview->end();
			wrapper_group_paintview->resizable(0);
			wrapper_group_paintview->box(FL_NO_BOX);

		group->end();
		Fl_Group::current()->resizable(group);
		// current() should mean the mainwindow

	m_mainWindow->end();

	// filter kernel dialog
	m_FilterDialog = new Fl_Window(210, 500, "Filter Dialog Dialog");
	m_FilterDialog->set_modal();
		Fl_Group *inputGroup = new Fl_Group(5, 5, 200, 150);
		inputGroup->box(FL_DOWN_BOX);
	// how to deactive the mainwindow when this dialog is shown ?
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				m_FilterInput[i][j] = new Fl_Int_Input(10 + 40 * i, 10 + 30 * j, 30, 20);
				m_FilterInput[i][j]->value("0");
			}
		}
		inputGroup->end();

		m_FilterEdgeEnhanceButton = new Fl_Button(10, 220, 190, 30, "Edge Enhance");
		m_FilterBlurButton = new Fl_Button(10, 300, 90, 30, "Blur");
		m_FilterEnhanceButton = new Fl_Button(110, 300, 90, 30, "Enhance");
		m_FilterEdgeDetectButton = new Fl_Button(10, 260, 90, 30, "Edge Detect");
		m_FilterSharpenButton = new Fl_Button(10, 340, 90, 30, "Sharpen");
		m_FilterEmbossButton = new Fl_Button(110, 260, 90, 30, "Emboss");
		
		m_FilterBlurButton->user_data((void*)this);
		m_FilterEnhanceButton->user_data((void*)this);
		m_FilterSharpenButton->user_data((void*)this);
		m_FilterEdgeEnhanceButton->user_data((void*)this);
		m_FilterEdgeDetectButton->user_data((void*)this);
		m_FilterEmbossButton->user_data((void*)this);
		
		m_FilterBlurButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_blur);
		m_FilterEnhanceButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_enhance);
		m_FilterSharpenButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_sharpen);
		m_FilterEdgeEnhanceButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_edgeenhance);
		m_FilterEdgeDetectButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_edgedetect);
		m_FilterEmbossButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_emboss);
		
		m_FilterNormalizeButton = new Fl_Button(10, 190, 80, 20, "Normalize");
		m_FilterNormalizeButton->user_data((void*)this);
		m_FilterNormalizeButton->labelsize(12);
		m_FilterNormalizeButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_normalize);

		m_FilterResetButton = new Fl_Button(110, 340, 90, 30, "Reset");
		m_FilterResetButton->user_data((void*)this);
		m_FilterResetButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_reset);

		m_FilterInput[2][2]->value("1");
		m_FilterDivideByInput = new Fl_Int_Input(150, 160, 50, 20, "Divide By: ");
		m_FilterDivideByInput->align(FL_ALIGN_LEFT);
		m_FilterDivideByInput->value("1");
		m_FilterOffsetInput = new Fl_Int_Input(150, 190, 50, 20, "Offset: ");
		m_FilterOffsetInput->value("0");
		// no need to set user_data/callback for inputs, values will be retrieved directly

		m_FilterCancelButton = new Fl_Button(110, 460, 90, 30, "Cancel");
		m_FilterCancelButton->user_data((void*)this);
		m_FilterCancelButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_cancel);
		m_FilterApplyButton = new Fl_Button(10, 460, 90, 30, "Apply");
		m_FilterApplyButton->user_data((void*)this);
		m_FilterApplyButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_apply);
		m_FilterPreviewButton = new Fl_Button(10, 420, 190, 30, "Preview");
		m_FilterPreviewButton->user_data((void*)this);
		m_FilterPreviewButton->callback((Fl_Callback*)ImpressionistUI::cb_filter_preview);



	m_FilterDialog->end();

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 475, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		// clear canvas button
		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		// line direction choice
		m_LineDirectionChoice = new Fl_Choice(100, 45, 250, 25, "&Line Direction");
		m_LineDirectionChoice->user_data((void*)(this));
		m_LineDirectionChoice->menu(lineDirectionMenu);
		m_LineDirectionChoice->callback(cb_lineDirectionChoice);
		m_LineDirectionChoice->deactivate();

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushSizeSlider->labelfont(FL_COURIER);
		m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(20);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);

		// Add brush width slider to the dialog 
		m_BrushWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Width");
		m_BrushWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWidthSlider->labelfont(FL_COURIER);
		m_BrushWidthSlider->labelsize(12);
		m_BrushWidthSlider->minimum(1);
		m_BrushWidthSlider->maximum(40);
		m_BrushWidthSlider->step(1);
		m_BrushWidthSlider->value(1);
		m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushWidthSlider->deactivate(); // disable

		// Add brush angle slider to the dialog 
		m_BrushAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Angle");
		m_BrushAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAngleSlider->labelfont(FL_COURIER);
		m_BrushAngleSlider->labelsize(12);
		m_BrushAngleSlider->minimum(0);
		m_BrushAngleSlider->maximum(180);
		m_BrushAngleSlider->step(1);
		m_BrushAngleSlider->value(0);
		m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushAngleSlider->deactivate(); // disable

		// Add brush alpha slider to the dialog 
		m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0.0f);
		m_BrushAlphaSlider->maximum(1.0f);
		m_BrushAlphaSlider->step(0.01f);
		m_BrushAlphaSlider->value(1.0f);
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);

		m_AutoPaintBox = new Fl_Group(10, 215, 380, 50, "Auto Painting ");
		m_AutoPaintBox->box(FL_UP_BOX);
		m_AutoPaintBox->align(FL_ALIGN_TOP_LEFT);

			m_AutoPaintDistanceSlider = new Fl_Value_Slider(20, 235, 150, 20, "Spacing");
			m_AutoPaintDistanceSlider->type(FL_HOR_NICE_SLIDER);
			m_AutoPaintDistanceSlider->labelfont(FL_COURIER);
			m_AutoPaintDistanceSlider->labelsize(12);
			m_AutoPaintDistanceSlider->minimum(1);
			m_AutoPaintDistanceSlider->maximum(16);
			m_AutoPaintDistanceSlider->step(1);
			m_AutoPaintDistanceSlider->value(4);
			m_AutoPaintDistanceSlider->align(FL_ALIGN_TOP_LEFT);

			m_AutoPaintRandButton = new Fl_Light_Button(190, 225, 100, 30, "Rand Size");
			m_AutoPaintRandButton->value(1);

			m_AutoPaintButton = new Fl_Button(310, 225, 50, 30, "Do!");
			m_AutoPaintButton->user_data((void*)(this));
			m_AutoPaintButton->callback(cb_autoPaint);

		m_AutoPaintBox->end();

		m_EdgeSettingBox = new Fl_Group(10, 285, 380, 50, "Edge Detecting");
		m_EdgeSettingBox->box(FL_UP_BOX);
		m_EdgeSettingBox->align(FL_ALIGN_TOP_LEFT);;

			m_EdgeThresholdSlider = new Fl_Value_Slider(20, 300, 180, 20, "Edge Threshold");
			m_EdgeThresholdSlider->type(FL_HOR_NICE_SLIDER);
			m_EdgeThresholdSlider->minimum(0);
			m_EdgeThresholdSlider->maximum(500);
			m_EdgeThresholdSlider->step(1);
			m_EdgeThresholdSlider->value(100);
			m_EdgeThresholdSlider->labelsize(12);
			m_EdgeThresholdSlider->align(FL_ALIGN_RIGHT);
			m_EdgeThresholdSlider->callback((Fl_Callback *)ImpressionistUI::cb_edge);
			m_EdgeThresholdSlider->user_data((void*)this);

			m_EdgingButton = new Fl_Button(310, 295, 50, 30, "Do!");
			m_EdgingButton->user_data((void*)(this));
			m_EdgingButton->callback((Fl_Callback *)ImpressionistUI::cb_edge);

		m_EdgeSettingBox->end();
		
		this->m_DissolveAlphaSlider = new Fl_Value_Slider(20, 365, 280, 20, "Dissolve Level in Original View");
		this->m_DissolveAlphaSlider->type(FL_HOR_NICE_SLIDER);
		this->m_DissolveAlphaSlider->minimum(0.0f);
		this->m_DissolveAlphaSlider->maximum(1.0f);
		this->m_DissolveAlphaSlider->step(0.02f);
		this->m_DissolveAlphaSlider->value(0.0f);
		// this->m_DissolveAlphaSlider->labelsize(12);
		this->m_DissolveAlphaSlider->align(FL_ALIGN_TOP_LEFT);
		// this->m_DissolveAlphaSlider->callback((Fl_Callback *)ImpressionistUI::cb_dissolve);
		this->m_DissolveAlphaSlider->user_data((void*)this);

		this->m_DissolveAlphaDoButton = new Fl_Button(320, 360, 50, 30, "Do");
		this->m_DissolveAlphaDoButton->callback((Fl_Callback*)ImpressionistUI::cb_dissolve);
		this->m_DissolveAlphaDoButton->user_data((void*)this);

		this->m_DimAlphaSlider = new Fl_Value_Slider(20, 410, 280, 20, "Dim Level in Paint View");
		this->m_DimAlphaSlider->type(FL_HOR_NICE_SLIDER);
		this->m_DimAlphaSlider->maximum(255);
		this->m_DimAlphaSlider->minimum(0);
		this->m_DimAlphaSlider->step(1);
		this->m_DimAlphaSlider->value(0);
		this->m_DimAlphaSlider->align(FL_ALIGN_TOP_LEFT);

		this->m_DimAlphaDoButton = new Fl_Button(320, 405, 50, 30, "Do");
		this->m_DimAlphaDoButton->callback((Fl_Callback*)ImpressionistUI::cb_dim);
		this->m_DimAlphaDoButton->user_data((void*)this);
		
		
		m_EdgeClipButton = new Fl_Light_Button(20, 440, 90, 30, "Edge Clip");
		m_EdgeClipButton->value(1);

		m_UseAnotherButton = new Fl_Light_Button(120, 440, 120, 30, "Another Image");
		m_UseAnotherButton->value(0);

		m_ReverseColorButton = new Fl_Light_Button(260, 440, 120, 30, "Reverse Color");
		m_ReverseColorButton->value(0);

	m_brushDialog->end();	

	m_PainterlyDialog = new Fl_Window(400, 400, "Painterly");
	// m_PainterlyDialog->set_modal();
	// set_modal() could cause the exit button to disapear, 
	// thus need to implement cancel button by yourself...
	// to do or not to do
		m_PainterlyDoButton = new Fl_Button(340, 10, 50, 30, "Do");
		m_PainterlyDoButton->callback((Fl_Callback*)ImpressionistUI::cb_painterly_do);
		m_PainterlyDoButton->user_data((void*) this);

		m_PaintingStyleChoice = new Fl_Choice(10, 10, 130, 20, "Style");
		m_PaintingStyleChoice->menu(ImpressionistUI::painterlyStyleMenu);
		m_PaintingStyleChoice->user_data((void*)this);
		m_PaintingStyleChoice->align(FL_ALIGN_RIGHT);

		m_PainterlyBrushChoice = new Fl_Choice(190, 10, 100, 20, "Brush");
		m_PainterlyBrushChoice->align(FL_ALIGN_RIGHT);
		m_PainterlyBrushChoice->user_data((void*)this);
		m_PainterlyBrushChoice->menu(ImpressionistUI::painterlyBrushMenu);

		m_PainterlyThresholdSlider = new Fl_Value_Slider(10, 50, 280, 20, "Threshold");
		m_PainterlyMaxBrushSlider = new Fl_Value_Slider(10, 80, 280, 20, "Max Brush Size");
		m_PainterlyMinBrushSlider = new Fl_Value_Slider(10, 110, 280, 20, "Min Brush Size");
		m_PainterlyGridSizeSlider = new Fl_Value_Slider(10, 140, 280, 20, "Grid Size");
		m_PainterlyLayerSlider = new Fl_Value_Slider(10, 170, 280, 20, "Layer Number");
		m_PainterlyCurvatureSlider = new Fl_Value_Slider(10, 200, 280, 20, "Curvature ?");
		m_PainterlyBlurSlider = new Fl_Value_Slider(10, 230, 280, 20, "Blur Scale");
		m_PainterlyAlphaSlider = new Fl_Value_Slider(10, 260, 280, 20, "Alpha Scale");
		m_PainterlyMaxStrokeLengthSlider = new Fl_Value_Slider(10, 290, 280, 20, "Max Stroke L");
		m_PainterlyMinStrokeLengthSlider = new Fl_Value_Slider(10, 320, 280, 20, "Min Stroke L");

		m_PainterlyThresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyMaxBrushSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyMinBrushSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyGridSizeSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyLayerSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyCurvatureSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyBlurSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyMaxStrokeLengthSlider->type(FL_HOR_NICE_SLIDER);
		m_PainterlyMinStrokeLengthSlider->type(FL_HOR_NICE_SLIDER);

		m_PainterlyThresholdSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyMaxBrushSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyMinBrushSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyGridSizeSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyLayerSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyCurvatureSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyBlurSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyAlphaSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyMaxStrokeLengthSlider->align(FL_ALIGN_RIGHT);
		m_PainterlyMinStrokeLengthSlider->align(FL_ALIGN_RIGHT);

		m_PainterlyThresholdSlider->minimum(0);
		m_PainterlyThresholdSlider->maximum(250);
		m_PainterlyThresholdSlider->value(100);
		// to add more here

	m_PainterlyDialog->end();
}
