#!/bin/bash

# The following script tests all possible user permisions for various file operations, printing
# weather the given command successfully executes or fails due to a permission issue.
#
# I did not test directories as the rule is fairly consistent for most tests i.e. execute
# permissions only except for deleting/creating a file where you also need write and listing
# contents of a directory where you need read.


# Cycle through each of the possible permission bit orientations and test a command
test_command ()
{
    local file="$1"
    local cmd="$2"
    local PERMISSIONS=(000 100 200 300 400 500 600 700)

    for perm in "${PERMISSIONS[@]}"; do
        chmod "$perm" ${file}
        echo -n "Executing '${cmd}' with $perm..."
        eval ${cmd}
        if [ $? -eq 0 ]; then
            echo "Success!"
        else
            echo "Failed!"
        fi
    done

    echo
}

file="myfile"

touch ${file}

# Test read -> 0400 min permissions needed
test_command ${file} "test -r ${file}"

# Test write -> 0200 min permissions needed
test_command ${file} "test -w ${file}"

rm ${file}
