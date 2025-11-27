/* This library function is linked against libvalue.so so this x will
 * be resolved to the x that lives in it */

extern int x;

int return_symbol()
{
    return x;
}
