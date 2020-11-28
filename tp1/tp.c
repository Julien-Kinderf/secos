/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>

extern info_t *info;

void print_gdt()
{
    gdt_reg_t default_gdtr;
    get_gdtr(default_gdtr);
    printf("Default gdt is located at %p and its limit is %d\n", default_gdtr.addr, default_gdtr.limit);
    printf("Displaying the default gdt ...\n");

    seg_desc_t *current_seg_desc;
    for (unsigned int entry = 0; entry < (default_gdtr.limit + 1) / sizeof(seg_desc_t); entry++)
    {
        current_seg_desc = default_gdtr.desc;
        int base = (current_seg_desc->base_3 << 24) | (current_seg_desc->base_2 << 16) | current_seg_desc->base_2;
        int limit = (current_seg_desc->limit_2 << 16) | current_seg_desc->limit_1;
        printf("Segment %d : base = %p, limit = %d, type=%d\n", entry, base, limit, current_seg_desc->type);
        current_seg_desc++;
    }
}

void tp()
{
    // Question 1 : afficher la gdt :
    print_gdt();
}
