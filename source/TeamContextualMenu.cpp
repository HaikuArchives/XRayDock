/* TeamContextualMenu.cpp par etno 2008 */

#include "TeamContextualMenu.h"

#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif

TeamContextualMenu::TeamContextualMenu()
: BPopUpMenu("Team")
{
	minimize_all = new BMenuItem("Minimize All The Team", new BMessage(DOCK_MINIMIZE_ALL));
	bring_all_to_front = new BMenuItem("Bring All The Team To Front", new BMessage(DOCK_BRING_FRONT_ALL));
	
	close = new BMenuItem("Close All This Team", new BMessage(DOCK_CLOSE_TEAM));
	close_active = new BMenuItem("Close This Window (Experimental)", new BMessage(CLOSE_ACTIVE_WINDOW));
	
	hide_all = new BMenuItem("Clean The Desktop", new BMessage(DOCK_CLEAN));
	hide_all_except = new BMenuItem("Hide All Except", new BMessage(DOCK_CLEAN_EXCEPT));
	
	
	//move_left = new BMenuItem("Move to Left", new BMessage(DOCK_MOVE_LEFT));
	AddItem(hide_all);
	AddItem(hide_all_except);
	AddSeparatorItem();
	AddItem(minimize_all);
	AddItem(bring_all_to_front);
	AddItem(close);
	AddItem(close_active);


	

	minimize_all->SetTarget(((XDockApplication*)be_app)->backWindow);
	bring_all_to_front->SetTarget(((XDockApplication*)be_app)->backWindow);
	close->SetTarget(((XDockApplication*)be_app)->backWindow);
}

TeamContextualMenu::~TeamContextualMenu()
{
}
