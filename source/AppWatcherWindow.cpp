/* AppWatcherWindow.cpp par etno 2008 */


#include "AppWatcherWindow.h"

#include <Roster.h>
#include <VolumeRoster.h>
#include <NodeMonitor.h>
#include <Volume.h>
#include <Directory.h>
#include <Mime.h>
#include <OutlineListView.h>
#include <List.h>
#include <OS.h>
#include <Messenger.h>
#include <Screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <Path.h>
#include <Entry.h>
#include <Node.h>
#include <NodeInfo.h>

#include "MinimizedWindow.h"
#include "XDockApp.h"
#include "messages.h"

#define TrackerSig "application/x-vnd.Be-TRAK"
#define DeskbarSig "application/x-vnd.Be-TSKB"
#define XRayDockSig "application/x-vnd.xraydock"
#define TRACKER_STATUS_WINDOW	"Tracker Status"

WatcherWindow::WatcherWindow()
		: BWindow(BRect(0,100,90,400), "Watcher", B_FLOATING_WINDOW, B_NOT_ZOOMABLE | B_NOT_H_RESIZABLE | B_NOT_CLOSABLE | B_WILL_ACCEPT_FIRST_CLICK | B_WILL_DRAW)
{
	rgb_color noir = {0,0,0,125};
	
	// TODO: [waddlesplash] Yet more Zeta/Dano...
	//SetWindowColor(noir);
	
	vertical = true;
	BScreen screen;
	MoveTo(screen.Frame().right - 90, screen.Frame().bottom - 400);
	initialized = false;
	bring_or_minimize = false;
	watcherview = new WatcherView(BRect(Bounds().right -10, Bounds().top + 2, Bounds().right -1, Bounds().bottom -2));
	//watcherview = new WatcherView(Bounds());
	AddChild(watcherview);
	preview_window_list = new BList();
	
	show_level_init = false;
	mouse_area = 80;
	
	AddAllWindows();
	
/* roster : */
	StartWatching();
	
	initialized = true;

	SetPulseRate(1000);
}


WatcherWindow::~WatcherWindow()
{
	StopWatching();
	kill_thread(sourisThread);
	
}

bool 
WatcherWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return true;
}

