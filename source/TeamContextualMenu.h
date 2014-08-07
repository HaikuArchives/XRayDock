/* TeamContextualMenu.h par etno 2008 */

#ifndef TEAM_CONTEXTUAL_MENU_H
#define TEAM_CONTEXTUAL_MENU_H


#include <PopUpMenu.h>
#include <MenuItem.h>
#include "messages.h"


class TeamContextualMenu : public BPopUpMenu
{
	public:	TeamContextualMenu();
		~TeamContextualMenu();

	BMenuItem *minimize_all, *close, *bring_all_to_front, *hide_all, *hide_all_except, *close_active;
	
};

#endif
