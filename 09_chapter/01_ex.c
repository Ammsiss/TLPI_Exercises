////////////////////////////////////////////////////////////////////////
// 9.9 - 9-1. Changes to permission ID's from various function calls.
////////////////////////////////////////////////////////////////////////

/*
* Assume in each of the following cases that the initial set of process user IDs is
* real=1000 effective=0 saved=0 file-system=0. What would be the state of the user IDs
* after the following calls?
*
*     The procces is a set-UID-root program being executed by a user with UID 1000.
*
* a) setuid(2000);
*
*     For unprivileged processes setuid() only effects the effective-UID and it can
*     only be changed to that of the processes saved-set-UID or real-UID.
*
*     For prviliged procceses setuid() changes the real, effective and saved-set UID
*     with the file-system id mirroring the effective-UID.
*
*     Therefore this call will result in the following values:
*
*     real=2000    effective=2000    saved=2000    file-system=2000.
*
* b) setreuid(–1, 2000);
*
*     For unprivileged processes setreuid() can only change the effective-UID to the
*     saved-set-UID or the real-UID.
*
*     For privilieged processes setreuid() can make arbitrary changes to both ID's.
*
*     For both types of processes the saved-set-UID will not mirror the effective-UID if
*     both ruid is specified as -1 and the effective-UID is specfified as the same value
*     as the ruid prior to the call.
*
*     Therefore this call will result in the following values:
*
*     real=1000    effective=2000    saved=2000    file-system=2000
*
* c) seteuid(2000);
*
*     For an unprivileged process setuid() and seteuid() perform identically.
*
*     For prvilieged proccesses seteuid() can make arbitrary changes to the effective-UID.
*
*     Therefore this call will result in the following values:
*
*     real=1000    effective=2000     saved=0    file-system=2000
*
* d) setfsuid(2000);
*
*     For an unprivilieged process setfsuid() can only change the file-system-UID to
*     either the effective-UID, saved-set-UID, or real-UID.
*
*     For a prviliged process setfsui() can change the file-system-UID to any value.
*
*     Therefore this call will result in the following values:
*
*     real=1000    effective=0    saved=0    file-system=2000
*
*
* e) setresuid(–1, 2000, 3000);
*
*     For an unpriviliged process setfsuid() can only change each of the values to each
*     of the values prior to the call.
*
*     For privilieged processes setfsuid() can change each of the id's to arbitrary values.
*     (file-system-UID always mirrors the effective)
*
*     Therefore this call will result in the following values:
*
*     real=1000    effective=2000    saved=3000    file-system=2000
*/
