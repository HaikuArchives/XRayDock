## BeOS Generic Makefile v2.0 ##

## Fill in this file to specify the project being created, and the referenced
## makefile-engine will do all of the hard work for you.  This handles both
## Intel and PowerPC builds of the BeOS.

## Application Specific Settings ---------------------------------------------
ifeq ($(wildcard /boot/develop/lib/*/libzeta.so), )
OS = _BEOS_
else
OS = _ZETA_
endif

# specify the name of the binary
#NAME= XRayDock_ii_g_1
NAME= XRayDock_ii_`makefile-date`

#APP_VERSION = -app II g 1 f 0 -short "XRayDock II.g.1" -long "EtnoApps XRayDock II.g.1 written by etno"`echo "merde"`
# specify the type of binary
#	APP:	Application
#	SHARED:	Shared library or add-on
#	STATIC:	Static library archive
#	DRIVER: Kernel Driver
TYPE= APP

#	specify the source files to use
#	full paths or paths relative to the makefile can be included
# 	all files, regardless of directory, will have their object
#	files created in the common object directory.
#	Note that this means this makefile will not work correctly
#	if two source files with the same name (source.c or source.cpp)
#	are included from different directories.  Also note that spaces
#	in folder names do not work well with this makefile.
SRCS= 	XDockView.cpp \
		XDockWindow.cpp \
		ButtonWindow.cpp \
		XDockApp.cpp \
		BackWindow.cpp \
		DockIconMenu.cpp \
		DockImageFilter.cpp \
		DockPreferencesWindow.cpp \
		AppWatcherWindow.cpp \
		MinimizedWindow.cpp \
		TeamContextualMenu.cpp \
		
		
#	specify the resource files to use
#	full path or a relative path to the resource file can be used.
RSRCS= xrayD.rsrc

#	specify additional libraries to link against
#	there are two acceptable forms of library specifications
#	-	if your library follows the naming pattern of:
#		libXXX.so or libXXX.a you can simply specify XXX
#		library: libbe.so entry: be
#		
#	- 	if your library does not follow the standard library
#		naming scheme you need to specify the path to the library
#		and it's name
#		library: my_lib.a entry: my_lib.a or path/my_lib.a
ifeq ($(OS), _ZETA_)
LIBS= be zeta translation experimental tracker
else
LIBS= be translation experimental tracker
endif		
#	specify additional paths to directories following the standard
#	libXXX.so or libXXX.a naming scheme.  You can specify full paths
#	or paths relative to the makefile.  The paths included may not
#	be recursive, so include all of the paths where libraries can
#	be found.  Directories where source files are found are
#	automatically included.
LIBPATHS= 

#	additional paths to look for system headers
#	thes use the form: #include <header>
#	source file directories are NOT auto-included here
SYSTEM_INCLUDE_PATHS = 

#	additional paths to look for local headers
#	thes use the form: #include "header"
#	source file directories are automatically included
LOCAL_INCLUDE_PATHS = /boot/develop/headers/be/experimental

#	specify the level of optimization that you desire
#	NONE, SOME, FULL
OPTIMIZE= 

#	specify any preprocessor symbols to be defined.  The symbols
#	will be set to a value of 1.  For example specify DEBUG if you want
#	DEBUG=1 to be set when compiling.
DEFINES=

#	specify special warning levels
#	if unspecified default warnings will be used
#	NONE = supress all warnings
#	ALL = enable all warnings
WARNINGS = ALL

#	specify whether image symbols will be created
#	so that stack crawls in the debugger are meaningful
#	if TRUE symbols will be created
SYMBOLS = 

#	specify debug settings
#	if TRUE will allow application to be run from
#	a source-level debugger
DEBUGGER = 

#	specify additional compiler flags for all files
COMPILER_FLAGS =

#	specify additional linker flags
LINKER_FLAGS =


## include the makefile-engine
include /boot/develop/etc/makefile-engine

