/* XDockView.cpp par etno 2008 */


#include <Bitmap.h>
#include <TranslationUtils.h>
#include <experimental/BitmapTools.h>
#include <Window.h>
#include <Entry.h>
#include <FilePanel.h>
#include <stdlib.h>
#include <NodeInfo.h>
#include <Path.h>
#include <Roster.h>
#include <Region.h>
#include <Alert.h>
#include <FindDirectory.h>
#include <stdio.h>
#include <Application.h>
#include <TranslatorFormats.h>


#ifndef XDOCK_VIEW_H
#include "XDockView.h"
#endif
#ifndef XDOCK_WINDOW_H
#include "XDockWindow.h"
#endif
#ifndef XDOCK_APP_H
#include "XDockApp.h"
#endif
#ifndef BACK_WINDOW_H
#include "BackWindow.h"
#endif
#ifndef DOCK_ICON_MENU_H
#include "DockIconMenu.h"
#endif
#include <stdio.h>
#include <ToolTip.h>



XDockView::XDockView(BRect rect, const char *icon_p, const char *shortcut_p)
	   	   : BView(rect, "", B_FOLLOW_ALL, B_SUBPIXEL_PRECISE | B_PULSE_NEEDED | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS | B_WILL_DRAW)
{
	// definition des liens vers les raccourcis et les icones
	#if defined(DEBUG)
	printf("%s\n", icon_p);
	#endif
	shortcutPath << shortcut_p;
	iconPath << icon_p;
	

	
	icon_size = ((XDockApplication*)be_app)->icon_size;
	icon_expanded_size = ((XDockApplication*)be_app)->icon_expanded_size;
	icon_space = ((XDockApplication*)be_app)->icon_space;
	
	// recuperation de l'icone
	icon = BTranslationUtils::GetBitmap(icon_p);	
	
	SetDoubleBuffering(B_UPDATE_RESIZED | B_UPDATE_INVALIDATED | B_UPDATE_SCROLLED | B_UPDATE_EXPOSED);
	inside_view = false;
	
// ajustement couleur fond de vue doit etre la meme que dans draw
	SetHighColor(0,0,0,0);
	rgb_color color = {0,0,0,0};
	SetViewColor(color);
	
	




}

XDockView::~XDockView()
{

}

void
XDockView::Pulse()
{
	BPoint		ptv;
	ulong		button;
	GetMouse(&ptv, &button);
		
	if( ( (ptv.x > Bounds().left + 1) && (ptv.x < Bounds().right - 1) )
		&& ((ptv.y > Bounds().top + 1) && (ptv.y < Bounds().bottom - 1))) {
		inside_view = true;	
	}
	else{
		
		inside_view = false;
	}
	

}


void
XDockView::Draw(BRect) {
	
	SetHighColor(0,0,0,0);
	blend_bitmap_color(icon,B_TRANSPARENT_32_BIT , 0);//B_TRANSPARENT_COLOR
	DrawBitmapAsync(icon, Bounds());

}




