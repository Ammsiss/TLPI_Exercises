////////////////////////////////////////////////////////////////////////
// 9.9 - 9-5. Dropping and regaining privileges, Part 2.
////////////////////////////////////////////////////////////////////////

/*
    Repeat the previous exercise (04_ex.c) for a process executing a
    set-user-ID-root program, which has the following initial set of
    process credentials:

        real=X effective=0 saved=0

    a)

    setuid()

        cannot be used for this purpose.

    seteuid()

        1: seteuid(getuid()); <- set effective-UID to real-UID
        1: seteuid(0);        <- set effective-UID to saved-UID

    setreuid()

        1: setreuid(-1, getuid()); <- set effective-UID to real-UID
        2: setreuid(-1, 0);        <- set effective-UID to saved-UID

    setresuid()

        1: setresuid(-1, getuid(), -1); <- set effective-UID to real-UID
        2: setresuid(-1, getuid(), -1); <- set effective-UID to saved-UID

    b)

    setuid()

        1: setuid(getuid()) <- change all id's to real-UID

    seteuid()

        cannot be used for this purpose

    setreuid()

        1: setreuid(getuid(), getuid())

    setresuid()

        1: setresuid(getuid(), getuid(), getuid())

*/
