/* ButtonWindow.h par etno 2008 */
#ifndef BUTTON_WINDOW_H
#define BUTTON_WINDOW_H

#include <Window.h>
#include <Button.h>
#include <View.h>

class ButtonWindow : public BWindow 
{
public:
				ButtonWindow(BRect frame);
				~ButtonWindow(); 
//virtual	bool	QuitRequested();
virtual void ScreenChanged( BRect, color_space );
virtual void WorkspaceActivated(int32,bool);

	static	long	Cache(void *);
	static	long	Montre(void *);
	
	BButton *add_button;
	BButton *settings_button;
	BButton *quit_button;
	BView *vue;

long			cacheThread;
long			montreThread;

};

#endif
