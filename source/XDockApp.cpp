/* XDockApp.cpp par etno 2008 */

#ifndef BACK_WINDOW_H
#include "BackWindow.h"
#endif
#ifndef BUTTON_WINDOW_H
#include "ButtonWindow.h"
#endif
#ifndef XDOCK_WINDOW_H
#include "XDockWindow.h"
#endif
#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif
#ifndef APP_WATCHER_WINDOW_H
#include "AppWatcherWindow.h"
#endif
#include <Screen.h>

int main(int, char**)
{	
	XDockApplication	myApplication;

	myApplication.Run();

	return(0);
}

XDockApplication::XDockApplication()
		  		  : BApplication("application/x-vnd.xraydock")
{
	
	BRect			bRect;
	preview_list = new BList();
	watch = new WatcherWindow();
	watch->Show();
//	watch->Hide();	
	BScreen screen;
	icon_size = 50;
	expanded_ratio_size = float(1)/2; //icon_expanded_size = 200;
	icon_expanded_size = (int)(icon_size + icon_size*expanded_ratio_size);
	icon_space = 5;
	
	bRect.Set(0, screen.Frame().bottom - icon_expanded_size-20,
				70, screen.Frame().bottom);
	backWindow = new BackWindow(bRect);
	backWindow->Show();
	
	bRect.Set(0, screen.Frame().bottom - 92,
				70, screen.Frame().bottom);
	buttonWindow = new ButtonWindow(bRect);
	buttonWindow->Show();
	
	backWindow->CheckIfPrefIconExist();	

}

void
XDockApplication::MessageReceived(BMessage *message)
{
	
	switch(message->what){
		
		}
		
}
void 
XDockApplication::DispatchMessage(BMessage *message, BHandler *)
{
	if (message->WasDropped() && message->what == B_SIMPLE_DATA) {
		DetachCurrentMessage();
		message->what = B_REFS_RECEIVED;
		printf("message recu\n");
		PostMessage(message);
		delete message;
	} else {
		//DispatchMessage(message, handler);
	}
}


	
#include <Alert.h>
void
XDockApplication::ShowPrefButtons(bool pref){
	
	//creer une fenetre independante qui contiendra les boutons de preference, et qu'on pourra cacher quand on veut
	
	if(pref==true) {

	
			if(((ButtonWindow*)buttonWindow)->cacheThread)
				suspend_thread(((ButtonWindow*)buttonWindow)->cacheThread);
			
			
			resume_thread(((ButtonWindow*)buttonWindow)->montreThread);

		
	}
	else
	{
			if(((ButtonWindow*)buttonWindow)->montreThread)
				suspend_thread(((ButtonWindow*)buttonWindow)->montreThread);
			resume_thread(((ButtonWindow*)buttonWindow)->cacheThread);
	}
}
