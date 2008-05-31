/* DockImageFilter.cpp par etno 2008 */

#include "DockImageFilter.h"
#include <string.h>

DockImageFilter::DockImageFilter()
{
}

DockImageFilter::~DockImageFilter()
{
}

bool
DockImageFilter::Filter(const entry_ref*, BNode*, struct stat*, const char* mimetype)
{
	if (strcmp("application/x-vnd.Be-directory", mimetype) == 0)
  		return true;
	else if (strcmp("application/x-vnd.Be-volume", mimetype) == 0)
  		return true;
	/* maintenant on compare les premières lettres du
	mimetype pour voir si c'est une image*/
	else if (strncmp(mimetype, "image/", 6) == 0)
		return true;
	else
		return false;
}
