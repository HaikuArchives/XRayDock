/* BackWindow.cpp par etno 2008 */


#include <Bitmap.h>
#include <TranslationUtils.h>
#include <experimental/BitmapTools.h>
#include <FilePanel.h>
#include <Path.h>
#include <Entry.h>
#include <Screen.h>
#include <Alert.h>
#include <Roster.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <Roster.h>
#include "messages.h"
#include <malloc.h>
#include <Font.h>
#include <Button.h>
#include <BitmapStream.h>
#include <TranslatorRoster.h>
#include <NodeInfo.h>
#include <stdlib.h>


#ifndef _APPLICATION_H
#include <Application.h>
#endif
#ifndef BACK_WINDOW_H
#include "BackWindow.h"
#endif
#ifndef DOCK_IMAGE_FILTER_H
#include "DockImageFilter.h"
#endif
#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif




BackView::BackView(BRect rect)
	   	   : BView(rect, "", B_FOLLOW_ALL, B_WILL_DRAW)
{

}

BackView::~BackView()
{

}

void
BackView::AttachedToWindow()
{

}




void
BackWindow::Resize(){

				
				int icon_size_ = ((XDockApplication*)be_app)->icon_size;
				float ratio = (float)(((XDockApplication*)be_app)->expanded_ratio_size);	
				
				
				
												
				BPoint		pt;
				BScreen screen;
				
				
				ulong		button;
				backview->GetMouse(&pt, &button);
				
				
				
				
				
				// pour eviter le flashing lors du drag n drop on snooze le thread quand le bouton est enfoncé :
				while(button == B_PRIMARY_MOUSE_BUTTON){
					backview->GetMouse(&pt, &button);
					snooze(1000);	
				}
				
				
				
				nbb_temp = windows_list->CountItems();
				
				
				longueur_totale = 0;
				
				int i;
				
				XDockWindow *first, *last;
				first = ((XDockWindow *)windows_list->ItemAt(0));
				last = ((XDockWindow *)windows_list->ItemAt(nbb_temp-1));
				
				
				
				bool une_vue_active = false;
				int vue_active = 0;
				for(i=0; i<=nbb_temp-1; i++){
					if(((XDockWindow*)windows_list->ItemAt(i))->view->inside_view == true){
						une_vue_active = true;
						vue_active = i;
						break;
					}
					else{
						une_vue_active = false;
						
					}
				}
				
				
				BPoint convert;
				convert = ConvertToScreen(pt);
				
				
				int limite;
				int icon_expanded_size_ = (int)(((XDockApplication*)be_app)->icon_size*(1 + ((XDockApplication*)be_app)->expanded_ratio_size));
				if(text_feel == 1)
						limite = (int)(- icon_expanded_size_ - 10 - font_size - 10);
				else
						limite = (int)(- icon_expanded_size_ - 10);
				
				if( //((convert.x) >= screen.Frame().left + 100) && 
					//(convert.y <= screen.Frame().bottom + limite) ){
					(pt.y <= limite-1) ){	
								
						int not_organized = 0;
						for(int ii=0; ii<=nbb_temp-1; ii++){
							XDockWindow * win = ((XDockWindow*)windows_list->ItemAt(ii));
							if(win->Bounds().Width() > icon_size_) {
								win->ResizeTo(icon_size_, icon_size_);
								not_organized++;
							}
						}
						if(not_organized >= 1) { // reorganisation exigée
							Organise(nbb_temp * icon_size_);
							
								
						}
						if(toolwindow->Frame().LeftTop() != BPoint(screen.Frame().left, screen.Frame().top - 200))
								toolwindow->MoveTo(screen.Frame().left, screen.Frame().top - 200);	
					
					return; // si la souris n'est pas dans la zone on ne va pas plus loin
					}
				
					
					
					

				
				if(	une_vue_active == true) {
					
					
				
						
					const char *string = ((XDockWindow*)windows_list->ItemAt(vue_active))->view->shortcutPath.String();
					char name[B_FILE_NAME_LENGTH];
					char pref[B_FILE_NAME_LENGTH] = "Preferences";
					char trash[B_FILE_NAME_LENGTH] = "Trash";
					BEntry entry(string);
					// si ce n'est pas une icone preference
					if( (((XDockWindow*)windows_list->ItemAt(vue_active))->view->is_pref_icon == false)
						&& (((XDockWindow*)windows_list->ItemAt(vue_active))->view->is_trash_icon == false))
					{
						entry.GetName(name);
					}
					else
					{
						if (((XDockWindow*)windows_list->ItemAt(vue_active))->view->is_pref_icon == true)
						name = pref;
						else if(((XDockWindow*)windows_list->ItemAt(vue_active))->view->is_trash_icon == true)
						name = trash;
					}
					
					float longueur = tooltip->StringWidth(name);

					// texte au dessus :
					if(text_feel == 0)
					toolwindow->MoveTo(((XDockWindow*)windows_list->ItemAt(vue_active))->Frame().left + ((XDockWindow*)windows_list->ItemAt(vue_active))->Bounds().right/2 - longueur/2, ((XDockWindow*)windows_list->ItemAt(vue_active))->Frame().top - font_size - 10);	
					if(text_feel == 1)
					toolwindow->MoveTo(((XDockWindow*)windows_list->ItemAt(vue_active))->Frame().left + ((XDockWindow*)windows_list->ItemAt(vue_active))->Bounds().right/2 - longueur/2, screen.Frame().bottom - font_size - 10);	
					if(text_feel == 2){
						toolwindow->MoveTo(screen.Frame().left, screen.Frame().top - 200);
						goto suite;
					}
					if(toolwindow->LockLooper()) {
					BFont font(be_fixed_font);
					tooltip->GetFont(&font);
					font.SetSize(font_size);
					font_height height;
					font.GetHeight(&height);
					//font.SetShear(356);
					//font.SetRotation(90);
					//tooltip->SetFont(&font, B_FONT_SIZE | B_FONT_FLAGS | B_FONT_ROTATION | B_FONT_SHEAR);
					tooltip->SetFont(&font, B_FONT_SIZE | B_FONT_FLAGS);
					
					toolwindow->ResizeTo(longueur + 9,(height.ascent + height.descent + height.leading));
					tooltip->ResizeTo(toolwindow->Bounds().Width(), toolwindow->Bounds().Height());
					tooltip->FillRect(tooltip->Bounds(),B_SOLID_LOW);
					tooltip->DrawString(name, BPoint(5,font_size));
					}
					toolwindow->UnlockLooper();
				}
				else
					toolwindow->MoveTo(screen.Frame().left, screen.Frame().top - 200);	
		
		
				if(pt == last_pt)
					return; // la souris n'a pas bougé
		suite:			
				for(i=0; i<=nbb_temp-1; i++){
					
					XDockWindow * window = ((XDockWindow*)windows_list->ItemAt(i));
							
							
							/* calcul du centre de la vue */
							BPoint center;
							center = ConvertToScreen(BPoint((window->Frame().left + window->Bounds().Width()/2),
														(window->Frame().top  + window->Bounds().Height()/2)));
							
							

							


			/* si on est devant l'icone */
							
							float t;
							float pourcentage_temp;
							float pourcentage;
							if( (pt.x < center.x) && (pt.x>(center.x-distance)) &&
									( (pt.y >= (window->Frame().top - 20 - screen.Frame().Height())) && (pt.y <= (window->Frame().bottom + 150- screen.Frame().Height())))
									){
								t = (center.x - pt.x);
								pourcentage_temp = float(((float)t/distance)*100);
								pourcentage = 100 - pourcentage_temp;
									window->ResizeTo((icon_size_+ (pourcentage/100)*(ratio*icon_size_)),
													(icon_size_+ (pourcentage/100)*(ratio*icon_size_)));
										
								#if defined(DEBUG)
								printf("ratio : %f, taille icone : %f\n", (float)(ratio*icon_size_), (float)pourcentage*(ratio*icon_size_));
								printf("t = %f; pourcentage = %f\n", t, pourcentage);
								printf("nous sommes à %f du centre,\n", pt.x-center.x);			
								#endif	
								
								
								
								
							}
			/* si on est derriere l'icone */

							else if( (pt.x >= center.x) && (pt.x < (center.x+distance)) &&
									( (pt.y >= (window->Frame().top - 20 - screen.Frame().Height())) && (pt.y <= (window->Frame().bottom + 150- screen.Frame().Height())))
									){
	
								t = (pt.x - center.x);
								pourcentage_temp = float(((float)t/distance)*100);
								pourcentage = 100 - pourcentage_temp;								
								#if defined(DEBUG)
								printf("%f, %f,\n", t, ratio);
								#endif
									window->ResizeTo((icon_size_+ (pourcentage/100)*(ratio*icon_size_)),
													(icon_size_+ (pourcentage/100)*(ratio*icon_size_)));
									
								
								
								
							}
							
					/* en dehors */
							else {
								
								window->ResizeTo(int(icon_size_),int(icon_size_));
								#if defined(DEBUG)		
								printf("Nous sommes pas dans une vue\n");
								#endif
								
								
								
								
							}
							
				
				/* calcul de la longueur de la bande d'icones qq soit le grossissement */
				longueur_totale = (int)(longueur_totale + window->Bounds().Width());
				
						
					
					
				}
				Organise(longueur_totale);	
				//printf("longueur = %d\n", longueur_totale);	
				dernier_point = pt;
				
		backview->GetMouse(&last_pt, &button);		

}

