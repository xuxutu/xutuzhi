Partner Security Bulletin Patch Instructions

# HOW TO APPLY THE PATCHES

1. Check out the appropriate version of Android from AOSP, and apply patches
from previous bulletins if applicable.
2. Set the location of the patch subdirectory in an environment variable, for
example:

$ export DIR=/path/to/bulletin_YYYY_MM/patches/android-X.Y.Z_rW

3. Finally, to apply all of the patches, the following command will iterate
across all of the projects in repo, and apply all of the patches in the
appropriate order.

$ repo forall -epvc '[ ! -d $DIR/$REPO_PROJECT ] || git am $DIR/$REPO_PROJECT/*.bulletin.patch'
