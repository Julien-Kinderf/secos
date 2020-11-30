/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>

void print_gdt(gdt_reg_t gdtr)
{
    /*
    gdt_reg_t default_gdtr;
    get_gdtr(default_gdtr);
    printf("Default gdt is located at %p and its limit is %d\n", default_gdtr.addr, default_gdtr.limit);
    printf("Displaying the default gdt ...\n");
    */
    seg_desc_t *current_seg_desc;
    current_seg_desc = gdtr.desc;
    for (unsigned int entry = 0; entry < (gdtr.limit + 1) / sizeof(seg_desc_t); entry++)
    {
        int base = (current_seg_desc->base_3 << 24) | (current_seg_desc->base_2 << 16) | current_seg_desc->base_2;
        int limit = (current_seg_desc->limit_2 << 16) | current_seg_desc->limit_1;
        printf("Segment %d : base = %p, limit = %d, type=%d\n", entry, base, limit, current_seg_desc->type);
        current_seg_desc++;
    }
    printf("\n");
}

void load_gdt(seg_desc_t gdt[], size_t size)
{
    // the null segment
    memset(&gdt[0], 0, sizeof(seg_desc_t));

    // the first (code) segment
    memcpy(&gdt[1], &gdt[0], sizeof(seg_desc_t));
    gdt[1].limit_1 = 0xffff;
    gdt[1].limit_2 = 0xf;
    gdt[1].d = 1;
    gdt[1].g = 1;
    gdt[1].s = 1;
    gdt[1].p = 1;
    gdt[1].type = SEG_DESC_CODE_XR;

    // the second (data) segment
    gdt[2] = gdt[1];
    gdt[2].type = SEG_DESC_DATA_RW;

    // load the new gdt
    gdt_reg_t new_gdtr;
    new_gdtr.desc = gdt;
    new_gdtr.limit = sizeof(*gdt) * size - 1;
    set_gdtr(new_gdtr);

    // reload the segment selectors
    seg_sel_t code_segment_selector;
    seg_sel_t data_segment_selector;
    code_segment_selector.index = 1;
    code_segment_selector.ti = 0;
    code_segment_selector.rpl = 0;

    data_segment_selector = code_segment_selector;
    data_segment_selector.index = 2;
    data_segment_selector.rpl = 0;

    set_ss(data_segment_selector);
    set_ds(data_segment_selector);
    set_es(data_segment_selector);
    set_fs(data_segment_selector);
    set_gs(data_segment_selector);
    printf("passed the data segments \n");

    // on passe la valeur directement, je n'ai toujours pas compris pourquoi
    set_cs(8);
    printf("passed the code segments\n");
}
void tp()
{
    // Question 1 : afficher la gdt :

    gdt_reg_t default_gdtr;
    get_gdtr(default_gdtr);
    printf("Default gdt is located at %p and its limit is %d\n", default_gdtr.addr, default_gdtr.limit);
    printf("Displaying the default gdt ...\n");
    print_gdt(default_gdtr);

    // Question 2 : charger ma propre gdt :
    seg_desc_t *gdt = (seg_desc_t *)0x10000;
    load_gdt(gdt, 3);
    gdt_reg_t new_gdtr;
    get_gdtr(new_gdtr);
    printf("New gdt is located at %p and its limit is %d\n", new_gdtr.addr, new_gdtr.limit);
    printf("Displaying the new gdt ...\n");
    print_gdt(new_gdtr);
}
