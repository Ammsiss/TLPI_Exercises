BEGIN {
    parentFirst = 0
    childFirst = 0
}

NR % 2 != 0 {
    if ($2 ~ /parent/)
        ++parentFirst
    else
        ++childFirst
}

END {
    printf "Parent first: %d\n", parentFirst
    printf "Child first: %d\n", childFirst
}
