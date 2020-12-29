OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
    . = 0x200000;
    .text : 
    {
        *(.text)
    }

    .data : 
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }
}