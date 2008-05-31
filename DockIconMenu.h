/* DockIconMenu.h par etno 2008 */

#ifndef DOCK_ICON_MENU_H
#define DOCK_ICON_MENU_H



#include <PopUpMenu.h>
#include <MenuItem.h>
#include "messages.h"


class DockIconMenu : public BPopUpMenu
{
	public:	DockIconMenu();
		~DockIconMenu();
	
	void	Construction(bool is_preferences);		
	void	ConstructionTrash(bool is_trash);
	
	BMenuItem *remove, *move_right, *move_left, *modify_shortcut, *modify_icon;
	BMenuItem *set_as_pref_icon, *add, *settings, *quit;
	BMenuItem *set_as_trash_icon;
	BMenuItem *empty_trash;
	bool is_pref_menu, is_trash_menu;		
};

#endif //DOCK_ICON_MENU_H