void
BackWindow::Organise(int longueur_tot)
{
	
	XDockWindow *window, *next, *first, *last, *previous;
	nbb_temp = windows_list->CountItems();
	first = ((XDockWindow *)windows_list->ItemAt(0));
	last = ((XDockWindow *)windows_list->ItemAt(nbb_temp-1));
	BPoint		pt;
	ulong		button;
	backview->GetMouse(&pt, &button);
	BScreen screen;
	int i;
	
	
	for(i=0; i<nbb_temp; i++) {
	
	
	window = ((XDockWindow *)windows_list->ItemAt(i));
	previous = ((XDockWindow *)windows_list->ItemAt(i-1));
	float longueur_ensemble = longueur_tot;
						if(pt != dernier_point) {
							next = ((XDockWindow *)windows_list->ItemAt(i+1));
							first->MoveTo(int32((screen.Frame().Width() - longueur_ensemble)/2), first->Frame().top);
							next->MoveTo(int32(window->Frame().right), next->Frame().top);		
							}
					
	}
}



long
BackWindow::Thread_Position_Souris(void *arg)
{

	BackWindow*	window = (BackWindow*)arg;

	BPoint pt;
	BScreen screen;
	uint32 button;
	int32 show_icon_level = -1;
	while (true) {
		
		
		if (window->Lock()) {
				window->backview->GetMouse(&pt, &button);
				
				int limite;
				int icon_expanded_size_ = (int)(((XDockApplication*)be_app)->icon_size*(1 + ((XDockApplication*)be_app)->expanded_ratio_size));
				if(window->text_feel == 1)
						limite = (int)(- icon_expanded_size_ - 10 - window->font_size - 10);
				else
						limite = (int)(- icon_expanded_size_ - 10);
				
/* determine la position du curseur :
	si il est bas la barre remonte
	puis reste montée tant que le curseur ne redepasse pas une certaine hauteur */
							
				if(pt.y >= - ((XDockApplication*)be_app)->icon_size - 20){
								window->show_level_bool = true;
								show_icon_level = 0;
							}
							
							if( (window->show_level_bool == true) && (pt.y >= limite) ){
									show_icon_level = 0;
									window->show_level_bool = true;
									
							}
							else {
									show_icon_level = -1;
									window->show_level_bool = false;
							}
							
							if(show_icon_level == 0){
								window->Resize();
								window->ShowIcons();
								window->hidden = false;
								
							}
							if(show_icon_level == -1){
								if(window->not_hidden == true)
								window->HideIcons();
								window->not_hidden = false;
							}
						
			
		}
		window->Unlock();
		
		snooze(10000);
	}
	free(arg);

	return B_NO_ERROR;
}



