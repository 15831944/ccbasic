# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.lualib.Debug:
/Volumes/data/github/ccbasic/buildios/src/lualib/Debug/liblualib.dylib:
	/bin/rm -f /Volumes/data/github/ccbasic/buildios/src/lualib/Debug/liblualib.dylib


PostBuild.lualib.Release:
/Volumes/data/github/ccbasic/buildios/src/lualib/Release/liblualib.dylib:
	/bin/rm -f /Volumes/data/github/ccbasic/buildios/src/lualib/Release/liblualib.dylib




# For each target create a dummy ruleso the target does not have to exist
