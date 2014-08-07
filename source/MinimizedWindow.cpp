/* MinimizedWindow.cpp par etno 2008 */

#include "MinimizedWindow.h"
#include <Screen.h>
#include "BitmapTools.h"
#include <NodeInfo.h>
#include <Font.h>

#include <stdio.h>

#include "TeamContextualMenu.h"
#include "XDockApp.h"

#define INVALIDATE	'INVA'


void
MinimizedView::Draw(BRect rect)
{
//	BRect converted( ConvertToScreen(Bounds()) );
	DrawBitmap(bit, Bounds());
	SetDrawingMode(B_OP_ALPHA);
	//DrawBitmap((dynamic_cast<MinimizedWindow*>(Window()))->app_icon, BRect(0,0,rect.Width()/3,rect.Width()/3));
	
	/* barre transparente */
	rgb_color gris_transparent = {100,100,100,170};
	rgb_color blanc = {255,255,255,255};
	SetHighColor(gris_transparent);
	


	BEntry entry(window_name);
	char name[B_FILE_NAME_LENGTH];	
	entry.GetName(name);
	
	float font_size;
	if( (inside_preview == true) && (rect.Height()>=B_LARGE_ICON))
		font_size = 11;
	else
		font_size = 8;
	BFont font(be_fixed_font);
	GetFont(&font);
	
	font.SetSize(font_size);
	font_height height;
	font.GetHeight(&height);
	SetFont(&font, B_FONT_SIZE | B_FONT_FLAGS);
//	float longueur_du_nom = StringWidth(name);
// = new BString(name);
	window_name_string.RemoveAll(window_name_string);
	window_name_string << name;
	TruncateString(&window_name_string,B_TRUNCATE_MIDDLE,rect.Width()-1);
	
	
	/* dessine moi une icone */
	
	if( (inside_preview == true)/* && (rect.Height()>=B_LARGE_ICON + 10)*/){	
		/* la barre */
		FillRect(BRect(Bounds().left, Bounds().bottom -10, Bounds().right, Bounds().bottom),B_SOLID_HIGH);
		/* le texte */
		MovePenTo(Bounds().left+1/*+ B_LARGE_ICON*/,Bounds().bottom-1);
		SetHighColor(blanc);
		//DrawString(dynamic_cast<MinimizedWindow*>(Window())->window_name);
		DrawString(window_name_string.String());
		/* l'icone */
		DrawBitmap(app_icon, BRect(0,0,B_LARGE_ICON,B_LARGE_ICON));
		
	}
	else{
		/* la barre */
		FillRect(BRect(Bounds().left, Bounds().bottom -6, Bounds().right, Bounds().bottom),B_SOLID_HIGH);
		/* le texte */
		MovePenTo(Bounds().left+1,Bounds().bottom-0.5);
		SetHighColor(blanc);
		//DrawString(dynamic_cast<MinimizedWindow*>(Window())->window_name);	
		DrawString(window_name_string.String());
		/* l'icone */
		DrawBitmap(app_icon_mini, BRect(0,0,B_MINI_ICON,B_MINI_ICON));
			
	}
	/* et enfin, le texte : */
	
	
}




#include <malloc.h>

