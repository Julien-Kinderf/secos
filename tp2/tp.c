/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <intr.h>
extern info_t *info;
extern int_desc_t IDT[IDT_NR_DESC];

void bp_handler()
{
    // Ecrit un message de débug
    debug("Je suis un message de debug de la fonction bp_handler ! On duty !\n");
}

void bp_trigger()
{
    // déclenche un breakpoint grâce à l'instruction int3
    asm("int3");
}

void tp()
{
    intr_init();

    // 3.1 : localisation de l'IDT
    idt_reg_t idtr;
    get_idtr(idtr);
    printf("IDT : @base = %p, limite = %d bytes\n", idtr.addr, idtr.limit);

    // 3.4 : on remplace le descripteur d'interruption de
    // breakpoint par notre breakpoint à nous
    int_desc_t *my_desc = (int_desc_t *)idtr.addr + 3;
    my_desc->offset_1 = (offset_t)bp_handler;
    my_desc->offset_2 = (offset_t)bp_handler >> 16;

    printf("Everything is ready\n");
    bp_trigger();
}
