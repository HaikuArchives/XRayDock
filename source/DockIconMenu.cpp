/* DockIconMenu.cpp par etno 2008 */

#include "DockIconMenu.h"
#ifndef XDOCK_WINDOW_H
#include "XDockWindow.h"
#endif
#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif

DockIconMenu::DockIconMenu()
: BPopUpMenu("Options")
{
	set_as_pref_icon = new BMenuItem("Set As Prefs Icon", new BMessage(DOCK_PREF_ICON));
	set_as_trash_icon = new BMenuItem("Set As Trash", new BMessage(DOCK_TRASH_ICON));
	
	
	remove = new BMenuItem("Remove", new BMessage(DOCK_REMOVE));
	move_right = new BMenuItem("Move to Right", new BMessage(DOCK_MOVE_RIGHT));
	move_left = new BMenuItem("Move to Left", new BMessage(DOCK_MOVE_LEFT));
	modify_shortcut = new BMenuItem("Modify Shortcut", new BMessage(DOCK_MODIFY_SHORTCUT));
	modify_icon = new BMenuItem("Modify Icon", new BMessage(DOCK_MODIFY_ICON));
	

	// Menu preferences
	add = new BMenuItem("Add an icon", new BMessage(ADD_ICON_WINDOW));
	settings = new BMenuItem("Settings", new BMessage(OPEN_SETTINGS_WINDOW));
	quit = new BMenuItem("Quit XRayDock", new BMessage(CLOSE_WINDOW));

	// menu corbeille
	empty_trash = new BMenuItem("Empty Trash", new BMessage(DOCK_EMPTY_TRASH));
	
	

	
}

DockIconMenu::~DockIconMenu()
{
}

void
DockIconMenu::Construction(bool is_preferences){
	if(is_preferences == false) {
	AddItem(modify_shortcut);
	AddItem(modify_icon);
	AddSeparatorItem();
	AddItem(move_left);
	AddItem(move_right);
	AddSeparatorItem();
	AddItem(remove);
	AddSeparatorItem();
	AddItem(set_as_pref_icon);
	AddItem(set_as_trash_icon);
	}
	else {
	AddItem(add);
	AddItem(settings);
	AddSeparatorItem();
	AddItem(move_left);
	AddItem(move_right);
	AddSeparatorItem();
	AddItem(set_as_pref_icon);
	AddSeparatorItem();
	AddItem(quit);
	add->SetTarget(((XDockApplication*)be_app)->backWindow);
	settings->SetTarget(((XDockApplication*)be_app)->backWindow);
	quit->SetTarget(((XDockApplication*)be_app)->backWindow);
	}
		
}

void
DockIconMenu::ConstructionTrash(bool is_trash){
	if(is_trash == false) {
	AddItem(modify_shortcut);
	AddItem(modify_icon);
	AddSeparatorItem();
	AddItem(move_left);
	AddItem(move_right);
	AddSeparatorItem();
	AddItem(remove);
	AddSeparatorItem();
	AddItem(set_as_pref_icon);
	AddItem(set_as_trash_icon);
	}
	else {
	AddItem(empty_trash);
	AddSeparatorItem();
	AddItem(set_as_trash_icon);
	//empty_trash->SetTarget(((XDockApplication*)be_app)->backWindow);
	empty_trash->SetTarget(((XDockApplication*)be_app)->backWindow);
	}
		
}