void
MinimizedView::MouseDown(BPoint pt)
{
	BRect r(this->Bounds());
	uint32 button;
	GetMouse(&pt, &button);
	
	
	switch(button)
	{
		case B_PRIMARY_MOUSE_BUTTON:
		{
		if (r.Contains(pt)) {	
			// detection du clic souris drag
			while (true) {
				BPoint pt1;
				uint32 buts;
				GetMouse(&pt1, &buts);
				if (!buts) {
					/* envoi des infos vers watch */
					BMessage msg(B_MOUSE_DOWN);
					msg.AddString("nom", sig.String());
					msg.AddInt32("id", ID);
					BMessenger(((XDockApplication*)be_app)->watch).SendMessage(&msg);
					return;
					break;
				}
				if (pt1 != pt) {
					break;
				}
			// vitesse
			snooze(40000);
			}
					
					Window()->LockLooper();
				BBitmap * bmp = new BBitmap(BRect(0,0,Bounds().Width()*2,Bounds().Height()*2), bit->ColorSpace(), true);
				BBitmap * icon_ = new BBitmap(bit->Bounds(), bit->ColorSpace(), true);
				memcpy(icon_->Bits(), bit->Bits(), bit->BitsLength());
				bmp->Lock();
				BView *_view =new BView(bmp->Bounds(), "",B_FOLLOW_NONE, 0);		
				bmp->AddChild(_view);
				BPicture *picture;
				// preparation du fond :
				_view->BeginPicture( new BPicture() );
				_view->SetDrawingMode(B_OP_OVER);
				_view->SetHighColor(0, 0, 0, 128);
				_view->FillRect(bmp->Bounds());
				picture = _view->EndPicture();
				_view->DrawBitmap( icon_, _view->Bounds() );
				_view->ResizeTo(Bounds().Width()*2, Bounds().Height()*2);
				
				bmp->RemoveChild(_view);
                bmp->Unlock();
                delete icon_;
                delete picture;
				
				// le drag
				
					BMessage mesg(B_SIMPLE_DATA);
					entry_ref ref;
					//BPath path(window_name);
					printf("sig = %s\n", sig.String());
					
					BNode node(window_name);
					BNodeInfo NodeInfo(&node);
					char mime[B_MIME_TYPE_LENGTH+1];
					NodeInfo.GetType(mime);
					BMimeType Pref(mime);
					
					
					/* ici l'ideal serait de dire:
					si ce n'est pas le tracker mais que le titre renvoi au tracker (un dossier)
					alors creer un lien vers l'application ouverte et non vers le dossier,
					mais j'ai pas le temps ! */
					BPath *path;
					
					
					if( (strncmp(mime, "image/", 6) == 0)
						|| (strncmp(mime, "document/", 9) == 0)
						|| (strncmp(mime, "audio/", 6) == 0)
						|| (strncmp(mime, "video/", 6) == 0)
						|| (strncmp(mime, "text/", 5) == 0)
						|| (strncmp(mime, "message/", 8) == 0)
						|| (strcmp(sig.String(),"application/x-vnd.Be-TRAK") == 0)
						)
						{
						path = new BPath(window_name);
						get_ref_for_path(path->Path(), &ref);
					
					
					}
					else {
						/* ici renvoyer vers l'app preferee */
						
						
						be_roster->FindApp(sig.String(), &ref);
						
						// si la signature n'est pas connue
						/*if(be_roster->FindApp(sig.String(), &ref) != B_OK) {
							app_info info;
							be_roster->GetRunningAppInfo(Team, &info);
							BEntry entry(&info.ref);
							BString nom; 
							nom.SetTo("");
							char name[64];
							entry.GetName(name);
							nom << name;
							path = new BPath(name);
							printf("name app= %s\n", name);
							get_ref_for_path(path->Path(), &ref);
						}*/
					}
						
					
					
					
					
					
					
					mesg.AddRef("refs", &ref);
					mesg.AddString("be:types", B_FILE_MIME_TYPE);
				
				
				
				//	printf("name = %s\n", ref.name);
					mesg.AddString("be:filetypes", "application/x-vnd.Be-symlink");
					mesg.AddInt32("be:actions", B_LINK_TARGET);
					BPoint ptx(pt.x, pt.y);
					DragMessage(&mesg, bmp, B_OP_BLEND, ptx, Window());
					Window()->UnlockLooper();
			
			
		}
		}
		break;
		case B_TERTIARY_MOUSE_BUTTON:
		{
		if (r.Contains(pt)) {	
			
			BMessage msg(B_RIGHT_MOUSE_DOWN);
			msg.AddString("nom", sig.String());
			msg.AddInt32("id", ID);
			//msg.AddBool("close", true);
			BMessenger(((XDockApplication*)be_app)->watch).SendMessage(&msg);
		}
		}
		break;
		case B_SECONDARY_MOUSE_BUTTON:
		{
		
			Window()->Activate(false);
			TeamContextualMenu *teammenu = new TeamContextualMenu();
			BMenuItem *choice;
			choice = teammenu->Go(ConvertToScreen(pt), true, false, false);
				if(choice){
					choice->Message()->AddInt32("id", ID);
					BMessenger(((XDockApplication*)be_app)->watch).SendMessage(choice->Message());
				
				}	
			delete teammenu;
			
		
		}
		break;
		/* j'ai decidé de ce reglage pour ne pas léser les possesseurs de souris ou pads à 2 boutons */
		/* j'ai donc decidé d'inverser les boutons 2 et 3 et d'affecter SwapFront au 1 au lieu de BringToFront*/
	}
}


