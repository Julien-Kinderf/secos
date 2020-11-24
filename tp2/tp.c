/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <intr.h>
extern info_t *info;
extern int_desc_t IDT[IDT_NR_DESC];

bp_handler()
{
    debug("Je suis un message de debug de la fonction bp_handler ! On duty !\n");
}

void tp()
{
    intr_init();

    // 3.1 : localisation de l'IDT
    idt_reg_t idtr;
    get_idtr(idtr);
    printf("IDT : @base = %p, limite = %d\n", idtr.addr, idtr.limit);
}