void
WatcherWindow::FrameMoved(BPoint pt) {
	//printf("moved !\n");
	BScreen screen;
	MoveTo(screen.Frame().right - 90, pt.y);
}
#include <FindDirectory.h>
void
WatcherWindow::HideAll()
{
	be_roster->GetAppList(&app_list);
	int32 count_app = app_list.CountItems();
	
	BPath DesktopPath;
	find_directory(B_DESKTOP_DIRECTORY, &DesktopPath);
	
	for (int32 xi = 0; xi < count_app; xi++) {
		team_id teamT = (team_id) app_list.ItemAt(xi);
		int32 count = 0;
		int32 *num = get_token_list(teamT, &count);
			for(int32 yi=0; yi<count; yi++){
				window_info	*win_inf = get_window_info(num[yi]);
				if ( //((active_app.flags & B_BACKGROUND_APP) == 0) &&
						(strcmp(win_inf->name,"Desktop") != 0)  
						&& (strcmp(win_inf->name,"Deskbar") != 0)
						&& (strcmp(win_inf->name,"Twitcher") != 0)
						&& (strcmp(win_inf->name,"WatcherWindow") != 0)
						&& (strcmp(win_inf->name,"Team Monitor") != 0)
						&& (strcmp(win_inf->name,"BeMenu") != 0)
						&&	(strcmp(win_inf->name,DesktopPath.Path()) != 0)
						){
							//printf("desktop : %s pour %s\n", DesktopPath.Path(), win_inf->name);
							do_window_action(win_inf->id, B_MINIMIZE_WINDOW, BRect(0,0,0,0), false);
							}
			}
	}
}
void
WatcherWindow::HideAllExcept(int32 ID)
{
	be_roster->GetAppList(&app_list);
	int32 count_app = app_list.CountItems();
	
	BPath DesktopPath;
	find_directory(B_DESKTOP_DIRECTORY, &DesktopPath);
	
	for (int32 xi = 0; xi < count_app; xi++) {
		team_id teamT = (team_id) app_list.ItemAt(xi);
		int32 count = 0;
		int32 *num = get_token_list(teamT, &count);
			for(int32 yi=0; yi<count; yi++){
				window_info	*win_inf = get_window_info(num[yi]);
				if ( //((active_app.flags & B_BACKGROUND_APP) == 0) &&
						(strcmp(win_inf->name,"Desktop") != 0)  
						&& (strcmp(win_inf->name,"Deskbar") != 0)
						&& (strcmp(win_inf->name,"Twitcher") != 0)
						&& (strcmp(win_inf->name,"BeMenu") != 0)
						&&	(strcmp(win_inf->name,DesktopPath.Path()) != 0)
						
						&& (win_inf->id != ID)){
							//printf("desktop : %s pour %s\n", DesktopPath.Path(), win_inf->name);
							do_window_action(win_inf->id, B_MINIMIZE_WINDOW, BRect(0,0,0,0), false);
							}
				if ((win_inf->id == ID)
					&& win_inf->is_mini){
						do_window_action(win_inf->id, B_BRING_TO_FRONT, BRect(0,0,0,0), false);
					}
			}
	}
}
void
WatcherWindow::CloseTeam(int32 ID)
{
	
	window_info	*win_inf = get_window_info(ID);
	app_info inf;
	be_roster->GetRunningAppInfo(win_inf->team, &inf);			
	
				if ((win_inf)
						&& (strcmp(win_inf->name,"Desktop") != 0)  
						&& (strcmp(win_inf->name,"Deskbar") != 0)
						//&&	(strcmp(win_inf->name,DesktopPath.Path()) != 0)
						){
						// tout le team
							BMessenger team_to_close( (char *)NULL, win_inf->team );
							BMessage *mess;
							mess = new BMessage(B_QUIT_REQUESTED);
							// specifique au tracker
							if (strcmp(inf.signature, TrackerSig) == 0){
									mess = new BMessage('Tall');
							}
							team_to_close.SendMessage(mess);
						}
			
}
status_t 
WatcherWindow::AddAllWindows()
{
	((XDockApplication*)be_app)->Lock();

	app_info active;
	BScreen screen;
	
	
	be_roster->GetAppList(&app_list);
	int32 count_app = app_list.CountItems();
	
	
	for (int32 ix = 0; ix < count_app; ix++) {
		team_id team = (team_id) app_list.ItemAt(ix);
		int32 count = 0;
		int32 *num = get_token_list(team, &count);
			for(int32 i=0; i<count; i++){
					window_info	*win_inf = get_window_info(num[i]);
					
					if(win_inf->show_hide_level > 1)
					do_window_action(win_inf->id, B_MINIMIZE_WINDOW, BRect(0,0,0,0), false);
					else
					do_window_action(win_inf->id, B_BRING_TO_FRONT, screen.Frame(), true);
					
					
			
					snooze(100000);
					be_roster->GetRunningAppInfo(team, &active);
					ReadyForScreenshot(win_inf, active);
					
				}
			
																
			
		//free(win_inf);
		
		}
		
		
	
	Classement();

	((XDockApplication*)be_app)->Unlock();
	HideAll();
	return B_OK;


}
void 
WatcherWindow::AddNewWindow(team_id new_team)
{
		BScreen screen;
		app_info active;
		team_id team = new_team;
		
		int32 count = 0;
		int32 *num = get_token_list(team, &count);
			window_info	*win_inf = get_window_info(num[0]);
				if(win_inf) {	
					// quelle app vient on d'activer ?
					be_roster->GetRunningAppInfo(win_inf->team, &active);
					
					/* CHEEEEESE !!! une petite pause pour la photo : */
					snooze(300000);
					ReadyForScreenshot(win_inf, active);
				}	
			
		
}

