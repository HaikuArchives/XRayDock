/* MinimizedWindow.h par etno 2008 */

#ifndef MINIMIZED_WINDOW_H
#define MINIMIZED_WINDOW_H

#include <View.h>
#include <Bitmap.h>

#define	update_Windows	'upDw'
#define	B_MIDDLE_MOUSE_DOWN	'MmDw'
#define	B_RIGHT_MOUSE_DOWN	'RmDw'

class MinimizedView : public BView 
{
public:
	MinimizedView(BRect frame, BBitmap *bitmap, BBitmap *icon, BBitmap *icon_mini);
	
	~MinimizedView();
virtual void	Draw(BRect rect);	
virtual void	MouseMoved(BPoint, uint32 code, const BMessage *);
virtual void	MouseDown(BPoint pt);
virtual void	Pulse();
//virtual	void	KeyDown(const char *bytes, int32 numBytes);
//virtual	void	KeyUp(const char *bytes, int32 numBytes);
virtual void	FrameResized(float, float);

	bool inside_preview;
	BBitmap *bit;
	BBitmap *app_icon;
	BBitmap *app_icon_mini;
	MinimizedView	*minimizedview;



	BString	sig;
	int32	ID;
	team_id	Team;
	float	rapport_H_L;
	const char *window_name;
	BString window_name_string;
};



#endif