void
XDockView::MouseDown(BPoint pt)
{
	BRect rec(this->Bounds());
	uint32 button;
	GetMouse(&pt, &button);
	
	
	switch(button)
	{
		case B_PRIMARY_MOUSE_BUTTON:
		{
		if (rec.Contains(pt)) {	
			// detection du clic souris
			while (true) {
				BPoint pt1;
				uint32 buts;
				GetMouse(&pt1, &buts);
				if (!buts) {
					goto punt;
				}
				if (pt1 != pt) {
						break;
				}
			// vitesse
			snooze(40000);
			}
			BMessage msg(B_SIMPLE_DATA);
			XDockWindow *window = ((XDockWindow*)Window());
			float index_item_ = ((XDockApplication*)be_app)->backWindow->windows_list->IndexOf(window);
			
			msg.AddString("be:types", B_FILE_MIME_TYPE);
			msg.AddString("be:clip_name", shortcutPath.String());
			msg.AddString("be:filetypes", "application/x-vnd.Be-symlink");
			
			msg.AddInt32("be:actions", B_LINK_TARGET);
			msg.AddInt32("be:actions", B_TRASH_TARGET);
			msg.AddFloat("index_item", index_item_);
			BString type;
			type << "icone_deplacee";
			msg.AddString("drag_type", type.String());
			
			
			/* la partie qui suit nous permet de deplacer le nom vers
			une fenetre de terminal par exemple,
			on pourrait aussi creer un lien sur le bureau etc... */
			entry_ref ref;
			BPath path(shortcutPath.String());
			get_ref_for_path(path.Path(), &ref);
			msg.AddRef("refs",&ref);
			//BBitmap * bmp = new BBitmap(icon->Bounds(), icon->ColorSpace(), true);
			int largeur_ = (int)( ((XDockApplication*)be_app)->icon_size + ((XDockApplication*)be_app)->icon_size*((XDockApplication*)be_app)->expanded_ratio_size );
			BBitmap * bmp = new BBitmap(BRect(0,0,largeur_,largeur_), icon->ColorSpace(), true);
			BBitmap * icon_ = new BBitmap(icon->Bounds(), icon->ColorSpace(), true);
			memcpy(icon_->Bits(), icon->Bits(), icon->BitsLength());
			bmp->Lock();
			BView *offscreen_view =new BView(bmp->Bounds(), "",B_FOLLOW_NONE, 0);		
			bmp->AddChild(offscreen_view);
			
                       
                       BPicture *picture;
							
							// preparation du fond :
							offscreen_view->BeginPicture( new BPicture() );
							offscreen_view->SetHighColor(B_TRANSPARENT_COLOR);
							offscreen_view->SetDrawingMode(B_OP_OVER);
							// on color le fond transparent
							blend_bitmap_color(icon_, B_TRANSPARENT_COLOR, 0);
							// on dessine le BBitmap dans le BPicture
							offscreen_view->DrawBitmapAsync( icon_, offscreen_view->Bounds() );
							offscreen_view->Sync();
							// Fin du dessin du BPicture
							picture = offscreen_view->EndPicture();
							offscreen_view->SetHighColor(0,0,0,0);
                        	offscreen_view->FillRect(bmp->Bounds());
                        
                        // maintenant le niveau de dessin de l'icone
						offscreen_view->SetHighColor(0, 0, 0, 128);
						offscreen_view->SetBlendingMode(B_CONSTANT_ALPHA,B_ALPHA_COMPOSITE);
						//offscreen_view->SetDrawingMode(B_OP_BLEND);
						offscreen_view->SetDrawingMode(B_OP_ALPHA); // plus transparent
                        // on dessine l'icone dans la vue
                       // blend_bitmap_color(icon_, B_TRANSPARENT_COLOR, 0);
                        offscreen_view->DrawBitmap(icon_, offscreen_view->Bounds());
                        offscreen_view->Sync();
                        offscreen_view->ResizeTo(largeur_, largeur_);
			
						
						bmp->RemoveChild(offscreen_view);
                        bmp->Unlock();
                        delete icon_;
                        delete picture;
			// ca c'est ci on utilise pas la methode de la vue
			//blend_bitmap_color(icon, B_TRANSPARENT_COLOR, 0);
			//memcpy(bmp->Bits(), icon->Bits(), icon->BitsLength());
			
			
			//BPoint ptx(pt.x-rec.left, pt.y-rec.top);
			BPoint ptx(pt.x, pt.y);
			// si on ne souhaite pas d'effet transparent :
			//DragMessage(&msg, icon_, B_OP_ALPHA, ptx, Window());
			// sinon :
			DragMessage(&msg, bmp, B_OP_ALPHA, ptx, Window());

/* clipboard (pour le lien?)*/
//      		be_clipboard->Lock(); 
//       		be_clipboard->Clear(); 
//       		BMessage  *clipMessage; 
//			clipMessage = be_clipboard->Data(); 
//       		clipMessage->AddData("text/plain", B_MIME_TYPE, shortcutPath.String(), strlen(shortcutPath.String()));
//			be_clipboard->Commit(); 
//       		be_clipboard->Unlock();  		
       		
       		return;
		}
punt:			
			if( (is_pref_icon == false) && (is_trash_icon == false)){
				Launch();	
			}
			else{
				if(is_pref_icon == true)
				BMessenger(((XDockApplication*)be_app)->backWindow).SendMessage(OPEN_SETTINGS_WINDOW);
				else if(is_trash_icon == true){
					BPath trash;
					entry_ref ref;
					find_directory(B_TRASH_DIRECTORY, &trash);
					get_ref_for_path(trash.Path(), &ref);
					BMessage msg(B_REFS_RECEIVED);
					msg.AddRef("refs",&ref);
		
					BFile *file;
					file = new BFile("/boot/beos/system/Tracker",B_READ_ONLY);
					if(file) {
						be_roster->Launch("application/x-vnd.Be-TRAK", &msg);
					}
					
						
				}
			}	
		break;
		}
		case B_SECONDARY_MOUSE_BUTTON:
		{
			DockIconMenu *dockiconmenu = new DockIconMenu();
			if(is_pref_icon == true){
				dockiconmenu->set_as_pref_icon->SetMarked(true);
				dockiconmenu->set_as_trash_icon->SetMarked(false);
				dockiconmenu->is_pref_menu = true;
				dockiconmenu->Construction(true); // creation menu preferences
			}
			else{
				if(is_trash_icon == true){
					dockiconmenu->set_as_trash_icon->SetMarked(true);
					dockiconmenu->set_as_pref_icon->SetMarked(false);
					dockiconmenu->is_trash_menu = true;
					dockiconmenu->ConstructionTrash(true); // creation menu trash
					
				}
				else {
				dockiconmenu->set_as_pref_icon->SetMarked(false);
				dockiconmenu->set_as_trash_icon->SetMarked(false);
				dockiconmenu->is_pref_menu = false;
				dockiconmenu->is_trash_menu = false;
				dockiconmenu->Construction(false); // creation menu normal
				}
			}
			BMenuItem *choice;
			choice = dockiconmenu->Go(ConvertToScreen(pt), true, false, false);
				if(choice)
					BMessenger(this).SendMessage(choice->Message());
			delete dockiconmenu;
		break;
		}
		
	}
	
}




