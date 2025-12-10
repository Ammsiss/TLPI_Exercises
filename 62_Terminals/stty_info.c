#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "../lib/tlpi_hdr.h" // IWYU pragma: export

#define CHECK_FLAG(flag_type, flag) if (flag_type & flag) { \
    printf(#flag " "); \
} \

int main(void)
{
    struct termios tb;
    if (tcgetattr(STDIN_FILENO, &tb) == -1)
        errExit("tcgetsattr");

    printf("c_iflag Flags:\n    ");
    CHECK_FLAG(tb.c_cflag, IGNBRK);
    CHECK_FLAG(tb.c_cflag, BRKINT);
    CHECK_FLAG(tb.c_cflag, IGNPAR);
    CHECK_FLAG(tb.c_cflag, PARMRK);
    CHECK_FLAG(tb.c_cflag, INPCK);
    CHECK_FLAG(tb.c_cflag, ISTRIP);
    CHECK_FLAG(tb.c_cflag, INLCR);
    CHECK_FLAG(tb.c_cflag, IGNCR);
    CHECK_FLAG(tb.c_cflag, ICRNL);
    CHECK_FLAG(tb.c_cflag, IXON);
    CHECK_FLAG(tb.c_cflag, IXOFF);
    CHECK_FLAG(tb.c_cflag, IUCLC);
    CHECK_FLAG(tb.c_cflag, IXANY);
    CHECK_FLAG(tb.c_cflag, IMAXBEL);   /* Not on linux; always defined */
    CHECK_FLAG(tb.c_cflag, IUTF8);
 // nl0 cr0 tab0 bs0 vt0 ff0
    printf("\nc_oflag Flags:\n    ");
    CHECK_FLAG(tb.c_cflag, OPOST);
    CHECK_FLAG(tb.c_cflag, OLCUC);
    CHECK_FLAG(tb.c_cflag, OCRNL);
    CHECK_FLAG(tb.c_cflag, ONLCR);
    CHECK_FLAG(tb.c_cflag, ONOCR);
    CHECK_FLAG(tb.c_cflag, ONLRET);
    CHECK_FLAG(tb.c_cflag, OFILL);
    CHECK_FLAG(tb.c_cflag, OFDEL);

    switch (tb.c_cflag & NLDLY) {
    case NL0: printf("NLO "); break;
    case NL1: printf("NL1 "); break; }

    switch (tb.c_cflag & CRDLY) {
    case CR0: printf("CR0 "); break;
    case CR1: printf("CR1 "); break;
    case CR2: printf("CR2 "); break;
    case CR3: printf("CR3 "); break; }

    switch (tb.c_cflag & TABDLY) {
    case TAB0: printf("TAB0 "); break;
    case TAB1: printf("TAB1 "); break;
    case TAB2: printf("TAB2 "); break;
    case TAB3: printf("TAB3 "); break; }

    CHECK_FLAG(tb.c_cflag, BSDLY);
    CHECK_FLAG(tb.c_cflag, VTDLY);
    CHECK_FLAG(tb.c_cflag, FFDLY);

    printf("\nc_cflag Flags:\n    ");
    CHECK_FLAG(tb.c_cflag, PARENB);
    CHECK_FLAG(tb.c_cflag, PARODD);
    CHECK_FLAG(tb.c_cflag, CMSPAR);
    CHECK_FLAG(tb.c_cflag, CSIZE);
    CHECK_FLAG(tb.c_cflag, HUPCL);
    CHECK_FLAG(tb.c_cflag, CSTOPB);
    CHECK_FLAG(tb.c_cflag, CREAD);
    CHECK_FLAG(tb.c_cflag, CLOCAL);
    CHECK_FLAG(tb.c_cflag, CRTSCTS);

    printf("\nc_lflag Flags:\n    ");
    CHECK_FLAG(tb.c_cflag, ISIG);
    CHECK_FLAG(tb.c_cflag, ICANON);
    CHECK_FLAG(tb.c_cflag, XCASE);
    CHECK_FLAG(tb.c_cflag, ECHO);
    CHECK_FLAG(tb.c_cflag, ECHOE);
    CHECK_FLAG(tb.c_cflag, ECHOK);
    CHECK_FLAG(tb.c_cflag, ECHONL);
    CHECK_FLAG(tb.c_cflag, ECHOCTL);
    CHECK_FLAG(tb.c_cflag, ECHOPRT);
    CHECK_FLAG(tb.c_cflag, ECHOKE);
    CHECK_FLAG(tb.c_cflag, FLUSHO);
    CHECK_FLAG(tb.c_cflag, NOFLSH);
    CHECK_FLAG(tb.c_cflag, TOSTOP);
    CHECK_FLAG(tb.c_cflag, PENDIN);
    CHECK_FLAG(tb.c_cflag, IEXTEN);

    printf("\n");
}
