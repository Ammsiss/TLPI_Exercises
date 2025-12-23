## Syscalls

**Setting terminal attributes**

`tcgetattr()` - Get attributes of a tty  
`tcsetattr()` - Set attributes of a tty  
`stty()`    - cli analogue of above functions

**Setting line speed**

`cfgetispeed()` - Get input line speed  
`cfgetospeed()` - Get output line speed  
`cfsetospeed()` - Set input line speed  
`cfsetispeed()` - Set output line speed

**Line-control functions**

`tcsendbreak()` - Generate break condition  
`tcdrain()`     - Block until output queue is empty  
`tcflush()`     - Discard terminal data either/both queue  
`tcflow()`      - Control the flow of data

**Terminal Identification**

`isatty()`  - Determines if a fd is a terminal  
`ttyname()` - Return name of terminal associated with a fd  

## Tables

**Figure 62-1**: Input and output queues for a terminal device

**Table 62-1**: Terminal special characters
**Table 62-2**: Terminal flags
**Table 62-3**: Differences between cooked, cbreak, and raw
terminal modes
**Table 62-4**: Values for the tcflush() queue_selector argument
**Table 62-5**: Values for the tcflush() action argument

## More Info

For terminal window size operations (and other operations) see
*ioctl_tty(2)* and search for '*window size*'

## TODO

- Write a program that detects `SIGWINCH` and outputs the new
  terminal dimensions.

- Create a program that returns information about what mode of
  the original BSD presets (cooked, raw, cbreak) that the
  terminal is currently in or if it is under a configuration
  that does not satify any of them. You can use **Table 62-3**
  for the relevant differences. Afterwards test your program by
  seeing if it correctly identifies the preset of a terminal
  that just called one of Kerrisks conversion functions.

- Make a program of the form `watchterm <command> [args ...]`
  that polls for any changes to the termios structure and if it
  detects any changes log it to a file. Then examine what
  settings various programs (vim, less...) modify and try to un-
  derstand why.

- Create a simple line editor in non-canonical mode, Bypassing
  the kernels own line editor. This program can simply end when
  the user hits `\n`.

- Make a program that implements a more precise interbyte
  timeout. Try to get it as low as 1ms. The time in milliseconds
  to wait for a key code sequence to complete. timerfd with
  `epoll()` would be a good fit.
