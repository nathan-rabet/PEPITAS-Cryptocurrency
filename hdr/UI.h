#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

int setup();

gboolean on_main_window_delete(GtkWidget *widget, __attribute__ ((unused)) gpointer data);

void on_main_window_destroy(__attribute ((unused)) GtkWidget *widget, 
                            __attribute__ ((unused)) gpointer data);

gboolean on_transaction_button_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

gboolean on_pkey_button_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

#endif
