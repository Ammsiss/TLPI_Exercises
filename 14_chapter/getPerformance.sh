#!/bin/zsh

fs_dir_list=("ext2_fs" "ext3_fs" "ext4_fs" "xfs_fs")
NF_values=(1000 5000 10000 20000)

for fs_dir in "${fs_dir_list[@]}"; do

    echo "Testing $fs_dir..."

    for NF in "${NF_values[@]}"; do
        sudo /usr/bin/time --format="Elapsed:%e System:%S User:%U CPU:%P (%I %O)" \
            ./01-ex.bin "$NF" "$fs_dir"
    done 2>> fs-metrics.txt

    tail fs-metrics.txt -n 4
done