void 
WatcherWindow::ReadyForScreenshot(window_info *window_inf, app_info active_app)
{
		BScreen screen;
		BPath DesktopPath;
		find_directory(B_DESKTOP_DIRECTORY, &DesktopPath);
		//printf("desktop : %s pour %s\n", DesktopPath.Path(), window_inf->name);
			if ( ((active_app.flags & B_BACKGROUND_APP) == 0)
						&& (strcmp(active_app.signature, XRayDockSig) != 0)
						&& (strcmp(active_app.signature, DeskbarSig) != 0)
						&& (strcmp(window_inf->name,"Desktop") != 0)
						&& (strcmp(window_inf->name,DesktopPath.Path()) != 0)
						//&& (strcmp(window_inf->name,TRACKER_STATUS_WINDOW) != 0)
						&& (strcmp(active_app.signature, "suite/x-vnd.Be-TRAK") != 0)
						//&& (strcmp(window_inf->name,"Twitcher") != 0)
						//&& (strcmp(window_inf->name,"BeMenu") != 0)
						//&& (strcmp(window_inf->name,"Tool Tip") != 0)
						//&& (window_inf->show_hide_level <= 1)
						){
							int32 exists = 0;
							int32 previndex = preview_window_list->CountItems();
								for(int32 i=0; i<previndex; i++){
									if( (strcmp( ((MinimizedView*)preview_window_list->ItemAt(i))->sig.String(), active_app.signature) == 0 )
										&& ((MinimizedView*)preview_window_list->ItemAt(i))->ID == window_inf->id)
									{
										FindPreviewWindowByID(window_inf->id)->window_name = window_inf->name;
										exists++;
											// on casse ici
										break;
									}
									else
									{
										//n'existe pas;
						
									}
								}	
						// existe deja, on sort :
						if(exists >= 1){
							//if(bring_or_minimize == false){
								/* a revoir car bug un peu... */
								//snooze(100000);
								//UpdateScreenshot(window_inf, window_inf->id);
									
							//}
							return;
						}
						
						// existe pas, on fait un shot :	
								if(window_inf->window_left < 0)
									window_inf->window_left = 0;
								if(window_inf->window_top < 0)
									window_inf->window_top = 0;
								if(window_inf->window_right>screen.Frame().right)
									window_inf->window_right = (int32)screen.Frame().right;
								if(window_inf->window_bottom>screen.Frame().bottom)
									window_inf->window_bottom= (int32)screen.Frame().bottom ;
				
								BRect rect(window_inf->window_left,window_inf->window_top,window_inf->window_right,window_inf->window_bottom);
								
								// la premiere fenetre est a l'index x, les cachees sont à x+1
								// c a d qu'une fenetre minimisée est à 1 et que les cachées seront à 2
								//if(window_inf->show_hide_level == 0)
								if(		( (window_inf->show_hide_level == 0) && (window_inf->is_mini == false) )
									||	( (window_inf->show_hide_level == 1) && (window_inf->is_mini == true) ) )
								
								TakeScreenshotFor(window_inf->team, window_inf->id, window_inf->name, rect, true);
								
								
								Classement();
								
								// actualisation de l'affichage de la liste de fenetres
								replied = false; // pour pouvor acceder à la fonction suivante
								HidePreview();	
						}

}

void 
WatcherWindow::FullScreenShot(BRect rect)
{
			BScreen screen;
			screenshotBitmap = new BBitmap(rect, B_CMAP8, true);
			screen.GetBitmap(&screenshotBitmap, false, &rect);
}


