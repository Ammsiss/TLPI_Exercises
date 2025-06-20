////////////////////////////////////////////////////////////////////////
// 9.9 - 9-4. Dropping and regaining privileges.
////////////////////////////////////////////////////////////////////////

/*
    If a process whose user IDs all have the value X executes a set-user-ID program
    whose user ID, Y, is nonzero, then the process credentials are set as follows:

        real=X effective=Y saved=Y

    (We ignore the file-system user ID, since it tracks the effective user ID.) Show the
    setuid(), seteuid(), setreuid(), and setresuid() calls, respectively, that would be used to
    perform the following operations:

        a) Suspend and resume the set-user-ID identity (i.e., switch the effective user
        ID to the value of the real user ID and then back to the saved set-user-ID).

        b) Permanently drop the set-user-ID identity (i.e., ensure that the effective
        user ID and the saved set-user-ID are set to the value of the real user ID).

    (This exercise also requires the use of getuid() and geteuid() to retrieve the process’s
    real and effective user IDs.) Note that for certain of the system calls listed above,
    some of these operations can’t be performed.

    a)

    setuid() & seteuid()

        1: uid_t euid = getuid(); <- save effective-UID
        2: setuid(getuid());      <- set effective-UID to real-UID
        3: setuid(euid);          <- set effective-UID to saved-UID

    setreuid()

        1: uid_t euid = getuid();  <- save effective-UID
        2: setreuid(-1, getuid()); <- set effective-UID to real-UID
        3: setreuid(-1, euid);     <- set effective-UID to saved-UID

    setresuid()

        1: uid_t euid = getuid();       <- save effective-UID
        2: setresuid(-1, getuid(), -1); <- set effective-UID to real-UID
        3: setresuid(-1, euid, -1);     <- set effective-UID to saved-UID

    b)

    setreuid()

        1: setreuid(getuid(), getuid()); <- set effective-UID & saved-UID to real-UID

        This works becuase of the following rule:

            For both privileged and unprivileged processes, the saved set-user-ID is also set
            to the same value as the (new) effective user ID if either of the following is true:

                a) ruid is not –1 (i.e., the real user ID is being set, even to the same value it
                    already had), or

                b) the effective user ID is being set to a value other than the value of the real
                    user ID prior to the call.

            Put conversely, if a process uses setreuid() only to change the effective user ID
            to the same value as the current real user ID, then the saved set-user-ID is left
            unchanged, and a later call to setreuid() (or seteuid()) can restore the effective
            user ID to the saved set-user-ID value. (SUSv3 doesn’t specify the effect of
            setreuid() and setregid() on the saved set IDs, but SUSv4 specifies the behavior
            described here.)

    setresuid()

        1: setresuid(getuid(), getuid(), getuid()); <- set all id's to real-UID

    setresuid()

    1: 

 */
