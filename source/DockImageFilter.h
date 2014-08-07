/* DockImageFilter.h par etno 2008 */


#ifndef DOCK_IMAGE_FILTER_H
#define DOCK_IMAGE_FILTER_H


#include <FilePanel.h>

class DockImageFilter : public BRefFilter
{
	public:	DockImageFilter();
		virtual ~DockImageFilter();
		bool Filter(const entry_ref*, BNode*, struct stat_beos*, const char* mimetype);
};

#endif	//DOCK_IMAGE_FILTER_H