BBitmap * 
WatcherWindow::TakeScreenshotFor(team_id team, int32 id, const char *name, BRect zone, bool add_new)
{
	

    
			    
	
	
	
	Lock();
			FullScreenShot(zone);
			
			
			app_info active;
			be_roster->GetRunningAppInfo(team, &active);	
					
				// ajout de l'icone		
		    		BEntry *entry;
		    		entry = new BEntry(&active.ref);
    				BBitmap *Large_Icon = new BBitmap( BRect(0, 0, B_LARGE_ICON - 1,
				                    B_LARGE_ICON - 1), B_COLOR_8_BIT);
    				BBitmap *Mini_Icon = new BBitmap( BRect(0, 0, B_MINI_ICON - 1,
				                    B_MINI_ICON - 1), B_COLOR_8_BIT);
					
					// recup des icones
					BNode	node;
		    		BNodeInfo	node_info;
		    		if ((node.SetTo(entry) == B_NO_ERROR) &&
			        (node_info.SetTo(&node) == B_NO_ERROR)) {
				    
				    if(! Large_Icon)
				    		return NULL;
				    if(!  Mini_Icon)
				       		return NULL;
				        
					node_info.GetIcon(Large_Icon, B_LARGE_ICON);
					node_info.GetIcon(Mini_Icon, B_MINI_ICON);
				       
				    }
		    		
		    
		    // fin de l'ajout;*/	
				if(add_new == true) {	
					BRect pos_rect(0,0,20,20);
					MinimizedView *minimizedview = new MinimizedView(pos_rect,screenshotBitmap, Large_Icon, Mini_Icon);

		   			AddChild(minimizedview);
		    		
		    		if(vertical == true)
		    			minimizedview->rapport_H_L = (float)zone.Height()/zone.Width();
		    		else
		    			minimizedview->rapport_H_L = (float)zone.Width()/zone.Height();
		    		
		    		minimizedview->sig.RemoveAll(minimizedview->sig);
		    		minimizedview->sig << active.signature;
		    		minimizedview->ID = id;
		    		minimizedview->window_name = name;
		    		
		    		minimizedview->Team = team;
		    		// ajout à la liste
		   			preview_window_list->AddItem(minimizedview);
		   			((XDockApplication*)be_app)->preview_list->AddItem(minimizedview);
				}
				else
				{
					if(vertical == true)
						FindPreviewWindowByID(id)->rapport_H_L = (float)zone.Height()/zone.Width();
					else
						FindPreviewWindowByID(id)->rapport_H_L = (float)zone.Width()/zone.Height();

				}
	Unlock();
	
	return screenshotBitmap;
}

void
WatcherWindow::SwapFront(int32 ID)
{
		/* avec ces infos, on a la fenetre */
			window_info	*info;
			info = get_window_info(ID);
		/* on recherche si elle est deja minimisée */
			if(info) {
				if(info->is_mini) {
					/* on remonte et on prend la photo */
					do_window_action(ID, B_BRING_TO_FRONT, BRect(0,0,0,0), false);
					snooze(100000);
					UpdateScreenshot(info, ID);
				}
				else{
					/* on remonte pour prendre la photo */
					do_window_action(ID, B_BRING_TO_FRONT, BRect(0,0,0,0), false);
					snooze(100000);
					UpdateScreenshot(info, ID);
					/* puis on minimise */
					//snooze(200000);
					do_window_action(ID, B_MINIMIZE_WINDOW, BRect(0,0,0,0), false);
					
				}
			}
			else{
				CloseIDWindow(ID);
				
			}
			//delete info;
}
void
WatcherWindow::BringToFront(int32 ID)
{
	if(bring_or_minimize == false) {
		bring_or_minimize = true;
		/* avec ces infos, on a la fenetre */
			window_info	*info;
			info = get_window_info(ID);
		/* on recherche si elle est deja minimisée */
			if(info) {
				do_window_action(ID, B_BRING_TO_FRONT, BRect(0,0,0,0), false);
				snooze(100000);
				UpdateScreenshot(info, ID);
				
			}
			else
				CloseIDWindow(ID);
		
	}
		bring_or_minimize = false;
		//snooze(200000);		
}
void
WatcherWindow::Minimize(int32 ID)
{
	if(bring_or_minimize == false) {
		bring_or_minimize = true;
		/* avec ces infos, on a la fenetre */
			window_info	*info;
			info = get_window_info(ID);

		
			
			app_info appinf;
			be_roster->GetActiveAppInfo(&appinf);
			
		
			if(info){
				if(!(info->is_mini)) {
					/* update du bitmap avant minimalisation: */
					do_window_action(ID, B_BRING_TO_FRONT, BRect(0,0,0,0), false);
					snooze(100000);
					UpdateScreenshot(info, ID);
					/* enfin on minimise */
					do_window_action(ID, B_MINIMIZE_WINDOW, BRect(0,0,0,0), false);
				}
			}
				/* sinon on ferme */
			else
				CloseIDWindow(ID);
		
	}
	bring_or_minimize = false;
	//snooze(200000);
}
void
WatcherWindow::UpdateScreenshot(window_info *window_inf, int32 ID)
{
			BScreen screen;
			/* update du screenshot */
			if(window_inf->window_left < 0)
									window_inf->window_left = 0;
								if(window_inf->window_top < 0)
									window_inf->window_top = 0;
								if(window_inf->window_right>screen.Frame().right)
									window_inf->window_right = (int32)screen.Frame().right;
								if(window_inf->window_bottom>screen.Frame().bottom)
									window_inf->window_bottom= (int32)screen.Frame().bottom ;
				
								BRect rect(window_inf->window_left,window_inf->window_top,window_inf->window_right,window_inf->window_bottom);
			FindPreviewWindowByID(ID)->window_name = window_inf->name;
			FindPreviewWindowByID(ID)->bit = TakeScreenshotFor(window_inf->team, ID, window_inf->name, rect, false);
			
		/* on redessine */
			if(FindPreviewWindowByID(ID)->LockLooper())
				FindPreviewWindowByID(ID)->Draw(FindPreviewWindowByID(ID)->Bounds());
			FindPreviewWindowByID(ID)->UnlockLooper();
		/* fin de l'update */
		
}

