/* DockPreferencesWindow.cpp par etno 2008 */
#include <Application.h>
#include <Screen.h>
#include <String.h>
#include <Path.h>
#include <Roster.h>
#include <Directory.h>
#include <View.h>
#include <FindDirectory.h>
#include <Alert.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include <stdio.h>
#include <Box.h>
#include <Button.h>
#include <Slider.h>

#include "messages.h"
#ifndef DOCK_PREFERENCES_WINDOW_H
#include "DockPreferencesWindow.h"
#endif
#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif

DockPreferencesWindow::DockPreferencesWindow(BRect rect)
 : BWindow(rect, "Preferences", B_DOCUMENT_WINDOW, B_WILL_DRAW | B_NOT_ZOOMABLE | B_NOT_RESIZABLE, B_ALL_WORKSPACES)
{
	BView *dockpreferencesview = new BView(Bounds(), "dockpreferencesview", B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS);
	AddChild(dockpreferencesview);
	rgb_color LightMetallicBlue =		{143,166,240,	255};
	dockpreferencesview->SetViewColor(LightMetallicBlue);
	

//// j'en suis ici pour les SetTarget	
	BBox *feelbox = new BBox(BRect(5,2,160,50),"Feel"); 
	feelbox->SetLabel("Feel");
	dockpreferencesview->AddChild(feelbox);
	BRadioButton *always_front_radiobutton = new BRadioButton(BRect(5, 10, 150, 20), "always_front_radiobutton", "Always Front Window", new BMessage(DOCK_ALWAYS_ON_TOP), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	feelbox->AddChild(always_front_radiobutton);
	always_front_radiobutton->SetTarget(((XDockApplication*)be_app)->backWindow);
	BRadioButton *not_always_front_radiobutton = new BRadioButton(BRect(5, 25, 150, 35), "not_always_front_radiobutton", "NOT Always Front Window", new BMessage(DOCK_NOT_ALWAYS_ON_TOP), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	feelbox->AddChild(not_always_front_radiobutton);
	not_always_front_radiobutton->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	if(((XDockApplication*)be_app)->backWindow->front_window == true) {
		always_front_radiobutton->SetValue(1);
		}
	else
		not_always_front_radiobutton->SetValue(1);
	



	

	
	BBox *generalbackgroundbox = new BBox(BRect(5,55,160,110),"GeneralBackground"); 
	generalbackgroundbox->SetLabel("General Background");
	dockpreferencesview->AddChild(generalbackgroundbox);
	BButton *change_background_button = new BButton(BRect(5,17,150,37),"change_background", "Change", new BMessage(DOCK_CHANGE_BACKGROUND));
	generalbackgroundbox->AddChild(change_background_button);
	change_background_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	
	BBox *zoom_distance_slider_box = new BBox(BRect(5,115,160,162),"Zoom distance"); 
	zoom_distance_slider_box->SetLabel("Zoom distance");
	zoom_distance_slider = new DockSlider(new BMessage(DOCK_DISTANCE_ZOOM),30,200,5,"Near","Far");
	zoom_distance_slider_box->AddChild(zoom_distance_slider);
	zoom_distance_slider->SetValue(((XDockApplication*)be_app)->backWindow->distance);
	dockpreferencesview->AddChild(zoom_distance_slider_box);
	zoom_distance_slider->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	BBox *zoom_power_box = new BBox(BRect(5,167,160,214),"Zoom power"); 
	zoom_power_box->SetLabel("Zoom power");
	zoom_power_slider = new DockSlider(new BMessage(DOCK_POWER_ZOOM),0,15,1,"Small","Big");
	zoom_power_box->AddChild(zoom_power_slider);
	zoom_power_slider->SetValue(((XDockApplication*)be_app)->expanded_ratio_size * 10);
	dockpreferencesview->AddChild(zoom_power_box);
	zoom_power_slider->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	
	BBox *icon_size_slider_box = new BBox(BRect(5,219,160,266),"Icon size"); 
	icon_size_slider_box->SetLabel("Icon size");
	icon_size_slider = new DockSlider(new BMessage(DOCK_ICON_SIZE),24,50,4,"Small", "Big");
	icon_size_slider_box->AddChild(icon_size_slider);
	// temporaire en attendant d'ecrire l'initialisation
	icon_size_slider->SetValue(((XDockApplication*)be_app)->icon_size);
	dockpreferencesview->AddChild(icon_size_slider_box);
	icon_size_slider->SetTarget(((XDockApplication*)be_app)->backWindow);






	BBox *text_size_slider_box = new BBox(BRect(5,271,160,318),"Text size"); 
	text_size_slider_box->SetLabel("Text size");
	text_size_slider = new DockSlider(new BMessage(DOCK_FONT_SIZE),10,30,1,"Small", "Big");
	text_size_slider_box->AddChild(text_size_slider);
	// temporaire en attendant d'ecrire l'initialisation
	text_size_slider->SetValue(((XDockApplication*)be_app)->backWindow->font_size);
	dockpreferencesview->AddChild(text_size_slider_box);
	text_size_slider->SetTarget(((XDockApplication*)be_app)->backWindow);


	BBox *text_feelbox = new BBox(BRect(5,323,160,383),"info Names"); 
	text_feelbox->SetLabel("Info Names");
	dockpreferencesview->AddChild(text_feelbox);
	
	BRadioButton *top_text_button = new BRadioButton(BRect(5, 10, 150, 20), "top text", "Top", new BMessage(DOCK_TEXT_ON_TOP), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	text_feelbox->AddChild(top_text_button);
	top_text_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	BRadioButton *bottom_text_button = new BRadioButton(BRect(5, 25, 150, 35), "bottom text", "Bottom", new BMessage(DOCK_TEXT_ON_BOTTOM), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	text_feelbox->AddChild(bottom_text_button);
	bottom_text_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	BRadioButton *no_text_button = new BRadioButton(BRect(5, 40, 150, 50), "bottom text", "No Names", new BMessage(DOCK_NO_TEXT), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	text_feelbox->AddChild(no_text_button);
	no_text_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	
	if(((XDockApplication*)be_app)->backWindow->text_feel == 0) {
		top_text_button->SetValue(1);
		}
	if(((XDockApplication*)be_app)->backWindow->text_feel == 1) {
		bottom_text_button->SetValue(1);
		}
	if(((XDockApplication*)be_app)->backWindow->text_feel == 2) {
		no_text_button->SetValue(1);
		}
		




	BBox *applybox = new BBox(BRect(5,388,160,430),"Remove Settings File"); 
	applybox->SetLabel("Remove Settings File");
	dockpreferencesview->AddChild(applybox);
	BButton *apply_button = new BButton(BRect(5,15,150,35),"delete", "Delete", new BMessage(DOCK_APPLY));
	applybox->AddChild(apply_button);
	apply_button->SetTarget(((XDockApplication*)be_app)->backWindow);	



	
	//BMessage preferencesMsg;

}


DockPreferencesWindow::~DockPreferencesWindow()
{
}
#include <stdlib.h>

bool
DockPreferencesWindow::QuitRequested(){
	if(LockLooper())
	((XDockApplication*)be_app)->backWindow->preferenceswindowouverte = false;
	UnlockLooper();
	Close();
	return true;
}

DockSlider::DockSlider(BMessage *message, long int minValue, long int maxValue, int incrementalValue, const char *low_limit_label, const char *high_limit_label)
	: BSlider(BRect(5,10,150,25), "",NULL, message, minValue, maxValue, B_HORIZONTAL , B_BLOCK_THUMB, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_NAVIGABLE | B_WILL_DRAW | B_FRAME_EVENTS)
{
		SetKeyIncrementValue(incrementalValue);
		SetLimitLabels(low_limit_label, high_limit_label);
		rgb_color rouge = {255,0,0,255};
		UseFillColor(true, &rouge);
		
}

DockSlider::~DockSlider()
{
}


// pris sur le bebook :
void DockSlider::DrawThumb() {
   BRect r;
   BView *v;

   rgb_color bleu = {80,157,205,255};
   rgb_color kDarkGray = {100,100,100,255};
   rgb_color kBlackColor = {0,0,0,255};
   
   // Get the frame rectangle of the thumb
   // and the offscreen view.
   
   r = ThumbFrame();
   v = OffscreenView();

   // Draw the black shadow

   v->SetHighColor(kBlackColor);   
   r.top++;
   r.left++;
   v->StrokeEllipse(r);
   
   // Draw the dark grey edge
   
   v->SetHighColor(kDarkGray);
   r.bottom--;
   r.right--;
   v->StrokeEllipse(r);
   
   // Fill the inside of the thumb
   
   v->SetHighColor(bleu);
   r.InsetBy(1,1);
   v->FillEllipse(r);
}
