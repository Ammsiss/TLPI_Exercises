////////////////////////////////////////////////////////////////////////
// 9.9 - 9-2. Is the process privileged?
////////////////////////////////////////////////////////////////////////

/*
* Is a process with the following user IDs privileged? Explain your answer.
* real=0 effective=1000 saved=1000 file-system=1000
*
*     A processes privilege is determined solely by the file-system-UID (and
*     in practice the effective-UID) therefore this process would not be privileged.
*
*     This is likely an example of a program that is owned by a non root user
*     with the set-UID bit enabled, being run by a root user.
*
*     Though this program could regain priviliges through a call to seteuid()
*/
