/* XDockWindow.h par etno 2008 */

#include <Picture.h>

#ifndef XDOCK_WINDOW_H
#define XDOCK_WINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif
#ifndef XDOCK_VIEW_H
#include "XDockView.h"
#endif


class XDockWindow : public BWindow 
{
public:
				XDockWindow(BRect frame, const char *icon_path, const char *shortcut_path, bool clip);
virtual	bool	QuitRequested();
virtual	void	FrameResized(float, float);
virtual	void	WindowActivated(bool);
virtual	void	MessageReceived(BMessage *message);
virtual	void	DispatchMessage(BMessage *message, BHandler *handler);
virtual	void	RefsReceived(BMessage *message);


		void ResizeOnScaledView(BRect size_of_scaled_view);
		BPicture	*ClippedPicture(BRect taille, const char *path);
		XDockView *view;
		
		BPoint		centre;
			static long		Get_Mouse(void *arg);
		BString		ref_icon_path;
		BString		ref_shortcut_path;
		bool		bool_transparency;
		
		int taille_exacte;
		int32 timeoclock;
		int32 timeontrue, timeonfalse;
		bool		IsResizing;
		bool	clipping_allowed;
		
		
private:
	BPicture	*picture;

};

#endif //XDOCK_WINDOW_H
