#!/bin/bash

dir=testdir
file=testfile

# rwx for owner by default
echo "Creating directory..."
mkdir ${dir}

echo "Creating a file in the directory..."
touch ${dir}/${file}

echo "Removing execute permission..."
chmod u-x ${dir}

# This wont fail
echo "Displaying contents of directory..."
ls  ${dir}

# This will
echo "Attempting to access file in directory..."
cat ${dir}/${file}

echo "Restoring execute permission..."
chmod u+x ${dir}

echo "Cleaning up..."
rm -rf ${dir}

echo "Done!"