void
WatcherWindow::CloseIDWindow(int32 ID)
{
				//LockLooper();
				RemoveChild(FindPreviewWindowByID(ID));
				preview_window_list->RemoveItem(FindPreviewWindowByID(ID));
				ReOrganiseLesFenetres();
				//UnlockLooper();
}
void
WatcherWindow::DelIfNeeded(int32 ID)
{
		window_info	*info;
		info = get_window_info(ID);
		
		/* on recherche si elle est deja minimisée */
			if(		(!info)
				||	( (info->show_hide_level > 0) && (info->is_mini == false) )
				||  ( (info->show_hide_level > 1) && (info->is_mini == true) )
				){
				//LockLooper();
				if(this->Lock()){
				if(FindPreviewWindowByID(ID) != NULL){
						
						CloseIDWindow(ID);
						replied = false; // pour pouvor acceder à la fonction suivante
						ReOrganiseLesFenetres();
					}
					
				}
				//UnlockLooper();
				this->Unlock();
			}
			
		free(info); // important car sinon augmente la taille memoire
					// utilisée, car la fonction est constamment appelée
		return;
}

long
WatcherWindow::Thread_Screenshot(tracking_data *data)
{
	WatcherWindow*	window = data->window;
	if(window->Lock()){
		if(data->team >= 0)
			window->AddNewWindow(data->team);
	
		window->ReOrganiseLesFenetres();
	}
	window->Unlock();
}





int
WatcherWindow::TriAlphabetiqueParSignature(const void *first, const void *second)
{
	return strcasecmp((*(static_cast<MinimizedView * const*>(first)))->sig.String(),
		(*(static_cast<MinimizedView * const*>(second)))->sig.String());
}
int
WatcherWindow::TriAlphabetiqueParNomDeFenetre(const void *first, const void *second)
{
	return strcasecmp((*(static_cast<MinimizedView * const*>(first)))->window_name_string.String(),
		(*(static_cast<MinimizedView * const*>(second)))->window_name_string.String());
}



