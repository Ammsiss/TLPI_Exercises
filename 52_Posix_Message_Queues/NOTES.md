## Syscalls

mq_open(3) -> Create or open a mq (message queue)
mq_close(3) -> Create or open a mq
mq_unlink(3) -> Remove mqd and queue destruction

mq_getattr(3) -> Get attributes of a mq
mq_setattr(3) -> Set attributes of a mq

mq_send(3) -> Sends a message to a mq
mq_receive(3) -> Read a message from a mq

mq_timedsend(3) -> Timed blocking send
mq_timedreceive(3) -> Timed blocking receive

mq_notify(3) -> Set up async message handling

## Tables

**Table 52-1**: Bit values for the mq_open() oflag argument
**Figure 52-1**: Relationship between kernel data structures for POSIX
message queues

## More Info

On Linux all message queues are stored at */dev/mqueue/<your-queue>*
regardless of what they are named.

See page 1084 for command line niceties for working with message
queues on linux.

See **QSIZE** macro

See */proc/sys/fs/mqueue* for linux specific files for controlling and
viewing POSIX mq limits.

See **Stevens, 1999** for a user space implementation of POSIX message
queues using memory mapped files.

## TODO

- In *pmsg_file.h* Used a different protocol for sending the
  struct over the bytes stream as to not send the entire 4096
  bytes of data every time even if the payload only has a
  fraction of that worth of meaningful bytes. (`offsetof()` can
  help)

- In the file server exersizes remove memcpy stuff cause you can
  literally just do `(char *) &req`

- Implement the async handling of the arrival of a POSIX MQ
  message via pthreads. (The current implementaiton uses the
  signals route)