BackWindow::BackWindow(BRect frame)
	: BWindow(frame, "Hello", B_DOCUMENT_WINDOW, B_NOT_ZOOMABLE | B_WILL_DRAW | B_WILL_ACCEPT_FIRST_CLICK)
{
	/* creation du filtre pour les filepanel (images) */

	
	windows_list = new BList();
	SetLook(B_NO_BORDER_WINDOW_LOOK);
	SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
	position_curseur.x=0;
	
	
	vue = new BView(Bounds(), "", 0,0);
	AddChild(vue);
	BView	tempView( frame, "", 0, 0);
	// définition du BPicture
	BPicture	*pic;
	//	Ajout de la vue temporaire
	// sinon BeginPicture et EndPicture crashent
	AddChild( &tempView );
	// Début du dessin du BPicture
	tempView.BeginPicture( new BPicture() );
	tempView.SetDrawingMode(B_OP_OVER);
	
	// vue reference (pas forcement besoin de la dessiner sauf si ca peut faire joli):
	//tempView.FillRect(BRect(0, Bounds().bottom - 61, 1, Bounds().bottom - 10));
	
	
	// vues pour boutons
/*	tempView.FillRoundRect(BRect(5, Bounds().bottom - 31, 60, Bounds().bottom - 10), 6, 6);
	tempView.FillRoundRect(BRect(5, Bounds().bottom - 61, 60, Bounds().bottom - 40), 6, 6);
	tempView.FillRoundRect(BRect(5, Bounds().bottom - 91, 60, Bounds().bottom - 70), 6, 6);
*/	
	// Fin du dessin du BPicture
	pic = tempView.EndPicture();
	
	// Remove the temporary view
	RemoveChild( &tempView);
		
	ClipWindowToPicture(pic,BPoint(0,0), 0);



	// ajout de la vue référence :
	backview  = new BackView(BRect(0, Bounds().bottom - 1, 500, Bounds().bottom));
	AddChild(backview);



/* thread souris */		
	fThread = spawn_thread(BackWindow::Thread_Position_Souris, "Thread_Position_Souris",
		B_NORMAL_PRIORITY, this);


	resume_thread(fThread);
	
/* quelques variables d'initialisation */	
	front_window = true;
	text_feel = 1;
	distance = 150;
	preferenceswindowouverte = false;
	font_size = 20.0;
	texte_en_dessous = true;
	
		
	// petite fenetre pour nom des icones
	toolwindow = new BWindow(BRect(10,-200,50,-180),"", B_MODAL_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_WILL_DRAW);
	tooltip = new BView(BRect(0,0,40,12), "", B_FOLLOW_ALL, B_WILL_DRAW);
	toolwindow->AddChild(tooltip);
	toolwindow->SetLook(B_BORDERED_WINDOW_LOOK);
	toolwindow->Show();


/* encore des variables, quel bordel ! */
	chargement_en_cours = true;
	BMessage preferencesMsg;
	LoadInitSettings(&preferencesMsg);
	chargement_en_cours = false;


	DockImageFilter	*dockimagefilter;
	dockimagefilter = new DockImageFilter();
	dernier_dossier_icones = "/boot/home";
	dernier_dossier_raccourcis = "/boot/home";
	/* creation du panel d'ajout d'icones */
	addiconPanel = new BFilePanel(B_OPEN_PANEL,new BMessenger(this), 0, 0, false, new BMessage(DOCK_ADD_ICON), dockimagefilter);
	
	/* celui de modif d'icones */
	iconPanel = new BFilePanel(B_OPEN_PANEL,0 ,0 , 0, false, new BMessage(DOCK_CHANGE_ICON), dockimagefilter);

	
	/* et celui des raccourcis */
	shortcutPanel = new BFilePanel(B_OPEN_PANEL, 0, 0, B_ANY_NODE, false, new BMessage(DOCK_CHANGE_SHORTCUT));
	
	shortcutPanel->SetPanelDirectory(raccourci.String()); 
	iconPanel->SetPanelDirectory(icone.String());
	addiconPanel->SetPanelDirectory(icone.String()); 

	/* initialisation du reste des options : */
	int32 nb_windows = ((XDockApplication*)be_app)->CountWindows();
	for(int j=0 ; j<nb_windows; j++) {
				((XDockApplication*)be_app)->WindowAt(j)->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
				
		/* permet de mettre les panel en floating_all */
	}
	int32 nb_items = windows_list->CountItems();
	int i;
	if(front_window == true) {
	for(i=0 ; i<nb_items; i++) {
				//((XDockApplication*)be_app)->WindowAt(i)->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
				((XDockWindow*)windows_list->ItemAt(i))->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
				front_window = true;
			}
	}
	else {
	for(i=0 ; i<nb_items; i++) {
				//((XDockApplication*)be_app)->WindowAt(i)->SetFeel(B_NORMAL_WINDOW_FEEL);
				((XDockWindow*)windows_list->ItemAt(i))->SetFeel(B_NORMAL_WINDOW_FEEL);
				front_window = false;
			}
	}
	
	
	/* mise en place finale */
	MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
	
}

