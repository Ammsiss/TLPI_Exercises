## Macros
## More Info

See the following man pages: man-pages(7), groff(1), man(7)
malloc
**Structural / heading macros**
    .TH — top header (title, section, date, source, manual)
    .SH — section heading
    .SS — subsection heading
    .EX/.EE — start an example section (monospace, indent, no fill)
    .TS/.TE - create a table

**Paragraph / spacing**
    .P — same as .PP
        .PP — start a new paragraph
        .LP — also the same as .PP
    .TP — tagged paragraph (used for option lists)
    .IP — indented paragraph (can take a tag or number)
    .RS — begin a relative indent (shift right)
    .RE — end that indent

**Font / emphasis**
    .B — bold
    .I — italic

**Misc formatting**
    .nf — no-fill mode (verbatim-like)
    .fi — fill mode (normal paragraph formatting)
    .nf/.fi pairs are often used around code blocks or output examples
    .RS/.RE (listed above) are also core for nested lists or examples
    .in n — Sets the indent level to *n*

**Alternating formatting**
.BI, .IB, .BR, .RB, .IR, .RI — mixed-font macros for alternating
bold/italic/roman within a single line (common for “command OPTIONS”
formatting)

## Examples

The alternating commands take white space (or quoted) separated args and
applies the specified formatting one after the other.

```nroff
.BI "void *reallocarray(void *_Nullable " ptr ", size_t " n ", size_t " size );
```
Example of a table (from malloc(1) 'attributes' section):

```nroff
.TS
allbox;
lbx lb lb
l l l.
Interface	Attribute	Value
T{
.na
.nh
.BR malloc (),
.BR free (),
.BR calloc (),
.BR realloc ()
T}	Thread safety	MT-Safe
.TE
```

## TODO

- Figure out how neovim does syntax highlighting as well as figuore
out why doing man in the terminal doesn't get it but :Man does.

- Figure out if you can use treesitter highlighting in man page code
sections.
