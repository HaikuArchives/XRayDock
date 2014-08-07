/* XDockWindow.cpp par etno 2008 */

#include <Bitmap.h>
#include <TranslationUtils.h>
#include "BitmapTools.h"
#include <Entry.h>
#include <Path.h>
#include <Alert.h>
#include <NodeInfo.h>
#include <SymLink.h>
#include <Application.h>

#include "messages.h"
#include "XDockWindow.h"
#include "XDockApp.h"
#include "DockImageFilter.h"

#include <stdio.h>

// TODO: [waddlesplash] Yet more Zeta/Dano...
//status_t ClipWindowToPicture(BPicture *picture, BPoint offset, uint32 flags);


XDockWindow::XDockWindow(BRect frame, const char *icon_path, const char *shortcut_path, bool clip)
	: BWindow(frame, icon_path, B_DOCUMENT_WINDOW, B_NOT_ZOOMABLE | B_WILL_DRAW | B_WILL_ACCEPT_FIRST_CLICK)
{
//	SetPulseRate(1000);
	ref_shortcut_path = shortcut_path;
	ref_icon_path = icon_path;

	SetLook(B_NO_BORDER_WINDOW_LOOK);
	SetFeel(B_FLOATING_ALL_WINDOW_FEEL);
	view = new XDockView(Bounds(), icon_path, shortcut_path);
	AddChild(view);
	
///	printf("index_window %d \n", index);
	
	
	clipping_allowed = false;
	bool_transparency = false;
//	if(clipping_allowed == true)
//		ClipWindowToPicture(ClippedPicture(Bounds(), icon_path),BPoint(0,0), 0);
	
	SetPulseRate(80000);

}



bool XDockWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return(true);
}

void XDockWindow::ResizeOnScaledView(BRect size_of_scaled_view) {
	// on retaille la fenetre en fonction de la taille que l'on veut donner
	// à l'icone
	ResizeTo(size_of_scaled_view.Width() , size_of_scaled_view.Height() );
	
}

#include <malloc.h>
// fonction origine :
BPicture
*XDockWindow::ClippedPicture(BRect taille, const char *path)
{
	BBitmap *Icon = BTranslationUtils::GetBitmap(path);
	// Creation de la vue temporaire
	BView	*tempView;
	tempView = new BView( taille, "", 0, 0);
	// définition du BPicture
	
	//	Ajout de la vue temporaire
	// sinon BeginPicture et EndPicture crashent
	AddChild( tempView );
	//tempView->SetDoubleBuffering(B_UPDATE_RESIZED);
	// Début du dessin du BPicture
	tempView->BeginPicture( new BPicture() );
	
	tempView->SetDrawingMode(B_OP_OVER);
	// on color le fond transparent
	//	view->SetBlendingMode(B_CONSTANT_ALPHA,B_ALPHA_COMPOSITE);
	
	blend_bitmap_color(Icon, B_TRANSPARENT_COLOR, 0);
	
	// on dessine le BBitmap dans le BPicture
	tempView->DrawBitmapAsync( Icon, taille );
	tempView->Sync();
	// Fin du dessin du BPicture
	picture = tempView->EndPicture();
	
	// Remove the temporary view
	RemoveChild( tempView);
	delete tempView;
	delete Icon;
	
	//delete picture;
	return picture;
	


	
}



void
XDockWindow::FrameResized(float, float){
	
	//if(clipping_allowed == true){
	//	ClipWindowToPicture(ClippedPicture(Bounds(), view->iconPath.String()),BPoint(0,0), 0);
	//	delete picture;
	//}
	//else {
		view->Draw(Bounds());
	//}
	
	
	return;
}


void
XDockWindow::WindowActivated(bool){
	return;
}