BackWindow::~BackWindow(){
	kill_thread(fThread);
}


void
BackWindow::MessageReceived(BMessage *message)
{
	entry_ref shortcut_ref;
	entry_ref icon_ref;
	
	BEntry entry("/boot/preferences/Backgrounds", true );
	entry_ref ref;
	entry.GetRef(&ref);
	BMessage msg(B_REFS_RECEIVED);
	msg.AddRef("refs",&ref);
	
	
	int32 nb_items = ((XDockApplication*)be_app)->CountWindows();
	int i;


	BScreen screen;
	BMessage trashmsg(B_DELETE_PROPERTY);
	/* ici la création du filtre : */
					
	switch(message->what) {
		case DOCK_EMPTY_TRASH :
		trashmsg.AddSpecifier( "Trash" );
		BMessenger("application/x-vnd.Be-TRAK").SendMessage(&trashmsg); 
		
		break;
		case DOCK_APPLY:
			// suppression du fichier de settings en cas de probleme.
			int deep;
			int32 buttonIndex;
			BAlert* alert;
			alert = new BAlert( "Delete Settings File ?",
										"Are you sure you want to delete the settings file ? You will lost all your icons and shortcuts. A backup will be made.",
										"Cancel", "YES, I'm Sure", NULL, B_WIDTH_AS_USUAL,
										B_WARNING_ALERT );
                buttonIndex = alert->Go();
                deep = buttonIndex == 1;
            if ( deep )
            {
                
/* la, je me suis pas fais chier ! */
/* j'ai honte */
                system("cp /boot/home/config/settings/XRAYDock/sysinit /boot/home/config/settings/XRAYDock/sysinit.bak");
                system("rm /boot/home/config/settings/XRAYDock/sysinit");
                alert = new BAlert( "Deleted",
										"Done ! sysinit deleted. sysinit.bak created."
										"the app have to close...",
										"Ok", NULL, NULL, B_WIDTH_AS_USUAL,
										B_IDEA_ALERT );
                alert->Go();
                QuitRequested();
               
            }
            
			break;
		case DOCK_DISTANCE_ZOOM:
			distance = ((XDockApplication*)be_app)->backWindow->preferenceswindow->zoom_distance_slider->Value();
			break;
		case DOCK_ICON_SIZE:
			for(i=0 ; i<nb_items; i++) {
				((XDockApplication*)be_app)->icon_size = ((XDockApplication*)be_app)->backWindow->preferenceswindow->icon_size_slider->Value();
				int icon_size_;
				icon_size_ = ((XDockApplication*)be_app)->icon_size;
				//if(  (((XDockApplication*)be_app)->WindowAt(i) != preferenceswindow) && (((XDockApplication*)be_app)->WindowAt(i) != this) && (((XDockApplication*)be_app)->WindowAt(i) != (BWindow*)addiconPanel)) {
					((XDockWindow*)windows_list->ItemAt(i))->ResizeTo(icon_size_,
													icon_size_);
					
			}
			MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
			break;
			break;
		case DOCK_POWER_ZOOM:
			(((XDockApplication*)be_app)->expanded_ratio_size) = float(((XDockApplication*)be_app)->backWindow->preferenceswindow->zoom_power_slider->Value())/10;
			MoveAfterSettings((((XDockApplication*)be_app)->expanded_ratio_size));
			
					
			break;
		case DOCK_FONT_SIZE:
			font_size = float(((XDockApplication*)be_app)->backWindow->preferenceswindow->text_size_slider->Value());
			MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
			break;		
		case DOCK_NO_TEXT:
			text_feel = 2;
			MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
			break;
		case DOCK_TEXT_ON_BOTTOM:
			text_feel = 1;
			MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
			break;
		case DOCK_TEXT_ON_TOP:
			text_feel = 0;
			MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
			break;
		case DOCK_ALWAYS_ON_TOP:
			for(i=0 ; i<nb_items; i++) {
				((XDockWindow*)windows_list->ItemAt(i))->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
				front_window = true;
			}
			break;
		case DOCK_NOT_ALWAYS_ON_TOP:
			for(i=0 ; i<nb_items; i++) {
				((XDockWindow*)windows_list->ItemAt(i))->SetFeel(B_NORMAL_WINDOW_FEEL);
				front_window = false;
			}
			break;
		
		case DOCK_CHANGE_BACKGROUND:
		
			//system("/boot/beos/system/Tracker /boot/preferences/Backgrounds &")
			
			/* bon, on va pas s'emmerder,
			considérons que le Tracker est encore dans le systeme,
			il est donc capable de lancer l'application Backgrounds
			qui entre parenthèses a été ajoutée au BMessage msg via son entry_ref */
			be_roster->Launch("application/x-vnd.Be-TRAK", &msg);
		break;	
		
		case ADD_ICON_WINDOW:
			/* noter maintenant l'intégration du filtre dans la création du FilePanel : */
			addiconPanel->Show();
			break;
		case DOCK_ADD_ICON:
			if (message->FindRef("refs", &icon_ref) == B_OK) {
				BPath *mypath = new BPath(&icon_ref);
                //
				BString nom;
				nom<<generateur_nom_fichier().String();
				StoreIcon(icon_ref, nom.String());
				mypath->GetParent(mypath);
				icone.RemoveAll(icone);
				icone << mypath->Path();
				AddIcon(nom.String(), "/boot/home/");
				addiconPanel->SetPanelDirectory(icone.String());
				iconPanel->SetPanelDirectory(icone.String());
				MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
			}
			
			break;
		case OPEN_SETTINGS_WINDOW:
			PreferencesWindow();
			break;
		case CLOSE_WINDOW:
			BMessage preferencesMsg;
			SaveInitSettings(&preferencesMsg);
			QuitRequested();
			
		break;
		
		
		
	}
	return;
}


