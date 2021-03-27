#include "../hdr/UI.h"

static GtkWidget *window;
static GtkListStore *list_store_th;
static GtkListStore *list_store_con;
static GtkLabel *private_key_label;

GtkButton *transa_but;
GtkButton *pkey_but;
GtkEntry *transa_amount;
GtkEntry *recipient_key;

int setup()
{
    GtkBuilder *builder;
    GError *err = NULL;

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "../glade/pepitas.glade", &err) == 0)
    {
        fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Pepitas"));

    list_store_th = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststoreTH"));
    list_store_con = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststoreCON"));

    transa_but = GTK_BUTTON(gtk_builder_get_object(builder, "transa_but"));

    pkey_but = GTK_BUTTON(gtk_builder_get_object(builder, "show_pkey_but"));

    transa_amount = GTK_ENTRY(gtk_builder_get_object(builder, "transa_amount"));

    recipient_key = GTK_ENTRY(gtk_builder_get_object(builder, "recipient_key"));

    private_key_label = GTK_LABEL(gtk_builder_get_object(builder, "private_key_label"));
    gtk_widget_hide(GTK_WIDGET(private_key_label));

    g_signal_connect(transa_but, "clicked", G_CALLBACK(on_transaction_button_press), NULL);
    g_signal_connect(pkey_but, "clicked", G_CALLBACK(on_pkey_button_press), NULL);

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(G_OBJECT(builder));

    gtk_widget_show(window);

    return 0;
}

gboolean on_main_window_delete(
            GtkWidget *widget, __attribute__((unused)) gpointer data)
{
    g_print("Quitting app...\n");
    gtk_widget_destroy(widget);
    return TRUE;

}

void on_main_window_destroy(__attribute__ ((unused)) GtkWidget *widget,
                            __attribute__ ((unused)) gpointer data)
{
    gtk_main_quit();
    g_print("App closed\n");
}

gboolean on_transaction_button_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused))gpointer user_data)
{
     return TRUE;
}

gboolean on_pkey_button_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    if(strcmp(gtk_button_get_label(pkey_but), "Show key") == 0)
    {
        gtk_button_set_label(pkey_but, "Hide key");
        gtk_widget_show(GTK_WIDGET(private_key_label));
    }
    else
    {
        gtk_button_set_label(pkey_but, "Show key");
        gtk_widget_hide(GTK_WIDGET(private_key_label));
    }

    return TRUE;
}
