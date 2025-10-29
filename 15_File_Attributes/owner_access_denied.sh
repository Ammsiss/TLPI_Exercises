#!/bin/bash

file=myfile
PERMISSIONS=(000 100 200 300 400 500 600 700)

echo "Clearing subshell umask and creating file..."

umask 000
touch ${file}

echo "File's permissions now looks like -> 0066"

echo "Removing owner read and write permissions..."

chmod u-rw ${file}

# We are part of the group that owns the file with read
# and write permissions but becuase we are also the owner
# of the file we get the less permissive owner permissions.

echo "Attempting to access file..."

cat ${file} # Permission deined

# We are still the owner of the file so we can delete it even
# if the sticky bit is set of the parent directory.
#
# rm will still warn about deleting a "write-protected" file
# i.e. one that you do not have write permissions for.
#
# Use -f to suppress warnings about write-protected files.

echo "Removing file..."

rm -f ${file}

