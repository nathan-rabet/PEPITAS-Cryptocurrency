#ifndef LABEL_H
#define LABEL_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <time.h>

extern GtkLabel *balance_1;
extern GtkLabel *balance_2;
extern GtkLabel *stake_label1;
extern GtkLabel *stake_label2;
extern GtkLabel *stake_label3;
extern GtkLabel *synchro_label;
extern GtkLabel *block_amount_label;
extern GtkLabel *connections_label;
extern GtkLabel *mempool_label;

void change_label_text(GtkLabel *label, char* text);
void add_new_blockinfo(size_t height, size_t transaction);

#endif