void
WatcherWindow::HidePreview()
{
	int32 previndex = 0;
	int32 show_level = -50;
	BScreen screen;
	int colonne = 0;

	previndex= preview_window_list->CountItems();
	if(replied == true)
		return;
	
		if (previndex <= 0)
			return;
		else {	
			//bool one_is_expanded = false;
			for(int i=0; i<previndex; i++){
				MinimizedView * to_move = ((MinimizedView*)preview_window_list->ItemAt(i));
				MinimizedView * previous = ((MinimizedView*)preview_window_list->ItemAt(i - 1));
						
				// on ajoute une colonne si besoin 
						
				if(previous){
					if(vertical == true)
						to_move->MoveTo(show_level + 10 - colonne * 70 ,previous->Frame().bottom + 10);	
					else
						to_move->MoveTo(previous->Frame().right + 10, show_level + 10 - colonne * 70);	
				}
				
				mouse_area = (colonne+1)*(70+10);
				
				{
					if(to_move->rapport_H_L * 60 <= B_MINI_ICON + 6){ // 6 correspond à la bande pour le nom
						if(vertical == true)
							to_move->ResizeTo(60,B_MINI_ICON + 6);
						else
							to_move->ResizeTo(B_MINI_ICON + 6,60);
					}
					else{
						if(vertical == true)
							to_move->ResizeTo(60,to_move->rapport_H_L * 60);	
						else
							to_move->ResizeTo(to_move->rapport_H_L * 60,60);	
					}
					
				}
				
				
			}
			
		}
		replied = true;
					
}
void
WatcherWindow::ReOrganiseLesFenetres()
{

	Rangement();
	
	return;	
}
void
WatcherWindow::Rangement()
{
	BScreen screen;
	int colonne = 0;
	int32 previndex = 0;
	previndex= preview_window_list->CountItems();
	
		if (previndex <= 0)
			return;
		else {	
			//bool one_is_expanded = false;
			for(int i=0; i<previndex; i++){
				MinimizedView * to_move = ((MinimizedView*)preview_window_list->ItemAt(i));
				MinimizedView * previous = ((MinimizedView*)preview_window_list->ItemAt(i - 1));
						
				if(previous){
					if(vertical == true)
						to_move->MoveTo(10 + colonne * 70,previous->Frame().bottom + 10);
					else
						to_move->MoveTo(previous->Frame().right + 10, 10 + colonne * 70);
				
				}
				else {
					if(vertical == true)
						to_move->MoveTo(10 + colonne * 70,watcherview->origine_barre.y + 10);
					else
						to_move->MoveTo(10,10 + colonne * 70);	
				}
				
				mouse_area = (colonne+1)*(70+10);
				
				
			}
			
		}
}
void
WatcherWindow::Classement()
{
	/* on classe d'abord par nom de fenetre */
	preview_window_list->SortItems(TriAlphabetiqueParNomDeFenetre);
	/* puis par signature */
	preview_window_list->SortItems(TriAlphabetiqueParSignature);
}
void 
WatcherWindow::StartWatching()
{
	BMessenger msgr(this);
	be_roster->StartWatching(msgr, B_REQUEST_LAUNCHED | B_REQUEST_QUIT | B_REQUEST_ACTIVATED);
	

}

void 
WatcherWindow::StopWatching()
{
	BMessenger msgr(this);
	be_roster->StopWatching(msgr);
	
}

MinimizedView *
WatcherWindow::FindPreviewWindowByID(int32 id)
{
			//LockLooper();
				int32 counter = preview_window_list->CountItems() + 1;
				for(int32 i=0; i <counter; i++) {
					MinimizedView *win = ((MinimizedView*)preview_window_list->ItemAt(i));
					if(win)
					if(win->ID == id){
						return win;
						//win->Close();
						//preview_window_list->RemoveItem(win);
						break;
					}
				}
				return NULL;
				//UnlockLooper();
	
}
void
WatcherWindow::MovePreviewWindows(float delta)
{
	int32 index= preview_window_list->CountItems();
	MinimizedView * first = (MinimizedView*)( preview_window_list->ItemAt(0) );
		if (index <= 0)
			return;
		else {	
			for(int i=0; i<index; i++){
				MinimizedView * to_move = (MinimizedView*)( preview_window_list->ItemAt(i) );
				to_move->MoveBy(0,delta*20);
				
				if(first)
					watcherview->origine_barre.Set(first->Frame().left, first->Frame().top - 10);// = ConvertToScreen(first->LeftTop());
				else
					watcherview->origine_barre.Set(10,0); 
			}
		}
		
}

