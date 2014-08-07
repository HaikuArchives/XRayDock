/* DockPreferencesWindow.h par etno 2008 */

#include <Window.h>
#include <Slider.h>

#ifndef DOCK_PREFERENCES_WINDOW_H
#define DOCK_PREFERENCES_WINDOW_H

class DockSlider : public BSlider
{
	public:		DockSlider(BMessage *message, long int minValue, long int maxValue, int incrementalValue, const char *low_limit_label, const char *high_limit_label);
			~DockSlider();
			
			virtual void	DrawThumb();
};

class DockPreferencesWindow : public BWindow
{
	public:		DockPreferencesWindow(BRect rect);
			~DockPreferencesWindow();
			
			virtual bool	QuitRequested();
			
			BView *preferenceswindowview;
			BSlider *zoom_distance_slider, *zoom_power_slider, *icon_size_slider, *text_size_slider;
			
	
};


#endif
