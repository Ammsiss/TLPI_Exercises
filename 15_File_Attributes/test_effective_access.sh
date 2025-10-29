#!/bin/bash

file=TEST_FILE
perm=${1}

echo "Testing -e flag..."

./effective_access.bin -e ${file}

touch ${file} # 0644 by default, owner and group -> executing process

./effective_access.bin -e ${file}

echo

# Expected output:
#
# Fail, Success

echo "Testing -r flag..."

chmod u+r ${file}
./effective_access.bin -r ${file}

chmod u-r ${file}
./effective_access.bin -r ${file}

sudo chown root ${file} # owner -> root, group -> executing process

sudo chmod g+r ${file}
./effective_access.bin -r ${file}

sudo chmod g-r ${file}
./effective_access.bin -r ${file}

sudo chown :root ${file} # owner -> root, group -> root

sudo chmod o+r ${file}
./effective_access.bin -r ${file}

sudo chmod o-r ${file}
./effective_access.bin -r ${file}

echo

# Expected Output:
#
# Success, Fail, Success, Fail, Success, Fail

echo "Testing -wx flag..."

sudo chown ${USER}:${USER} ${file} # owner and group -> executing process

sudo chmod u+wx ${file}
./effective_access.bin -wx ${file}

sudo chmod u-x ${file}
./effective_access.bin -wx ${file}

# Expected Output:
#
# Success, Fail

# CLEANUP
sudo rm -rf TEST_FILE
