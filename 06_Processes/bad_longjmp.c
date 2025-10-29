#include <setjmp.h>
#include <stdio.h>

jmp_buf env;

void f1()
{
    // Sets jump location to f1
    switch (setjmp(env)) {
    case 0:
        puts("Initial call to setjmp!");
    case 1:
        puts("???");
    }
}

int main(void)
{
    f1();
    longjmp(env, 2); // Jumps to f1 which is not on the stack!
}