void 
WatcherWindow::MessageReceived(BMessage *msg)
{
	BMessenger fenetre_active;
//	team_id	fenetre_team;
	const char *signature;
	int32 id;
	window_info	*infos;
	
	//msg->PrintToStream();
	
	switch(msg->what) {
		case B_MOUSE_MOVED:
			Activate(true); // ca va servir pour le wheel
		break;
		case B_MOUSE_WHEEL_CHANGED:
			float wdelta;
			msg->FindFloat("be:wheel_delta_y", &wdelta);
			MovePreviewWindows(-wdelta);
			//printf("wheel !\n");
		break;
		case B_LINK_TARGET:
		break;
		case DOCK_MINIMIZE_ALL:
			msg->FindInt32("id", &id);
			
			infos = get_window_info(id);
			do_minimize_team(BRect(0,0,0,0), infos->team, false);
			break;
		case DOCK_BRING_FRONT_ALL:
			msg->FindInt32("id", &id);
			
			infos = get_window_info(id);
			do_bring_to_front_team(BRect(0,0,0,0), infos->team, false);
			be_roster->ActivateApp(infos->team);
			break;
		case DOCK_CLOSE_TEAM:
			msg->FindInt32("id", &id);
			CloseTeam(id);
			break;
		case DOCK_CLEAN:
			HideAll();
			break;
		case DOCK_CLEAN_EXCEPT:
			msg->FindInt32("id", &id);
			HideAllExcept(id);
			break;
		case CLOSE_ACTIVE_WINDOW:
			
			// experimental
			msg->FindInt32("id", &id);
			//HideAllExcept(id);
			//
			Activate(false);
			LockLooper();
			infos = get_window_info(id);
			if(!(infos->is_mini))
					Minimize(id);
			snooze(20000);
				
			//do_window_action(id, B_BRING_TO_FRONT, BRect(0,0,0,0), false);
			BringToFront(id);
			
			// on desactive le screenshot
			BMessenger *mess;
			mess = new BMessenger(this);
			
			// TODO: [waddlesplash] Yet more Zeta/Dano...
			//if(*mess != BMessenger(be_roster->ActiveWindow(true)))
			//	BMessenger(be_roster->ActiveWindow(false)).SendMessage(B_QUIT_REQUESTED);
			
			UnlockLooper();
			break;
		
		case update_Windows:
			if(msg->FindString("nom", &signature) == B_OK){
				msg->FindInt32("id", &id);
				
				DelIfNeeded(id);
			}
		break;
		/* message recu quand on clique sur la souris depuis minimizedwindow
		 : obtenu apres envoi d'un bmessage */
		
		case B_MIDDLE_MOUSE_DOWN:
			if(msg->FindString("nom", &signature) == B_OK){
			msg->FindInt32("id", &id);
				if(bring_or_minimize == false){
					SwapFront(id);
				}
			}
			break;
		case B_RIGHT_MOUSE_DOWN:
			if(msg->FindString("nom", &signature) == B_OK){
			msg->FindInt32("id", &id);
				if(bring_or_minimize == false){
					Minimize(id);
				}
			}
			break;
		case B_MOUSE_DOWN:
			Activate(false);
			/* on recherche les infos concernant la fenetre
			lancée par un BMessage depuis minimizedwindow */
			if(msg->FindString("nom", &signature) == B_OK){
			msg->FindInt32("id", &id);
			/* opération de passage au premier plan */
			
			
			
				if(bring_or_minimize == false){
					//BringToFront(id);
					/* j'ai decidé de ce reglage pour ne pas léser les possesseurs de souris ou pads à 2 boutons */
					SwapFront(id);
				}
			}
		break;
		
		case B_SOME_APP_LAUNCHED:
		{
			team_id team;
			msg->FindInt32("be:team", &team);
			
		}
		break;
		case B_SOME_APP_QUIT:
		{
			//team_id team = -1;
			//msg->FindInt32("be:team", &team);
			
		}
		break;
#if 0 // TODO: [waddlesplash] Yet more Zeta/Dano...
		case B_SOME_WINDOW_ACTIVATED:
		{
			
			team_id team;
			msg->FindInt32("be:team", &team);
			//AddNewWindow(team);
			/*if(closeit ==true){
				BMessenger(be_roster->ActiveWindow(true)).SendMessage(B_QUIT_REQUESTED);
				closeit = false;			
			}
			else*/
			if(bring_or_minimize == false){
			tracking_data	*data = new tracking_data();
			data->window = this;
			data->team = team;
			screenshotThread = spawn_thread(WatcherWindow::Thread_Screenshot, "Thread_Screenshot",
				B_NORMAL_PRIORITY, data);
			
			resume_thread(screenshotThread);
			}
			
			break;
		}
#endif
		default:
			BWindow::MessageReceived(msg);	
	}
	
}


