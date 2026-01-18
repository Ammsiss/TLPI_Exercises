## Listings

**Listing 60-1** (*id_echo.h*): Header file for *id_echo_sv.c*
and *id_echo_cl.c*.

**Listing 60-2** (*id_echo_sv.c*): An iterative server that
implements the UDP echo service.

**Listing 60-3** (*id_echo_cl.c*): A client for the UDP echo
service.

**Listing 60-4** (*is_echo_sv.c*): A concurrent server that
implements the TCP echo service (The client code is in ch61)

**Listing 60-5**: Example lines from */etc/inetd.conf*

**Listing 60-6**: TCP echo server designed to be invoked via
inetd

## More Info

Preforked and prethreaded servers are described in some detail
in Chapter 30 of [Stevens et al., 2004].

Further information about DNS round-robin load sharing can be
found in [Albitz & Liu, 2006].

Further information about server load balancing can be found in
[Kopparapu, 2002].

## TODO

- [ ] Recreate a simple version of inetd() and create a server
  program that it works with. **MVP:** Have a config file that
  maps a name/port/protocol to a program and argv. Support TCP
  only at first. For each entry, create a listening socket, put
  it in nonblocking mode, then run a poll() loop over all
  listeners. When a listener is readable, accept(), then fork().
  In the child: dup2(connfd, 0), dup2(connfd, 1), maybe
  dup2(connfd, 2), close everything else, then execve() the
  target program. Parent closes connfd and keeps polling. This
  will need logging, maybe syslog if you don't use systemd to
  launch your inetd.

- [ ] After you create the `inetd()` imp, test *inetd_echo_sv.c*
  with the `-i` option with it.