#include <Button.h>

void
XDockView::MessageReceived(BMessage *message)
{
	entry_ref shortcut_ref;
	entry_ref icon_ref;
	BAlert *alert;

	XDockWindow *window = ((XDockWindow*)Window());
	// voici la formule pour deplacer une icone à la place d'une autre :
			XDockWindow *windowToMove;
			BackWindow *backWin;
			backWin = ((XDockApplication*)be_app)->backWindow;
			
			int32 index_item_ = backWin->windows_list->IndexOf(window);
			
			windowToMove = (XDockWindow *)backWin->windows_list->ItemAt(index_item_) ;
			XDockWindow *windowBefore;
			windowBefore = (XDockWindow *)backWin->windows_list->ItemAt(index_item_ - 1);
			XDockWindow *windowAfter;
			windowAfter = (XDockWindow *)backWin->windows_list->ItemAt(index_item_ + 1);
			
			
	
	
			int32 index_lastitem = backWin->windows_list->IndexOf(backWin->windows_list->LastItem());
			int32 counter;
	BString i_to_rem;
	switch(message->what) {
		case DOCK_TRASH_ICON:
			int k;
			for(k=0; k<index_lastitem+1; k++){
				if(((XDockWindow *)backWin->windows_list->ItemAt(k))->view->LockLooper()){
				if(((XDockWindow *)backWin->windows_list->ItemAt(k))->ChildAt(0) != this){
					
					((XDockWindow *)backWin->windows_list->ItemAt(k))->view->is_trash_icon = false;
					}
				}
				//else
					
				((XDockWindow *)backWin->windows_list->ItemAt(k))->view->UnlockLooper();
			}
			if(is_trash_icon == false)
				is_trash_icon = true;
			else if(is_trash_icon == true)
				is_trash_icon = false;
			
			
			break;
	
		case DOCK_PREF_ICON:
			// modification de la valeur is_pref_icon pour toutes les icones exceptée celle-ci
			int j;
			for(j=0; j<index_lastitem+1; j++){
				if(((XDockWindow *)backWin->windows_list->ItemAt(j))->view->LockLooper()){
				if(((XDockWindow *)backWin->windows_list->ItemAt(j))->ChildAt(0) != this){
					
					((XDockWindow *)backWin->windows_list->ItemAt(j))->view->is_pref_icon = false;
					}
				}
				//else
					
				((XDockWindow *)backWin->windows_list->ItemAt(j))->view->UnlockLooper();
			}
			if(is_pref_icon == false)
				is_pref_icon = true;
			else if(is_pref_icon == true)
				is_pref_icon = false;
			
			backWin->CheckIfPrefIconExist();
			break;
		case DOCK_REMOVE:
			
			alert=new BAlert("Are you sure ?","Are you sure you want to delete this ?","No","Yes",NULL,B_WIDTH_AS_USUAL,B_WARNING_ALERT);
			int32 buttonIndex;
			buttonIndex = alert->Go();
             		if ( buttonIndex == 1)
            		{
						
							int verif;
							verif = backWin->windows_list->CountItems();
							if(verif>1) { // c'est à dire si il reste plus d'un icone
								int i;
								for(i=index_item_; i<index_lastitem+1; i++){
									((XDockWindow *)backWin->windows_list->ItemAt(i + 1))->MoveBy(-icon_size, 0);
									
								}
								(XDockWindow *)backWin->windows_list->RemoveItem(index_item_);
								counter = backWin->windows_list->CountItems();
								for(i=0; i<counter; i++){
									((XDockWindow *)backWin->windows_list->ItemAt(i))->MoveBy(icon_size/2, 0);
									
								}
								
								i_to_rem << "rm " << window->ref_icon_path.String();
								printf("effacé : %s\n", window->ref_icon_path.String());
								
								system(i_to_rem.String());
								Window()->Close();
							}
            		}
            		else{
            			break;
            		}
			break;
		case DOCK_MOVE_LEFT:
			if(windowBefore){
			windowToMove->MoveBy(-icon_size, 0);
			windowBefore->MoveBy(icon_size, 0);
			(XDockWindow *)backWin->windows_list->SwapItems(index_item_, index_item_ - 1);
			}//MoveLeft();
			break;
		case DOCK_MOVE_RIGHT:
			if(windowAfter){
			windowToMove->MoveBy(icon_size, 0);
			windowAfter->MoveBy(-icon_size, 0);
			(XDockWindow *)backWin->windows_list->SwapItems(index_item_, index_item_ + 1);
			}//MoveRight();
			break;
		case DOCK_MODIFY_SHORTCUT:
			((XDockApplication*)be_app)->backWindow->shortcutPanel->SetTarget(this);
			((XDockApplication*)be_app)->backWindow->shortcutPanel->Show();
			
			break;
		case DOCK_MODIFY_ICON:
			((XDockApplication*)be_app)->backWindow->iconPanel->SetTarget(this);
			((XDockApplication*)be_app)->backWindow->iconPanel->Show();
			
			break;
		case DOCK_CHANGE_SHORTCUT:
			if (message->FindRef("refs", &shortcut_ref) == B_OK) {
				ChangeShortcut(&shortcut_ref);
				
			}
			break;
		case DOCK_CHANGE_ICON:
			if (message->FindRef("refs", &icon_ref) == B_OK) {
				ChangeIcon(icon_ref);
				
			}
			break;
	}
}

