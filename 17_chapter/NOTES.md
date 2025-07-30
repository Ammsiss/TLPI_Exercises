## Syscalls

**acl_get_file(3)** -> Loads a files ACL into memory
    Related: acl_set_file(3)

**acl_get_entry(3)** -> Retrieves an ACL entry from an in-memory ACL

**acl_get_tag_type(3)** -> Retrieves a tag type from an ACL entry

**acl_get_tag_type(3)** -> Sets a tag type from an ACL entry
    Related: acl_set_tag_type(3)

**acl_get_qualifier(3)** -> Retrieves a tag qualifier from an ACL entry
    Related: acl_set_qualifier(3)

**acl_get_permset(3)** -> Retrieves the perm set from an ACL entry
    Related: acl_set_permset(3), acl_add_perm(3), acl_delete_perm(3),
    acl_clear_perms(3)

**acl_create_entry(3)** -> Creates a new entry in an existing ACL
    Related: acl_delete_entry(3)

**acl_from_text(3)** -> Creates an in memory ACL from a textual one
    Related: acl_to_text(3)

## Tables

- See **Table 17-2** for a visual representation of some of the functions
  and data structures in the ACL API.

## More Info

- The funcitons that constitute the ACL API are listed in the **acl(5)** man page.

- See Figure 17-2 for a visual representation of teh ACL API.

- The final versions (Draft 17) of the draft POSIX.1e and POSIX.2c standards are
  available online at http://wt.tuxomania.net/publications/posix.1e/.

- The acl(5) manual page gives an overview of ACLs and some guidance on the
  portability of the various ACL library functions implemented on Linux.

- Details of the Linux implementation of ACLs and extended attributes can be
  found in [Grünbacher, 2003]. Andreas Grünbacher maintains a web site contain-
  ing information about ACLs at http://acl.bestbits.at/.

## TODO
