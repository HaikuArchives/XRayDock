/* ButtonWindow.cpp par etno 2008 */

#ifndef _APPLICATION_H
#include <Application.h>
#endif
#ifndef BACK_WINDOW_H
#include "BackWindow.h"
#endif

#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif

#include "ButtonWindow.h"

ButtonWindow::ButtonWindow(BRect frame)
	: BWindow(frame, "buttons", B_DOCUMENT_WINDOW, B_NOT_ZOOMABLE | B_WILL_DRAW | B_WILL_ACCEPT_FIRST_CLICK)
{
	SetLook(B_NO_BORDER_WINDOW_LOOK);
	SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
	
	vue = new BView(Bounds(), "", 0,0);
	AddChild(vue);
	BView	tempView( frame, "", 0, 0);
	// définition du BPicture
	BPicture	*pic;
	//	Ajout de la vue temporaire
	// sinon BeginPicture et EndPicture crashent
	AddChild( &tempView );
	// Début du dessin du BPicture
	tempView.BeginPicture( new BPicture() );
	tempView.SetDrawingMode(B_OP_OVER);
	
	// vue reference (pas forcement besoin de la dessiner sauf si ca peut faire joli):
	//tempView.FillRect(BRect(0, Bounds().bottom - 61, 1, Bounds().bottom - 10));
	
	
	// vues pour boutons
	tempView.FillRoundRect(BRect(5, Bounds().bottom - 31, 60, Bounds().bottom - 10), 6, 6);
	tempView.FillRoundRect(BRect(5, Bounds().bottom - 61, 60, Bounds().bottom - 40), 6, 6);
	tempView.FillRoundRect(BRect(5, Bounds().bottom - 91, 60, Bounds().bottom - 70), 6, 6);
	
	// Fin du dessin du BPicture
	pic = tempView.EndPicture();
	
	// Remove the temporary view
	RemoveChild( &tempView);
		
	ClipWindowToPicture(pic,BPoint(0,0), 0);
	add_button = new BButton(BRect(5, Bounds().bottom - 91, 61, Bounds().bottom-70),"","Add",
								new BMessage(ADD_ICON_WINDOW),B_FOLLOW_NONE,B_NAVIGABLE | B_WILL_DRAW); 
	settings_button = new BButton(BRect(5, Bounds().bottom - 61, 61, Bounds().bottom-40),"","Settings",
								new BMessage(OPEN_SETTINGS_WINDOW),B_FOLLOW_NONE,B_NAVIGABLE | B_WILL_DRAW);
	quit_button = new BButton(BRect(5, Bounds().bottom - 31, 61, Bounds().bottom-10),"","Quit XRD",
								new BMessage(CLOSE_WINDOW),B_FOLLOW_NONE,B_NAVIGABLE | B_WILL_DRAW); 
								 
	add_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	settings_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	quit_button->SetTarget(((XDockApplication*)be_app)->backWindow);
	
	
	// ajout des boutons
	vue->AddChild(add_button);
	vue->AddChild(settings_button);
	vue->AddChild(quit_button);
	
	cacheThread = spawn_thread(ButtonWindow::Cache, "Cache",
		B_NORMAL_PRIORITY, this);
	montreThread = spawn_thread(ButtonWindow::Montre, "Montre",
		B_NORMAL_PRIORITY, this);

	//resume_thread(cacheThread);
	
}
ButtonWindow::~ButtonWindow(){
		kill_thread(montreThread);
	kill_thread(cacheThread);
}



#include <malloc.h>

long
ButtonWindow::Cache(void *arg)
{

	ButtonWindow*	window = (ButtonWindow*)arg;

	
	while (true) {
		if (window->Lock()) {

				while(window->Frame().right > 0){
				window->MoveBy(-1,0);
				window->add_button->Draw(window->add_button->Bounds());
				window->settings_button->Draw(window->settings_button->Bounds());
				window->quit_button->Draw(window->quit_button->Bounds());
				window->vue->Draw(window->vue->Bounds());
				snooze(20000);
			}
			
			
		}
		window->Unlock();
		
		snooze(20000);
	}
	free(arg);

	return B_NO_ERROR;
}
long
ButtonWindow::Montre(void *arg)
{

	ButtonWindow*	window = (ButtonWindow*)arg;

	while (true) {
		if (window->Lock()) {

				while(window->Frame().left < 0){
				
				window->MoveBy(1,0);
				window->add_button->Draw(window->add_button->Bounds());
				window->settings_button->Draw(window->settings_button->Bounds());
				window->quit_button->Draw(window->quit_button->Bounds());
				window->vue->Draw(window->vue->Bounds());
				snooze(20000);
			}
			
			
		}
		window->Unlock();
		
		snooze(20000);
	}
	free(arg);

	return B_NO_ERROR;
}

void
ButtonWindow::WorkspaceActivated(int32,bool)
{
	BScreen screen;
	BRect bRect;
	bRect.Set(0, screen.Frame().bottom - 92,
				70, screen.Frame().bottom);
	MoveTo(bRect.left,bRect.top);
	ResizeTo(bRect.Width(), bRect.Height());

}
void
ButtonWindow::ScreenChanged( BRect, color_space )
{
	BScreen screen;
	BRect bRect;
	bRect.Set(0, screen.Frame().bottom - 92,
				70, screen.Frame().bottom);
	MoveTo(bRect.left,bRect.top);
	ResizeTo(bRect.Width(), bRect.Height());

}