void
BackWindow::WorkspaceActivated(int32,bool)
{
	BScreen screen;
	BRect bRect;
	bRect.Set(0, screen.Frame().bottom - ((XDockApplication*)be_app)->icon_expanded_size-20,
				70, screen.Frame().bottom);
	MoveTo(bRect.left,bRect.top);
	ResizeTo(bRect.Width(), bRect.Height());
	MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
}
void
BackWindow::ScreenChanged( BRect, color_space )
{
	BScreen screen;
	BRect bRect;
	bRect.Set(0, screen.Frame().bottom - ((XDockApplication*)be_app)->icon_expanded_size-20,
				70, screen.Frame().bottom);
	MoveTo(bRect.left,bRect.top);
	ResizeTo(bRect.Width(), bRect.Height());
	MoveAfterSettings(((XDockApplication*)be_app)->expanded_ratio_size);
}
void BackWindow::MoveAfterSettings(float ratio)
{
	int32 nb_items = windows_list->CountItems();
	int i;
	BScreen screen;
	//font_size = float(((XDockApplication*)be_app)->backWindow->preferenceswindow->text_size_slider->Value());
			int icon_size_ = ((XDockApplication*)be_app)->icon_size;
			int icon_expanded_size;
			icon_expanded_size = (int)(icon_size_ + icon_size_*ratio);
			((XDockApplication*)be_app)->icon_expanded_size = icon_expanded_size;
			for(i=0 ; i<nb_items; i++) {
				
					XDockWindow *HW = ((XDockWindow*)windows_list->ItemAt(i));
					if(text_feel == 1)
						HW->MoveTo(HW->Bounds().left, screen.Frame().Height()- icon_expanded_size - 10 - font_size - 10);
					else if(text_feel == 0)
						HW->MoveTo(HW->Bounds().left, screen.Frame().Height()- icon_expanded_size - 10);
					else if(text_feel == 2)
						HW->MoveTo(HW->Bounds().left, screen.Frame().Height()- icon_expanded_size - 10);	
				
			}
			HideIcons();
}
void BackWindow::HideIcons()
{
	
	int32 nb_items = windows_list->CountItems();
	BScreen screen;
	int icon_size_ = ((XDockApplication*)be_app)->icon_size;//*(1 + ((XDockApplication*)be_app)->expanded_ratio_size);
			for(int i=0; i<=nb_items-1; i++){
							XDockWindow * win = ((XDockWindow*)windows_list->ItemAt(i));
							
							if(win->Bounds().Width() > icon_size_) {
								if(hidden == false)
								win->ResizeTo(icon_size_, icon_size_);
								//not_organized++;
							}
							if(hidden == false) {
//								while(win->Frame().top < screen.Frame().bottom - (win->Bounds().Height()/4)*3) {
//									win->MoveBy(0, 2);
//								}
								win->MoveTo(win->Frame().left, screen.Frame().bottom - (win->Bounds().Height()/4)*3); 
							}
						}
			
			
			if(toolwindow->Frame().LeftTop() != BPoint(screen.Frame().left, screen.Frame().top - 200))
								toolwindow->MoveTo(screen.Frame().left, screen.Frame().top - 200);	
			Organise(nb_items * icon_size_);
			hidden = true;
	
}


void BackWindow::ShowIcons()
{
	
	int32 nb_items = windows_list->CountItems();
	int i;
	BScreen screen;
	int icon_size_ = ((XDockApplication*)be_app)->icon_size;//*(1 + ((XDockApplication*)be_app)->expanded_ratio_size);
	int icon_expanded_size = (int)(icon_size_ + icon_size_*( ((XDockApplication*)be_app)->expanded_ratio_size ));		
			
			for(i=0 ; i<nb_items; i++) {
				 {
					XDockWindow *HW = ((XDockWindow*)windows_list->ItemAt(i));
					if(not_hidden == false){
						if(text_feel == 1)
//							while(HW->Frame().top > screen.Frame().Height()- icon_expanded_size - 10 - font_size - 10) {
//									HW->MoveBy(0, -2);
//								}
							HW->MoveTo(HW->Frame().left, screen.Frame().Height()- icon_expanded_size - 10 - font_size - 10);
						else if( (text_feel == 0) || (text_feel == 2) )
//							while(HW->Frame().top > screen.Frame().Height()- icon_expanded_size - 10) {
//									HW->MoveBy(0, -2);
//								}
							HW->MoveTo(HW->Frame().left, screen.Frame().Height()- icon_expanded_size - 10);
					}
				}
			}
			
			not_hidden = true;
			
}