void
XDockView::ChangeShortcut(entry_ref *ref)
{
	XDockWindow *window = ((XDockWindow*)Window());
	BPath *path = new BPath(ref);
                			shortcutPath.RemoveAll(shortcutPath);
                			shortcutPath << path->Path();
                				char name[B_FILE_NAME_LENGTH];
								BEntry entry(shortcutPath.String());
								entry.GetName(name);
								
				path->GetParent(path);
				((XDockApplication*)be_app)->backWindow->raccourci.RemoveAll(((XDockApplication*)be_app)->backWindow->raccourci);
				((XDockApplication*)be_app)->backWindow->raccourci << path->Path();
				window->ref_shortcut_path = shortcutPath;
				((XDockApplication*)be_app)->backWindow->shortcutPanel->SetPanelDirectory(((XDockApplication*)be_app)->backWindow->raccourci.String());
				
}
void
XDockView::ChangeIcon(entry_ref ref)
{
	XDockWindow *window = ((XDockWindow*)Window());
	BString nom;
    nom << ((XDockApplication*)be_app)->backWindow->generateur_nom_fichier().String();
    if( ((XDockApplication*)be_app)->backWindow->StoreIcon(ref, nom.String()) == B_OK) {
    		BPath *path = new BPath(&ref);
    		
    		
                			iconPath.RemoveAll(iconPath);
                			iconPath << nom.String();
                			icon = BTranslationUtils::GetBitmap(nom.String());
                			
                			if( (icon->Bounds().Width() * icon->Bounds().Height()) > (200 * 200) )
								{
									BAlert *alert;
									alert = new BAlert("Transparency", "image resolution is too high, transparency,
																disabled for this icon.", "Ok");
									alert->Show();
									window->clipping_allowed = true;
									// retailler la fenetre à sa taille maxi
									window->ResizeTo(((XDockApplication*)be_app)->icon_expanded_size , ((XDockApplication*)be_app)->icon_expanded_size );
									window->ResizeOnScaledView(Bounds());
									//puis
									window->clipping_allowed = false;
									
									DrawBitmapAsync(icon, Bounds());
									Sync();
								
								}
								else {
									window->clipping_allowed = true;
									window->ResizeOnScaledView(Bounds());
								}
				path->GetParent(path);
				
				//if(LockLooper())
					((XDockApplication*)be_app)->backWindow->icone.RemoveAll(((XDockApplication*)be_app)->backWindow->icone);
					((XDockApplication*)be_app)->backWindow->icone << path->Path();
					((XDockApplication*)be_app)->backWindow->addiconPanel->SetPanelDirectory(((XDockApplication*)be_app)->backWindow->icone.String());
			
				window->ref_icon_path = iconPath;
				//UnlockLooper();
				if(icon) {
					Draw(Bounds());
					//DrawBitmap(icon, Bounds());
				}
    }
}

void
XDockView::Launch()
{
	entry_ref ref;
	BPath path;
	get_ref_for_path(shortcutPath.String(), &ref);
	BEntry entry(&ref,true);

				BNode Node(&entry);
				BNodeInfo NodeInfo(&Node);
				char mime[B_MIME_TYPE_LENGTH+1];
				NodeInfo.GetType(mime);
				BMimeType Pref(mime);
	
				BMessage msg(B_REFS_RECEIVED);
				msg.AddRef("refs",&ref);
		
				BFile *file;
				file = new BFile("/boot/beos/system/Tracker",B_READ_ONLY);
				if(file) {
					be_roster->Launch("application/x-vnd.Be-TRAK", &msg);
					
				}
				else
					be_roster->Launch(Pref.Type(), &msg);
					/* dans ce cas, certains fichiers,
					notamment si ils sont sur une partition non BFS
					ne se lanceront pas automatiquement */
				delete file;
				msg.MakeEmpty();
			/* et voila !!! */
		
	return;
}
