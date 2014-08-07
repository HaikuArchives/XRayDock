/* XDockView.h par etno 2008 */
#ifndef XDOCK_VIEW_H
#define XDOCK_VIEW_H

#include <Bitmap.h>
#include <String.h>
#include <StringView.h>

#define DOCK_CHANGE_ICON		'dChI'
#define DOCK_CHANGE_SHORTCUT	'dChS'
#define MY_DRAG_ACTION			'dMdA'
class XDockWindow;
class BackWindow;
class XDockView : public BView 
{
public:
	XDockView(BRect frame, const char *icon_p, const char *shortcut_p);
	~XDockView();
	

	BBitmap *icon;
	virtual void	Draw(BRect);
//	virtual void	MouseMoved(BPoint pt, uint32 code, const BMessage *msg);
	virtual void	MouseDown(BPoint pt);
//	virtual void	MouseUp(BPoint pt);
	virtual void	MessageReceived(BMessage *message);
	virtual void	Pulse();


	void MoveOthers();
	void MoveBackOthers();
	void GetOtherViews();
	void ChangeIcon(entry_ref ref);
	void ChangeShortcut(entry_ref *ref);
	
	void	Launch();
	BString	shortcutPath;
	BString	iconPath;
	int icon_size;
	int icon_expanded_size;
	int icon_space;
	BMessenger* dock_messenger;
	BString dernier_chemin_icones;
	BString dernier_chemin_raccourcis;
	
	bool inside_view;
	
	bool is_pref_icon;
	bool is_trash_icon;
	

private:	
	

};

#endif //XDOCK_VIEW_H