void BackWindow::AddIcon(const char* path, const char* shortcut)
{
	entry_ref shortcut_ref;
	entry_ref icon_ref;
	XDockWindow *newWindow;
	

	BScreen screen;
	int icon_size_ = ((XDockApplication*)be_app)->icon_size;
	int icon_expanded_size = ((XDockApplication*)be_app)->icon_expanded_size;
		
		BBitmap *temp_icon;
		temp_icon = BTranslationUtils::GetBitmapFile(path);
				if(temp_icon){
					// determiner si l'image chargée n'est pas trop grande
					// pour activer la transparence, sinon, ca provoque le crash de beos
					bool transparency;
					
					
					/* cette partie peut etre enlevée car depuis qq temps
					les images sont stockées à la bonne taille,
					la transparence peut donc etre toujours activée */
					
					if( (temp_icon->Bounds().Width() * temp_icon->Bounds().Height()) > (200 * 200) )
					{
							transparency = false;
							
							if(chargement_en_cours != true){
							BAlert *alert;
							alert = new BAlert("Transparency", "image resolution is too high, transparency,
														disabled for this icon.", "Ok");
							alert->Show();
							}
					}
					else
						transparency = true;
					
					//transparency = true;
					
					if(text_feel == 1) { // en dessous
					newWindow = new XDockWindow(BRect(screen.Frame().Width()/2,
										screen.Frame().Height()- icon_expanded_size  - 10 - font_size - 10,
										screen.Frame().Width()/2 + icon_size_,
										screen.Frame().Height()- icon_expanded_size + icon_size_ - 10 - font_size - 10),
										path,shortcut,transparency);
				
					}
					else
					{		// au dessus ou sans texte
					newWindow = new XDockWindow(BRect(screen.Frame().Width()/2,
										screen.Frame().Height()- icon_expanded_size  - 10,
										screen.Frame().Width()/2 + icon_size_,
										screen.Frame().Height()- icon_expanded_size + icon_size_ - 10),	
										path,shortcut,transparency);
					}
					// afficher la fenetre
					newWindow->Show();
					int i, count;
					
					
					count = windows_list->CountItems();
					// on déplace les vues déja chargées vers la gauche
					
					
					for(i=0; i< count; i++) {
						((XDockWindow*)windows_list->ItemAt(i))->MoveBy(-(icon_size_)/2, 0);
						
					}
					windows_list->AddItem(newWindow);
					newWindow->MoveBy(count*((icon_size_)/2), 0);
					int j;
					
					for(j=0; j< count; j++) {
						int X = (int)(((XDockWindow*)windows_list->ItemAt(i))->Frame().left);
						int Y = (int)(((XDockWindow*)windows_list->ItemAt(i))->Frame().top);
						((XDockWindow*)windows_list->ItemAt(i))->centre = BPoint(X,Y);
					}
					
					#if defined(DEBUG)
					printf("items in list %d \n", (int)windows_list->CountItems());
					#endif
					nbb = windows_list->CountItems();
					#if defined(DEBUG)
					printf("%d d'icones chargées\n", nbb);
					#endif
					delete temp_icon;
			
				((XDockWindow*)windows_list->ItemAt(i))->view->is_pref_icon = false;
				((XDockWindow*)windows_list->ItemAt(i))->view->is_trash_icon = false;
					
				}
				
}


bool BackWindow::QuitRequested()
{
	

	
	if(be_app->LockLooper()){
	be_app->Quit();
	}
	be_app->UnlockLooper();
	
	
	return(true);
}

void BackWindow::SaveInitSettings(BMessage* preferencesMsg)
{
	
		preferencesMsg->MakeEmpty();
		int32 nb = windows_list->CountItems();
		int32 i;
		BString storage_shortcut_name;
		BString storage_icon_name;
		BString shortcut_path;
		BString icon_path;
		
		int32 icon_size_ = ((XDockApplication*)be_app)->icon_size;
		int32 icon_expanded_size = ((XDockApplication*)be_app)->icon_expanded_size;
		float ratio = ((XDockApplication*)be_app)->expanded_ratio_size;
		
		/* sauvegardes des données concernant les tailles d'icones */
		preferencesMsg->AddInt32("icon_size",icon_size_);
		preferencesMsg->AddInt32("icon_expanded_size",icon_expanded_size);
		preferencesMsg->AddInt32("distance",distance);
		preferencesMsg->AddFloat("ratio",ratio);
		preferencesMsg->AddFloat("text_size",font_size);
		
		
		preferencesMsg->AddBool("front_window",front_window);
		
		preferencesMsg->AddInt32("text_feel",text_feel);
		
		preferencesMsg->AddString("derniers_raccourcis",raccourci.String());
		preferencesMsg->AddString("derniers_icones",icone.String());
		//#if defined(DEBUG)
		printf("dernier raccourcis : %s\n", dernier_dossier_raccourcis);
		printf("dernier icones : %s\n", dernier_dossier_icones);
		//#endif
		
		
		int32 icon_number = nb;
		preferencesMsg->AddInt32("icon_number",icon_number);
		#if defined(DEBUG)
		printf("number of iconview : '%d'\n'", (int)nb);
		printf("number of iconview saved : '%d'\n'", (int)icon_number);
		#endif
		for(i=0; i < nb; i++) {
			XDockWindow* eachIcon = ((XDockWindow*)windows_list->ItemAt(i));
			
			if((eachIcon)) {
			
			
			// RACCOURCI
				storage_shortcut_name.RemoveAll(storage_shortcut_name);
				storage_shortcut_name << "shortcut_" << i;
				
				shortcut_path.RemoveAll(shortcut_path);
				shortcut_path << eachIcon->ref_shortcut_path;
				preferencesMsg->AddString(storage_shortcut_name.String(), shortcut_path.String());
				#if defined(DEBUG)
				printf("shortcut '%s' saved to '%s'\n'", shortcut_path.String(), storage_shortcut_name.String());
				#endif
			// ICONE	
				storage_icon_name.RemoveAll(storage_icon_name);
				storage_icon_name << "icon_" << i;
				icon_path.RemoveAll(icon_path);
				icon_path << eachIcon->ref_icon_path;
				preferencesMsg->AddString(storage_icon_name.String(), icon_path.String());
			
			// TRANSPARENCE	
				bool trans = eachIcon->bool_transparency;
				preferencesMsg->AddBool("transparency",trans);
				
				const char* transparent_char;
				if(trans == true)
					transparent_char="yes";
				else
					transparent_char="no";
					
				BString preference;
				preference << "pref_" << i;	
				bool pref = eachIcon->view->is_pref_icon;
				preferencesMsg->AddBool(preference.String(),pref);
				
				BString trash;
				trash << "trash_" << i;	
				bool corbeille = eachIcon->view->is_trash_icon;
				preferencesMsg->AddBool(trash.String(),corbeille);
				
				#if defined(DEBUG)
				printf("icon '%s' saved to '%s', with transparency '%s'\n", icon_path.String(), storage_icon_name.String(), transparent_char);
				#endif
			}
		}
		
		
		
		
		// recherche du repertoire de l'appli
		BPath SettingsPath;
		find_directory(B_USER_SETTINGS_DIRECTORY, &SettingsPath);
		
		//création du répertoire
		SettingsPath.Append("XRAYDock",true);
		create_directory(SettingsPath.Path(),0777);
		
		const char *configFile;
		configFile = "sysinit";
				
			SettingsPath.Append(configFile);
			BFile file(SettingsPath.Path(),B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);		
			if (file.InitCheck() == B_OK)
				preferencesMsg->Flatten(&file);
	
	

}




void BackWindow::LoadInitSettings(BMessage* preferencesMsg)
{

if(LockLooper()) {	
	int32 i;
	BString storage_shortcut_name;
	BString storage_icon_name;
	BPath SettingsPath;
	find_directory(B_USER_SETTINGS_DIRECTORY, &SettingsPath);
	SettingsPath.Append("XRAYDock",true);
	{
		SettingsPath.Append("sysinit");
		
		
		BFile file;
		if (file.SetTo(SettingsPath.Path(),B_READ_ONLY) == B_OK)
		{

			preferencesMsg->Unflatten((BDataIO*)&file);
		
				int32 icon_size_, icon_expanded_size;
				float ratio, text_size;
				if(preferencesMsg->FindInt32("icon_size",&icon_size_) == B_OK)
					((XDockApplication*)be_app)->icon_size = icon_size_;
				if(preferencesMsg->FindInt32("icon_expanded_size",&icon_expanded_size) == B_OK)
					((XDockApplication*)be_app)->icon_expanded_size = icon_expanded_size;
				
				int32 distance_;
				if(preferencesMsg->FindInt32("distance",&distance_) == B_OK)
				distance = distance_;
				if(preferencesMsg->FindFloat("ratio",&ratio) == B_OK)
					((XDockApplication*)be_app)->expanded_ratio_size = ratio;
//				printf("icon_size : % f,  icon_expanded_size : %f, distance : %f, ratio : %f", icon_size, icon_expanded_size, distance, ratio);
				
				if(preferencesMsg->FindFloat("text_size",&text_size) == B_OK)
					font_size = text_size;
				
				
				/* recherche du nombre d'icônes */
				int32 icon_number;
				if(preferencesMsg->FindInt32("icon_number",&icon_number) != B_OK)
					icon_number = 0;
				
				
				if(preferencesMsg->FindBool("front_window",&front_window) != B_OK)
					front_window = true;

				if(preferencesMsg->FindInt32("text_feel",&text_feel) != B_OK)
					text_feel = 0;
				
				#if defined(DEBUG)
				printf("found : %d icons\n", (int)icon_number);
				#endif
				/* maintenant, création des icônes */	
				if(icon_number >= 0)
				for(i=0; i< icon_number; i++) {
					storage_shortcut_name.RemoveAll(storage_shortcut_name);
					storage_shortcut_name << "shortcut_" << i;
					const char *shortcut_path;
					preferencesMsg->FindString(storage_shortcut_name.String(),&shortcut_path);
					#if defined(DEBUG)
					printf("shortcut '%s' loaded in '%s'\n'", shortcut_path, storage_shortcut_name.String());
					#endif
					storage_icon_name.RemoveAll(storage_icon_name);
					storage_icon_name << "icon_" << i;
					const char *icon_path;
					preferencesMsg->FindString(storage_icon_name.String(),&icon_path);
					#if defined(DEBUG)
					printf("icon '%s' loaded in '%s'\n'", icon_path, storage_icon_name.String());
					#endif
					
					
					AddIcon(icon_path, shortcut_path);
					// pas besoin de la transparence, elle est recalculée à chaque fois.
					bool pref = false;
					BString preference;
					preference << "pref_" << i;
					preferencesMsg->FindBool(preference.String(),&pref);
					((XDockWindow*)windows_list->ItemAt(i))->view->is_pref_icon = pref;
					
					bool corbeille = false;
					BString trash;
					trash << "trash_" << i;
					preferencesMsg->FindBool(trash.String(),&corbeille);
					((XDockWindow*)windows_list->ItemAt(i))->view->is_trash_icon = corbeille;
					
				}
				
				
				
				
			const char * a;
			const char * b;
			if(preferencesMsg->FindString("derniers_raccourcis", &a) ==  B_OK) {
				raccourci << a;
			}
			else
				dernier_dossier_raccourcis = "";
			if(preferencesMsg->FindString("derniers_icones",&b) ==  B_OK) {
				icone << b;
			}
			else
				dernier_dossier_icone = "";				
				
				
				
					
				
				#if defined(DEBUG)
				printf("\ndernier icones : %s\n", dernier_dossier_icones);
				#endif
				
		}
		else {
			// initialisation au cas ou sysinit n'existe pas :
/*			style_titled = true;
			style_bordered = false;
			style_no_border = false;
			centered_on_top = false;
			free_position = true;
			centered_on_bottom = false;
			front_window = true;*/
		}
	}
	
}
UnlockLooper();

}

void
BackWindow::PreferencesWindow()
{
	BPoint		pt;
	ulong		button;
	backview->GetMouse(&pt, &button);
	
	printf("preferences\n");
				BScreen screen;
				if(preferenceswindowouverte == false) {
					preferenceswindow = new DockPreferencesWindow(BRect(screen.Frame().left+5,screen.Frame().bottom - 534,170,screen.Frame().bottom-96));
					if(pt.x < screen.Frame().right - 169)
						preferenceswindow->MoveTo(pt.x, screen.Frame().bottom-614);
					else
						preferenceswindow->MoveTo(screen.Frame().right - 170, screen.Frame().bottom-614);
					preferenceswindow->Show();
					preferenceswindow->SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
					preferenceswindowouverte = true;
				}
	return;
}
void
BackWindow::DernierChemin(const char* chemin){
	dernier_dossier_raccourcis = chemin;
	return;	
}
void
BackWindow::CheckIfPrefIconExist(){
	
	// test pour savoir si un icone est defini en tant qu'icone de preference
	//sinon faire réapparaitre les boutons de preferences
	int i;
	BackWindow *backWin;
	backWin = ((XDockApplication*)be_app)->backWindow;
	int32 index_lastitem = backWin->windows_list->IndexOf(backWin->windows_list->LastItem());
	bool vrai = false;
		for(i=0; i<index_lastitem+1; i++){
				if(((XDockWindow *)backWin->windows_list->ItemAt(i))->view->is_pref_icon == true){
					vrai=true;
					break;
				}
				else
					vrai=false;
		}
		if (vrai == true){
			((XDockApplication*)be_app)->ShowPrefButtons(false);
		}
		else{
			((XDockApplication*)be_app)->ShowPrefButtons(true);
		}
	
}


status_t
BackWindow::StoreIcon(entry_ref ref, const char *nom)
{
				BPath *path = new BPath(&ref);
				BBitmap *icon_to_store;
				icon_to_store = BTranslationUtils::GetBitmap(path->Path());
				StoreIdealBitmap(icon_to_store, nom, "image/x-be-bitmap", 96); // taille maxi et ratio maxi dans les settings
				delete icon_to_store;
				return B_OK;
}

static type_code
mime2type(const char *mime)
{
	// j'ai repiqué cette fonction je ne sais plus ou, désolé.
	BTranslatorRoster*
	roster = BTranslatorRoster::Default();

	translator_id * list = NULL;
	int32 count = 0;
	if (roster && roster->GetAllTranslators(&list, &count) == B_OK) {
		for (int i= 0; i < count; i++) {
			int32 num_output = 0;
			const translation_format * output_formats = NULL;
			if (roster->GetOutputFormats(list[i], &output_formats, &num_output) == B_OK) {
				for (int o= 0; o < num_output; o++) {
					if (strcmp(mime, output_formats[o].MIME) == 0)
						return output_formats[o].type;
	}	}	}	}
	return 0;
}


void
BackWindow::StoreIdealBitmap(BBitmap *bitmap, const char *filename, const char *mime, int largeur)
   {
// fichier à créer :
    BFile file(filename, B_CREATE_FILE | B_WRITE_ONLY);
// vue et icones sauvegardées :	
	BView		*offView = NULL;
	BBitmap		*icon_ = NULL;

// adapter la taille :

	// detection du rapport hauteur/largeur de l'image d'origine
	uint16 taille_origine_bitmap_X, taille_origine_bitmap_Y;
	float rapport_X_Y;
	taille_origine_bitmap_X = (uint16)(bitmap->Bounds().Width());
	taille_origine_bitmap_Y = (uint16)(bitmap->Bounds().Height());
	rapport_X_Y = (float)taille_origine_bitmap_X/taille_origine_bitmap_Y;
	
	uint16 taille_icone_desiree_X;
	uint16 taille_icone_desiree_Y;

// on entre la taille que l'on veut pour les images sauvegardées
	taille_icone_desiree_X = (uint16)largeur;
// calcul de la hauteur en fonction du rapport de l'image d'origine
	taille_icone_desiree_Y = (uint16)(taille_icone_desiree_X/rapport_X_Y);

	
// creation de l'icone et de la vue
	offView = new BView(BRect(0,0,taille_icone_desiree_X-1,taille_icone_desiree_Y-1),"hiddenview",B_FOLLOW_NONE,B_WILL_DRAW);	
	icon_ = new BBitmap(BRect(0,0,taille_icone_desiree_X-1,taille_icone_desiree_Y-1),bitmap->ColorSpace(),true,true);

// remplissage avec du transparent
	//size;
	int32 size = (int32)(((icon_->Bounds()).Width()+1) * ((icon_->Bounds()).Height()+1));
	for(int32 pixel=0;pixel<size;pixel++)
		*((int8 *)(icon_->Bits()) + pixel) = B_TRANSPARENT_MAGIC_CMAP8;
	
	
	// icon
	icon_->Lock();
	icon_->AddChild(offView);
	offView->DrawBitmapAsync(bitmap,BRect(0,0,taille_icone_desiree_X,taille_icone_desiree_Y));
	offView->Sync();
	icon_->Unlock();


   BTranslatorRoster *roster = BTranslatorRoster::Default();
     BBitmapStream stream(icon_);
      roster->Translate(&stream, NULL, NULL, &file, mime2type(mime));
      
      // attribution du type bitmap au fichier :
      			BNode Node(filename);
				BNodeInfo NodeInfo(&Node);
				NodeInfo.SetType(mime);
  
}

BString
BackWindow::generateur_nom_fichier(){
	BString aleatoire;
	bool existe = true;
	int alea;
	while(existe == true)
	{
		srand(time(NULL));
		
		alea = rand()%1000000000;
		//alea = rand()%6; // pour test
		BPath SettingsPath;
		find_directory(B_USER_SETTINGS_DIRECTORY, &SettingsPath);
		SettingsPath.Append("XRAYDock",true);
		BString al;
		al << alea;
		SettingsPath.Append(al.String());
		aleatoire.RemoveAll(aleatoire);
		//aleatoire << "/boot/home/config/settings/XRAYDock/" << alea;
		aleatoire << SettingsPath.Path();
		
		// on s'assure que le nom n'est pas deja pris
		BEntry entry(aleatoire.String(), true);
		if (entry.Exists() == true)
			existe = true;
		else{
			existe = false;
			
			break;
		}
		
	}
	return aleatoire;
	
}