void
XDockWindow::MessageReceived(BMessage *message)
{
	
	switch(message->what) {
		case B_LINK_TARGET:
		break;
		case B_TRASH_TARGET:
			if( (view->is_pref_icon == false) && (view->is_trash_icon == false) ){
			BMessenger(view).SendMessage(DOCK_REMOVE);
			}
		break;
		case B_REFS_RECEIVED :
        {
            const char *type;
            if(message->FindString("drag_type", &type) != B_OK)
            // appel de refsreceived pour le drag'n'drop
            RefsReceived(message);
            break;
            
        }

	
	//case B_MOUSE_MOVED:
	//	printf("mouse moved\n");
	break;
	
	}
}
void 
XDockWindow::DispatchMessage(BMessage *message, BHandler *handler)
{
	

	if (message->WasDropped()) {
		DetachCurrentMessage();
		printf("message moved\n");

	}
	
	
	
	if(message->WasDropped() && message->what == B_SIMPLE_DATA) {
		DetachCurrentMessage();
		//
		//
		
		
		
			
		printf("message recu\n");
		message->what = B_REFS_RECEIVED;
		XDockWindow *window = ((XDockWindow*)this);
		
		
		BackWindow *backWin;
		backWin = ((XDockApplication*)be_app)->backWindow;
		int32 index_item = backWin->windows_list->IndexOf(window);
		float index;
		
		//printf()
		//ca marche pas je pense qu'il faut passer un autre argument à l'objet et verifier s'il existe
		const char *type;
		message->FindFloat("index_item", &index);
		message->FindString("drag_type", &type);
		printf("index : %f, type : %s \n", index, type);
		BString dragtype;
		dragtype << type;
		if(dragtype == "icone_deplacee") {
		if(window->view->is_trash_icon == true){
			if( (((XDockWindow *)backWin->windows_list->ItemAt(index))->view->is_trash_icon != true)
				&& (((XDockWindow *)backWin->windows_list->ItemAt(index))->view->is_pref_icon != true) )
				BMessenger(((XDockWindow *)backWin->windows_list->ItemAt(index))->view).SendMessage(DOCK_REMOVE);
		}
		else{
				
				(XDockWindow *)backWin->windows_list->SwapItems(index_item, index);
		}
		}
		
		
		PostMessage(message);
		//BWindow::DispatchMessage(message, handler);
		//delete message;
	} else {
		
		BWindow::DispatchMessage(message, handler);
		//delete message;
	}
	
}

#include <Mime.h>
#include <AppFileInfo.h>
void XDockWindow::RefsReceived(BMessage *message)
{
	/* tout d'abord, voyons si c'est une icone de la barre
	ou autre chose que l'on deplace :
	si c'est la barre, on sort */
    float index;
    if(message->FindFloat("index_item", &index) == B_OK)
    	return;
    
    
    /* sinon */
    BackWindow *backWin;
	backWin = ((XDockApplication*)be_app)->backWindow;
              
    {
    entry_ref ref;
	int32 i = 0;
	
	BString FichierI;
	BPath fpath;	
	while (message->FindRef("refs", i++, &ref) == B_OK)
		{
			if (Lock())
			{
				BEntry entry(&ref,true);
				entry.GetPath(&fpath);
				BString fichier;
				
				BNode Node(&entry);
				BNodeInfo NodeInfo(&Node);
				char mime[B_MIME_TYPE_LENGTH+1];
				NodeInfo.GetType(mime);
				BMimeType Pref(mime);
				BAlert *alert;
				if (strncmp(mime, "image/", 6) == 0){
					alert=new BAlert("Error","Use this picture to ","Add an icon","Modify this Icon","Cancel",B_WIDTH_AS_USUAL,B_INFO_ALERT);
					//alert->Show();
					int32 buttonIndex = alert->Go();
               		if ( buttonIndex == 1)
            		{
						view->ChangeIcon(ref);
            		}
            		if ( buttonIndex == 0)
            		{
					BString nom;
					nom << ((XDockApplication*)be_app)->backWindow->generateur_nom_fichier().String();
					if(LockLooper())
							if( ((XDockApplication*)be_app)->backWindow->StoreIcon(ref, nom.String()) == B_OK) {
								((XDockApplication*)be_app)->backWindow->AddIcon(nom.String(), "/boot/home");
							}
					UnlockLooper();
            		}
				}
				else {
					
					alert=new BAlert("Error","Use this path","As a new Shortcut","Cancel",NULL,B_WIDTH_AS_USUAL,B_INFO_ALERT);
					alert->Show();
					int32 buttonIndex = alert->Go();
               		if ( buttonIndex == 0)
            		{
						view->ChangeShortcut(&ref);
            		}
            		else
            		{
            			Unlock();
            			return;
            			}	
            		
				}
					
				Unlock();
				
			}
		}
	}

}
