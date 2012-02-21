//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>

#include "impressionist.h"
#include "originalview.h"
#include "paintview.h"

#include "impbrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;

	Fl_Choice*			m_LineDirectionChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_BrushWidthSlider;
	Fl_Slider*			m_BrushAngleSlider;
	Fl_Slider*			m_BrushAlphaSlider;
	Fl_Button*          m_ClearCanvasButton;

	Fl_Slider*			m_DissolveAlphaSlider;

// for auto paint box
	Fl_Group* m_AutoPaintBox;
	Fl_Slider* m_AutoPaintDistanceSlider;
	Fl_Light_Button* m_AutoPaintRandButton;
	Fl_Button* m_AutoPaintButton;

// for edge detection setting box
	Fl_Group* m_EdgeSettingBox;
	Fl_Slider* m_EdgeThresholdSlider;
	Fl_Button* m_EdgingButton;

// for filter dialog
	Fl_Window* m_FilterDialog;
	Fl_Int_Input* m_FilterInput[5][5];
	Fl_Int_Input* m_FilterDivideByInput;
	Fl_Int_Input* m_FilterOffsetInput;
	Fl_Button* m_FilterPreviewButton;
	Fl_Button* m_FilterCancelButton;
	Fl_Button* m_FilterApplyButton;
	Fl_Button* m_FilterResetButton;
	Fl_Button* m_FilterNormalizeButton;

	Fl_Button* m_FilterBlurButton;
	Fl_Button* m_FilterEnhanceButton;
	Fl_Button* m_FilterSharpenButton;
	Fl_Button* m_FilterEdgeEnhanceButton;
	Fl_Button* m_FilterEdgeDetectButton;
	Fl_Button* m_FilterEmbossButton;

// for painterly dialog
	Fl_Window* m_PainterlyDialog;
	Fl_Choice* m_PaintingStyleChoice;
	Fl_Button* m_PainterlyDoButton;
	// to be implemented

// scroll window as wrapper
// this should be public because two views need to know wrapper's height and width
	Fl_Group * wrapper_group_paintview;
	Fl_Group * wrapper_group_origiview;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	
	int					getWidth();
	void				setWidth(int width);
	
	int					getAngle();
	void				setAngle(int angle);

	float				getAlpha();
	void				setAlpha(float alpha);

	static double blendColor[3];

	static void activeMenus();

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document


	static TYPE_LINE_DIRECTION m_pLineDirectionType;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];

	static Fl_Menu_Item		lineDirectionMenu[NUM_LDIRECTION_TYPE + 1];
	// 3 types of line direction, 
	// may be extended in further implementation

	static Fl_Menu_Item		painterlyStyleMenu[NUM_PAINTERLY_STYLE + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void cb_blendcolor(Fl_Menu_* o, void* v);
	// callbacks are not necessarily to be static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_change_image(Fl_Menu_* o, void* v);
	static void	cb_another_image(Fl_Menu_* o, void* v);
	static void	cb_undo(Fl_Menu_* o, void* v);
	static void	cb_redo(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_lineDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_widthSlides(Fl_Widget* o, void* v);
	static void	cb_angleSlides(Fl_Widget* o, void* v);
	static void	cb_alphaSlides(Fl_Widget* o, void* v);

	static void	cb_autoPaint(Fl_Widget* o, void* v);
	static void	cb_edge(Fl_Widget* o, void* v);
	static void	cb_dissolve(Fl_Widget* o, void* v);

	static void	cb_show_edge_image(Fl_Menu_* o, void* v);
	static void	cb_show_original_image(Fl_Menu_* o, void* v);
	static void	cb_show_another_image(Fl_Menu_* o, void* v);
	static void	cb_show_dissolve_image(Fl_Menu_* o, void* v);

	static void	cb_filter_preview(Fl_Widget* o, void* v);
	static void	cb_filter_apply(Fl_Widget* o, void* v);
	static void	cb_filter_cancel(Fl_Widget* o, void* v);
	static void	cb_filter_reset(Fl_Widget* o, void* v);
	static void	cb_filter_normalize(Fl_Widget* o, void* v);

	static void	cb_filter_dialog(Fl_Menu_* o, void* v);

	static void cb_filter_blur(Fl_Widget* o, void* v);
	static void cb_filter_enhance(Fl_Widget* o, void* v);
	static void cb_filter_sharpen(Fl_Widget* o, void* v);
	static void cb_filter_edgeenhance(Fl_Widget* o, void* v);
	static void cb_filter_edgedetect(Fl_Widget* o, void* v);
	static void cb_filter_emboss(Fl_Widget* o, void* v);
	
	static void cb_painterly_do(Fl_Widget* o, void* v);
	static void cb_painterly_dialog(Fl_Menu_* o, void* v);
	static void cb_swap_two_images(Fl_Menu_* o, void* v);
	static void cb_copy_to_paintview(Fl_Menu_* o, void* v);
	static void cb_copy_to_originalview(Fl_Menu_* o, void* v);
};

#endif