void
MinimizedView::MouseMoved(BPoint, uint32 code, const BMessage *)
{

	switch(code) {
		case B_INSIDE_VIEW:
			if(Bounds().Width() != 70){
				inside_preview = true;
				if(rapport_H_L * 70 <= B_LARGE_ICON + 6){ // 6 correspond à la bande pour le nom
						if(((WatcherWindow*)Window())->vertical == true)
							ResizeTo(70,B_LARGE_ICON + 6);
						else
							ResizeTo(B_LARGE_ICON + 6,70);
					}
					else{
						if(((WatcherWindow*)Window())->vertical == true)
							ResizeTo(70,rapport_H_L * 70 );
						else
							ResizeTo(rapport_H_L * 70, 70);
						
					}
					((WatcherWindow*)Window())->ReOrganiseLesFenetres();
			
			}
			Window()->Activate(true);
			//
			break;
			
		case B_OUTSIDE_VIEW:
			
			
		case B_EXITED_VIEW:
			if(Bounds().Width() != 60){
				inside_preview = false;
				// si la fenetre est trop petite par rapport à l'icone
				if(rapport_H_L * 60 <= B_MINI_ICON + 6){ // 6 correspond à la bande pour le nom
						if(((WatcherWindow*)Window())->vertical == true)
							ResizeTo(60,B_MINI_ICON + 6);
						else
							ResizeTo(B_MINI_ICON + 6,60);
					}
					else{
						if(((WatcherWindow*)Window())->vertical == true)
							ResizeTo(60,rapport_H_L * 60 );
						else
							ResizeTo(rapport_H_L * 60, 60);
					}
				((WatcherWindow*)Window())->ReOrganiseLesFenetres();
			}
			//Window()->Activate(false);
			//
			break;
		
	}
	
	return;

}



MinimizedView::MinimizedView(BRect frame, BBitmap *bitmap, BBitmap *icon, BBitmap *icon_mini)
	: BView(frame, NULL, B_FOLLOW_NONE, B_PULSE_NEEDED | B_FULL_UPDATE_ON_RESIZE | B_WILL_DRAW)
{
	app_icon = icon;
	app_icon_mini = icon_mini;
	bit = new BBitmap(bitmap->Bounds(), bitmap->ColorSpace(), true);
	memcpy(bit->Bits(), bitmap->Bits(), bitmap->BitsLength());
	
	DrawBitmap(bit, Bounds());
	DrawBitmap(app_icon, BRect(0,0,20,20));
	
	
return;



}
MinimizedView::~MinimizedView()
{
	//free(bit);
	//delete bit;
	
	
}



void
MinimizedView::Pulse()
{

	/* envoi du message pour savoir si la fenetre que l'on
	designe est toujours ouverte */

	BMessage msg(update_Windows);
		msg.AddString("nom", sig.String());
		msg.AddInt32("id", ID);

	BMessenger(((XDockApplication*)be_app)->watch).SendMessage(&msg);
	//	delete msg;
}
void
MinimizedView::FrameResized(float, float) {

	
	
}