WatcherView::WatcherView(BRect rect)
	   	   : BView(rect, "wview", B_FOLLOW_ALL, B_PULSE_NEEDED | B_FRAME_EVENTS | B_WILL_DRAW)
{
	rgb_color blanc = {255,255,255,125};
	rgb_color gris = {100,100,100,125};
	rgb_color noir = {0,0,0,125};
	SetViewColor(noir);
	SetLowColor(blanc);
	SetHighColor(gris);
	
//	
}
void
WatcherView::AttachedToWindow()
{
SetMouseEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY);
}

WatcherView::~WatcherView()
{

}
void
WatcherView::Draw(BRect)
{
	StrokeRoundRect(BRect(Bounds().left,Bounds().top +2, Bounds().right, Bounds().bottom -2 ), 3, 3, B_SOLID_LOW);
	FillRoundRect(BRect(Bounds().left+1,Bounds().top +3, Bounds().right-1, Bounds().bottom -3 ), 3, 3, B_SOLID_HIGH);
}

void
WatcherView::MouseDown(BPoint)
{
	pressing = true;
	
}
void
WatcherView::MouseUp(BPoint)
{
	pressing = false;
	
}
void
WatcherView::Pulse()
{
	MinimizedView * first = (MinimizedView*)( (((WatcherWindow*)Window())->preview_window_list)->ItemAt(0) );
	if(!first)
		return;
	if(first->Frame().top >= Frame().top + 10){
		first->MoveTo(10,10);
		origine_barre.Set(10,0);
		pressing = false;
		((WatcherWindow*)Window())->ReOrganiseLesFenetres();
		return;
	}
	
	if(pressing == false)
		return;
	
	
	BPoint		pti;
	ulong		but;
	GetMouse(&pti, &but);
	BRect barre(Bounds());
	
	
	
	if(!(barre.Contains(pti)))
		return;
	
	
	
	if (pressing == true){
		int32 index= ((WatcherWindow*)Window())->preview_window_list->CountItems();
		if (index <= 0)
			return;
		else {	
			for(int i=0; i<index; i++){
				MinimizedView * to_move = (MinimizedView*)( (((WatcherWindow*)Window())->preview_window_list)->ItemAt(i) );
				MinimizedView * last = (MinimizedView*)( (((WatcherWindow*)Window())->preview_window_list)->ItemAt(index-1) );
				//MinimizedView * previous = ((MinimizedView*)preview_window_list->ItemAt(i - 1));
				
				
				// pourcentage d'ecart par rapport au milieu		
				float pcent = (float)( ((pti.y - Bounds().Height()/2) / (Bounds().Height()/2)) * 100);
				if(to_move){
					if(((WatcherWindow*)Window())->vertical == true)
						
						if(pti.y < Bounds().Height() /2){
							if(first->Frame().top < Frame().top + 9){
								to_move->MoveBy(0,-0.3*pcent);
								
							}
							else {
								
							}
						}	
						if(pti.y > Bounds().Height() /2)
							if(last->Frame().bottom > Frame().bottom - 10)
								to_move->MoveBy(0,-0.3*pcent);
				}
			}
		}
	}
	
	
	if(first)
		origine_barre.Set(first->Frame().left, first->Frame().top - 10);// = ConvertToScreen(first->LeftTop());
	else
		origine_barre.Set(10,0); 
		
	printf("%f\n", origine_barre.y);
				
	
}
void
WatcherView::FrameResized(float, float) {
	Draw(Bounds());
	
}

