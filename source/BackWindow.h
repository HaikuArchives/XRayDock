/* BackWindow.h par etno 2008 */


#include <Picture.h>
#include <List.h>

#ifndef BACK_WINDOW_H
#define BACK_WINDOW_H

#ifndef _WINDOW_H
#include <Window.h>
#endif
#ifndef XDOCK_WINDOW_H
#include "XDockWindow.h"
#endif
#ifndef DOCK_PREFERENCES_WINDOW_H
#include "DockPreferencesWindow.h"
#endif

#define	ADD_ICON_WINDOW	'dAIW'
#define	DOCK_ADD_ICON	'dAIc'
#define	OPEN_SETTINGS_WINDOW	'dOSW'
#define	CLOSE_WINDOW	'dCIW'

class XDockView;

#include <Screen.h>
// vue reference pour positionnement souris :
class BackView : public BView
{
public:
	BackView(BRect frame);
	~BackView();


	void			AttachedToWindow();

private:

};

class BackWindow : public BWindow 
{
public:
				BackWindow(BRect frame);
				~BackWindow(); 
virtual	bool	QuitRequested();

virtual	void	MessageReceived(BMessage *message);
		void	MouseMoved(BPoint pt, uint32 code, const BMessage *msg);
virtual void ScreenChanged( BRect, color_space );
virtual void WorkspaceActivated(int32,bool);


	
	static	long	Thread_Position_Souris(void *);
	static	long	Thread_Organise(void *);
		
	BList	*windows_list;
	void		AddIcon(const char* path, const char* shortcut);
	void		SaveInitSettings(BMessage* preferencesMsg);
	void		LoadInitSettings(BMessage* preferencesMsg);
	void		Resize();
	void		PreferencesWindow();	
	void		MoveAfterSettings(float ratio);
	void		DernierChemin(const char* chemin);
	void		Organise(int longueur_tot);
	void		CheckIfPrefIconExist();	
//	void		ShowPrefButtons(bool pref);	
	void		StoreIdealBitmap(BBitmap *bitmap, const char *filename, const char *mime, int largeur);	
	status_t	StoreIcon(entry_ref ref, const char *nom);	
	BString		generateur_nom_fichier();



	BView *vue;


	DockPreferencesWindow	*preferenceswindow;
	BackView	*backview;
	XDockView	*icon_view;
	BPoint		position_curseur;
	int	nbb_temp;
	int nbb;
	bool front_window;
	int distance;
	bool preferenceswindowouverte;
	BPoint dernier_point;
	bool chargement_en_cours;
	bool zone_avant, zone_arriere;
	int longueur_totale;
	
	BWindow	*toolwindow;
	BView *tooltip;
	
	bool texte_en_dessous;
	
	
	
	const char	*dernier_dossier_icone;
	const char	*dernier_dossier_icones;
	const char	*dernier_dossier_raccourcis;
	BFilePanel	*addiconPanel;
	BFilePanel	*iconPanel;
	BFilePanel	*shortcutPanel;
	BString raccourci;
	BString icone;
	float font_size;
	int32 text_feel; // noms des icones au dessus, en dessous, ou sans.
	long			fThread;

private:
	//	detection souris	
		long			oThread;		// positionnement
		BPoint		last_pt;
		bool show_level_bool;
		bool hidden, not_hidden;
		void	HideIcons();
		void	ShowIcons();
		void	Do_Nothing();

};

#endif //BACK_WINDOW_H
