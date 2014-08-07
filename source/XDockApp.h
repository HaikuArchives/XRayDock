/* XDockApp.h par etno 2008 */

#ifndef XDOCK_APP_H
#define XDOCK_APP_H

#ifndef _APPLICATION_H
#include <Application.h>
#endif

#ifndef BACK_WINDOW_H
#include "BackWindow.h"
#endif

#ifndef APP_WATCHER_WINDOW_H
#include "AppWatcherWindow.h"
#endif

class XDockApplication : public BApplication 
{
public:
	XDockApplication();
	
	virtual	void	MessageReceived(BMessage *message);
//	virtual bool	QuitRequested();	
	void		ShowPrefButtons(bool pref);
	virtual	void	DispatchMessage(BMessage *message, BHandler *);
	
	BackWindow		*backWindow;
	WatcherWindow	*watch;
	BWindow			*buttonWindow;
	int icon_size;
	int icon_expanded_size;
	int icon_space;
	float expanded_ratio_size;
	
	BList	*preview_list;
};

#endif //XDOCK_APP_H
