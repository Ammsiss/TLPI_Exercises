#!/bin/zsh

for name in $(echo /dev/*); do
  INODE=$(echo "$name" | xargs stat -c"%i")
  if [[ $INODE -eq "4" ]]; then
      echo "Name: $name, Inode: $INODE"
  fi
done
