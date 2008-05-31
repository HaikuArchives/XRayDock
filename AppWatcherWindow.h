/* AppWatcherWindow.h par etno 2008 */

#ifndef APP_WATCHER_WINDOW_H
#define APP_WATCHER_WINDOW_H

#ifndef MINIMIZED_WINDOW_H
#include "MinimizedWindow.h"
#endif



#include <Window.h>
#include <Path.h>
#include <Roster.h>
#define	CLOSE_ACTIVE_WINDOW	'BeCW'

class BVolumeRoster;

class WatcherWindow;


// idée tirée d'opentracker
struct window_info {
	team_id		team;
    int32   	id;

	int32		thread;
	int32		client_token;
	int32		client_port;
	uint32		workspaces;

	int32		layer;
    uint32	  	w_type;
    uint32      flags;
	int32		window_left;
	int32		window_top;
	int32		window_right;
	int32		window_bottom;
	int32		show_hide_level;
	bool		is_mini;
	char		name[1];
};


struct tracking_data {
	WatcherWindow		*window;
	team_id			team;

};
class WatcherView : public BView {
	public:
							WatcherView(BRect rect);
		virtual				~WatcherView();

	virtual void	Draw(BRect);
	virtual void	MouseDown(BPoint);	
	virtual void	MouseUp(BPoint);
	virtual void	Pulse();
	virtual void	AttachedToWindow();
	virtual void	FrameResized(float, float);
	BPoint origine_barre;
private:
			bool	pressing;

};
class WatcherWindow : public BWindow {
	public:
							WatcherWindow();
		virtual				~WatcherWindow();
		bool				QuitRequested();
		virtual void		FrameMoved(BPoint pt);
		
			
		void				MessageReceived(BMessage *msg);
		void				ReOrganiseLesFenetres();
		void				SwapFront(int32 ID);
		void				DelIfNeeded(int32 ID);
		BBitmap *			TakeScreenshotFor(team_id team, int32 id, const char *name, BRect zone, bool add_new);
		void				AddNewWindow(team_id new_team);
		status_t			AddAllWindows();
		void				ReadyForScreenshot(window_info *window_inf, app_info active_app);
		void				BringToFront(int32 ID);
		void				Minimize(int32 ID);
		void				CloseIDWindow(int32 ID);
		void				FullScreenShot(BRect rect);
		void				UpdateScreenshot(window_info *window_inf, int32 ID);
		void				HideAll();
		void				HideAllExcept(int32 ID);
		void				CloseTeam(int32 ID);
		void				MovePreviewWindows(float delta);

		BPath *path;
		BList *preview_window_list;
		

		long	sourisThread;
		bool vertical;


	private:
		WatcherView	*watcherview;
		MinimizedView		*FindPreviewWindowByID(int32 id);
		static int TriAlphabetiqueParSignature( const void *first, const void *second);
		static int TriAlphabetiqueParNomDeFenetre( const void *first, const void *second);
		void	Classement();
		void	Rangement();
		void	HidePreview();
		void				ResizePreview();
		static	long	Thread_Souris(void *);		
		static	long	Thread_Screenshot(tracking_data *);			


		long	screenshotThread;


	
		BBitmap *screenshotBitmap;
		BList app_list; // app en cours d'utilisation.
		
		void				StartWatching();
		void				StopWatching();

		bool	initialized;	
		bool	bring_or_minimize;		
		bool	show_level_init;
		int32	mouse_area;

		bool	replied;
		bool closed;


};


// idée tirée d'opentracker
enum window_action {
	B_MINIMIZE_WINDOW,
	B_BRING_TO_FRONT,
};


int32		count_windows(team_id app);
void		do_window_action(int32 window_id, int32 action,BRect zoomRect, bool zoom);
window_info	*get_window_info(int32 a_token);
int32		*get_token_list(team_id app, int32 *count);
void do_minimize_team(BRect zoomRect, team_id team, bool zoom);
void do_bring_to_front_team(BRect zoomRect, team_id app, bool zoom);


#endif

