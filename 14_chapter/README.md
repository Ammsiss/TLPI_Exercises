## Chapter 14

### Syscalls

For information about a filesystem:

**statvfs(3)**
**fstatvfs(3)**

To mount and unmount a filesystem:

**mount(2)**
**umount(2)**
**umount2(2)**

### To wipe and repartition and mount a drive:

Use **wipefs(8)** to clear signatures then use **fdisk(8)** to create
partition table and partitions (MBR by default use 'g' to set to GPT)
then finally **mkfs(8)** to initialize partitions with a filesystem.

$ lsblk
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
sda           8:0    1  28.7G  0 disk
├─sda1        8:1    1     5G  0 part
└─sda2        8:2    1     5G  0 part

$ sudo wipefs -a /dev/sda
/dev/sda: 8 bytes were erased at offset 0x00000200 (gpt): 45 46 49 20 50 41 52 54
/dev/sda: 8 bytes were erased at offset 0x72adffe00 (gpt): 45 46 49 20 50 41 52 54
/dev/sda: 2 bytes were erased at offset 0x000001fe (PMBR): 55 aa
/dev/sda: calling ioctl to re-read partition table: Success

$ sudo fdisk /dev/sda

Welcome to fdisk (util-linux 2.40.4).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.

Device does not contain a recognized partition table.
Created a new DOS (MBR) disklabel with disk identifier 0x6b5cc42c.

Command (m for help): g
Created a new GPT disklabel (GUID: C1AD911D-D8EA-4072-9C67-F9B165DF5216).

Command (m for help): n
Partition number (1-128, default 1): 1
First sector (2048-60125150, default 2048):
Last sector, +/-sectors or +/-size{K,M,G,T,P} (2048-60125150, default 60123135): +5G

Created a new partition 1 of type 'Linux filesystem' and of size 5 GiB.
Partition #1 contains a ext4 signature.

Do you want to remove the signature? Yes/No: Y

The signature will be removed by a write command.

Command (m for help): w
The partition table has been altered.
Calling ioctl() to re-read partition table.
Syncing disks.

$ sudo mkfs.ext4 /dev/sda1
mke2fs 1.47.2 (1-Jan-2025)
Creating filesystem with 1310720 4k blocks and 327680 inodes
Filesystem UUID: feed656d-1bd4-4d8b-8bce-dc72f8e2b90c
Superblock backups stored on blocks:
        32768, 98304, 163840, 229376, 294912, 819200, 884736

Allocating group tables: done
Writing inode tables: done
Creating journal (16384 blocks): done
Writing superblocks and filesystem accounting information: done

$ sudo blkid /dev/sda1
/dev/sda1: UUID="feed656d-1bd4-4d8b-8bce-dc72f8e2b90c" BLOCK_SIZE="4096" TYPE="ext4" PARTUUID="09bbe621-6502-422a-b891-58b389a90286"

## TODO

- Write a program that calculates the level of indirection and index
  of the inode pointer for a given offset and blocksize.

- ex-01 BUG: Infinite loop if files already exist prior to running due to limited range of
  random number generator. (Highly unlikely would require specific file name xNNNNNN to exit
  prior to execution as well as a very high file creation count (maximum 899'999))

- Finish testing filesystems for 01-ex

- do older problems that rely on testing file systems that i skipped.
