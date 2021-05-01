#include "ui/ui.h"

static GtkWidget *window;
static GtkWidget *invest_window;
static GtkWidget *recover_window;
static GtkWidget *add_contact_window;
static GtkWidget *connection_window;
static GtkWidget *create_key_window;

static GtkButton *transa_but;
static GtkButton *pkey_but;
static GtkButton *invest_but1;
static GtkButton *invest_but2;
static GtkButton *recover_but1;
static GtkButton *recover_but2;
static GtkButton *add_contact_but1;
static GtkButton *add_contact_but2;
static GtkButton *create_key_but1;
static GtkButton *create_key_but2;
static GtkButton *connect_but;
static GtkCheckButton *password_rem_but;

GtkLabel *balance_1;
GtkLabel *balance_2;
GtkLabel *private_key_label;
GtkLabel *stake_label1;
GtkLabel *stake_label2;
GtkLabel *stake_label3;
GtkLabel *password_error_label;
GtkEntry *transa_amount;
GtkEntry *recipient_key;
GtkEntry *invest_entry;
GtkEntry *recover_entry;
GtkEntry *name_entry_con;
GtkEntry *public_key_entry_con;
GtkEntry *password_entry1;
GtkEntry *password_entry2;
GtkTreeView *tv_con;
GtkTreeStore *ts_con;
GtkTreeViewColumn *cx1_con;
GtkTreeViewColumn *cx2_con;
GtkCellRenderer *cr1_con;
GtkCellRenderer *cr2_con;
GtkTreeView *tv_th;
GtkTreeStore *ts_th;
GtkTreeViewColumn *cx1_th;
GtkTreeViewColumn *cx2_th;
GtkTreeViewColumn *cx3_th;
GtkCellRenderer *cr1_th;
GtkCellRenderer *cr2_th;
GtkCellRenderer *cr3_th;
GtkComboBox *contacts_combo;
GtkListStore *ls_combo;
GtkCellRenderer *cr1_combo;


int setup()
{
    GtkBuilder *builder;
    GError *err = NULL;

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "pepitas.glade", &err) == 0)
    {
        fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Pepitas"));
    invest_window = GTK_WIDGET(gtk_builder_get_object(builder, "invest_window"));
    recover_window = GTK_WIDGET(gtk_builder_get_object(builder, "recover_window"));
    add_contact_window = GTK_WIDGET(gtk_builder_get_object(builder, "add_contact_window"));
    connection_window = GTK_WIDGET(gtk_builder_get_object(builder, "connection_window"));
    create_key_window = GTK_WIDGET(gtk_builder_get_object(builder, "create_key_window"));

    transa_but = GTK_BUTTON(gtk_builder_get_object(builder, "transa_but"));

    pkey_but = GTK_BUTTON(gtk_builder_get_object(builder, "show_pkey_but"));

    invest_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "invest_but1"));
    invest_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "invest_but2"));

    recover_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "recover_but1"));
    recover_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "recover_but2"));

    invest_entry = GTK_ENTRY(gtk_builder_get_object(builder, "invest_entry"));
    recover_entry = GTK_ENTRY(gtk_builder_get_object(builder, "recover_entry"));

    transa_amount = GTK_ENTRY(gtk_builder_get_object(builder, "transa_amount"));
    recipient_key = GTK_ENTRY(gtk_builder_get_object(builder, "recipient_key"));
    contacts_combo = GTK_COMBO_BOX(gtk_builder_get_object(builder, "contacts_combo"));
    ls_combo = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststoreCON"));

    tv_con = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeviewCON"));
    ts_con = GTK_TREE_STORE(gtk_builder_get_object(builder, "treestoreCON"));
    cx1_con = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "Name"));
    cx2_con = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "Public key"));
    cr1_con = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cellrenderertext1"));
    cr2_con = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cellrenderertext2"));
    add_contact_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_contact_but1"));
    add_contact_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "add_contact_but2"));
    name_entry_con = GTK_ENTRY(gtk_builder_get_object(builder, "name_entry_con"));
    public_key_entry_con = GTK_ENTRY(gtk_builder_get_object(builder, "public_key_entry_con"));

    tv_th = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeviewTH"));
    ts_th = GTK_TREE_STORE(gtk_builder_get_object(builder, "treestoreTH"));
    cx1_th = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "Amount"));
    cx2_th = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "public_key_th"));
    cx3_th = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "Date"));
    cr1_th = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cellrenderertext3"));
    cr2_th = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cellrenderertext4"));
    cr3_th = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cellrenderertext5"));

    password_entry1 = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry1"));
    password_entry2 = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry2"));

    password_rem_but = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "password_rem_but"));
    create_key_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "create_key_but1"));
    create_key_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "create_key_but2"));
    connect_but = GTK_BUTTON(gtk_builder_get_object(builder, "connect_but"));

    private_key_label = GTK_LABEL(gtk_builder_get_object(builder, "private_key_label"));
    balance_1 = GTK_LABEL(gtk_builder_get_object(builder, "balance_1"));
    balance_2 = GTK_LABEL(gtk_builder_get_object(builder, "balance_2"));
    stake_label1 = GTK_LABEL(gtk_builder_get_object(builder, "stake_label1"));
    stake_label2 = GTK_LABEL(gtk_builder_get_object(builder, "stake_label2"));
    stake_label3 = GTK_LABEL(gtk_builder_get_object(builder, "stake_label3"));
    password_error_label = GTK_LABEL(gtk_builder_get_object(builder, "password_error_label"));

    gtk_widget_hide(GTK_WIDGET(private_key_label));
    gtk_widget_hide(invest_window);
    gtk_widget_hide(recover_window);
    gtk_widget_hide(add_contact_window);
    gtk_widget_hide(window);
    gtk_widget_hide(create_key_window);

    g_signal_connect(transa_but, "clicked", G_CALLBACK(on_transaction_button_press), NULL);
    g_signal_connect(pkey_but, "clicked", G_CALLBACK(on_pkey_button_press), NULL);
    g_signal_connect(invest_but1, "clicked", G_CALLBACK(on_invest_button1_press), NULL);
    g_signal_connect(invest_but2, "clicked", G_CALLBACK(on_invest_button2_press), NULL);
    g_signal_connect(recover_but1, "clicked", G_CALLBACK(on_recover_button1_press), NULL);
    g_signal_connect(recover_but2, "clicked", G_CALLBACK(on_recover_button2_press), NULL);
    g_signal_connect(add_contact_but1, "clicked", G_CALLBACK(on_add_contact_button1_press), NULL);
    g_signal_connect(add_contact_but2, "clicked", G_CALLBACK(add_contact), NULL);
    g_signal_connect(create_key_but1, "clicked", G_CALLBACK(on_create_key_but1_press), NULL);
    g_signal_connect(create_key_but2, "clicked", G_CALLBACK(on_create_key_but2_press), NULL);
    g_signal_connect(connect_but, "clicked", G_CALLBACK(on_connect_but_press), NULL);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(G_OBJECT(builder));

    gtk_widget_show(connection_window);

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
     //Call to transaction function
     if(strcmp(gtk_entry_get_text(transa_amount), "") != 0)
     {
        const time_t date = time(NULL);
        char *time_str = ctime(&date);
        double amount = strtod(gtk_entry_get_text(transa_amount), NULL);

        if(strcmp(gtk_entry_get_text(recipient_key), "") != 0)
        {
            char *public_key = (char *)gtk_entry_get_text(recipient_key);
            add_transaction_with_pkey(amount, public_key, time_str);
        }
        else if(gtk_combo_box_get_active(contacts_combo) != -1)
        {
            char *name = "";
            GtkTreeModel *p_model = gtk_combo_box_get_model(contacts_combo);
            GtkTreeIter iter;
            if(gtk_combo_box_get_active_iter(contacts_combo, &iter))
                gtk_tree_model_get(p_model, &iter, 0, &name, -1);
            char *public_key = get_public_key_from_contacts(name);
            add_transaction_with_contact(amount, public_key, time_str);
            free(public_key);
        }
     }
     gtk_entry_set_text(transa_amount, "");
     gtk_entry_set_text(recipient_key, "");

     return TRUE;
}

void add_transaction_with_pkey(double amount, char *public_key, char *date)
{
        FILE *th_f = fopen("src/ui/.transa_history", "a");
        if(th_f == NULL)
            err(-1, "Couldn't open transaction history file");

        GtkTreeIter iter;

        gtk_tree_store_append(ts_th, &iter, NULL);
        gtk_tree_store_set(ts_th, &iter, 0, amount, -1);
        gtk_tree_store_set(ts_th, &iter, 1, public_key, -1);
        gtk_tree_store_set(ts_th, &iter, 2, date, -1);
        fprintf(th_f, "%lf", amount);
        fprintf(th_f, "\n");
        fprintf(th_f, "%s", public_key);
        fprintf(th_f, "\n");
        fprintf(th_f, "%s", date);

        fclose(th_f);
}

void add_transaction_with_contact(double amount, char *public_key, char *date)
{
        FILE *th_f = fopen("src/ui/.transa_history", "a");
        if(th_f == NULL)
            err(-1, "Couldn't open transaction history file");

        GtkTreeIter iter;

        gtk_tree_store_append(ts_th, &iter, NULL);
        gtk_tree_store_set(ts_th, &iter, 0, amount, -1);
        gtk_tree_store_set(ts_th, &iter, 1, public_key, -1);
        gtk_tree_store_set(ts_th, &iter, 2, date, -1);
        fprintf(th_f, "%lf", amount);
        fprintf(th_f, "\n");
        fprintf(th_f, "%s", public_key);
        fprintf(th_f, "%s", date);

        fclose(th_f);
}

void add_transaction_from_file(double amount, char *public_key, char *date)
{
        GtkTreeIter iter;

        gtk_tree_store_append(ts_th, &iter, NULL);
        gtk_tree_store_set(ts_th, &iter, 0, amount, -1);
        gtk_tree_store_set(ts_th, &iter, 1, public_key, -1);
        gtk_tree_store_set(ts_th, &iter, 2, date, -1);
}

void load_transactions_from_file()
{
    char buff[128];
    FILE *th_f = fopen("src/ui/.transa_history", "r");
    if(th_f == NULL)
        err(-1, "Couldn't open transaction history file");

    while(1)
    {
        fgets(buff, 128, th_f);
        double amount = strtod(buff, NULL);
        fgets(buff, 128, th_f);
        char *public_key = malloc(sizeof(buff));
        strcpy(public_key, buff);
        fgets(buff, 128, th_f);
        char *date = malloc(sizeof(buff));
        strcpy(date, buff);
        if(strcmp(public_key, "") == 0 || strcmp(date, "") == 0 || amount == 0)
        {
            free(public_key);
            free(date);
            break;
        }
        add_transaction_from_file(amount, public_key, date);

        free(public_key);
        free(date);
    }
    fclose(th_f);
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

gboolean on_invest_button1_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    gtk_widget_show(invest_window);

    return TRUE;
}

gboolean on_invest_button2_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    //Call to the invest function
    gtk_widget_hide(invest_window);
    gtk_entry_set_text(invest_entry, "");

    return TRUE;
}
gboolean on_recover_button1_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    gtk_widget_show(recover_window);

    return TRUE;
}

gboolean on_recover_button2_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    //Call to the recover function
    gtk_widget_hide(recover_window);
    gtk_entry_set_text(recover_entry, "");

    return TRUE;
}

gboolean on_add_contact_button1_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    gtk_widget_show(add_contact_window);
    return TRUE;
}


gboolean add_contact(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    char *name = (char *) gtk_entry_get_text(name_entry_con);
    const char *public_key = gtk_entry_get_text(public_key_entry_con);
    if(strcmp(name, "") != 0 && strcmp(public_key ,"") != 0)
    {
        FILE *contacts_f = fopen("src/ui/.contacts", "a");
        if(contacts_f == NULL)
            err(-1, "Couldn't open contacts file");


        fprintf(contacts_f, "%s\n", name);
        fprintf(contacts_f, "%s\n", public_key);

        fclose(contacts_f);

        gtk_tree_store_clear(ts_con);
        load_contacts_from_file();
        gtk_entry_set_text(name_entry_con, "");
        gtk_entry_set_text(public_key_entry_con, "");

    }

    gtk_widget_hide(add_contact_window);
    return TRUE;
}

void add_contact_to_combobox(char *name)
{
    GtkTreeIter iter;

    gtk_list_store_append(ls_combo, &iter);
    gtk_list_store_set(ls_combo, &iter, 0, name, -1);
}

void add_contacts_from_file(char *name, char *public_key)
{
    GtkTreeIter iter;

    gtk_tree_store_append(ts_con, &iter, NULL);
    gtk_tree_store_set(ts_con, &iter, 0, name, -1);
    gtk_tree_store_set(ts_con, &iter, 1, public_key, -1);
}

void load_contacts_from_file()
{
    char buff[128];
    FILE *contacts_f = fopen("src/ui/.contacts", "r");
    if(contacts_f == NULL)
        err(-1, "Couldn't open contacts file");

    while(fgets(buff, 128, contacts_f) != NULL)
    {
        char *name = malloc(sizeof(buff));
        strcpy(name, buff);
        fgets(buff, 128, contacts_f);
        char *public_key = malloc(sizeof(buff));
        strcpy(public_key, buff);
        add_contacts_from_file(name, public_key);
        add_contact_to_combobox(name);
        free(name);
        free(public_key);
    }
    fclose(contacts_f);
}

char *get_public_key_from_contacts(const char *name)
{
    char buff[128];
    FILE *contacts_f = fopen("src/ui/.contacts", "r");
    if(contacts_f == NULL)
        err(-1, "Couldn't open contacts file");

    while(fgets(buff, 128, contacts_f) != NULL)
    {
        char *cont_name = malloc(sizeof(buff));
        strcpy(cont_name, buff);
        fgets(buff, 128, contacts_f);
        char *public_key = malloc(sizeof(buff));
        strcpy(public_key, buff);

        if(strcmp(cont_name, name) == 0)
        {
            free(cont_name);
            fclose(contacts_f);
            return public_key;
        }

        free(cont_name);
        free(public_key);
    }
    fclose(contacts_f);
    return NULL;
}



gboolean on_create_key_but1_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    gtk_widget_show(create_key_window);
    return TRUE;

}

gboolean on_create_key_but2_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    /*
    char *buff = malloc(sizeof(char) * strlen(gtk_entry_get_text(password_entry2)));
    sprintf(buff, gtk_entry_get_text(password_entry2));
    */
    get_keys();
    //char *hashed = sha384_data(buff, strlen(buff));
    //FILE *password_f = fopen("../../.password", "wb");
    //if(password_f == NULL)
    //    err(-1, "Couldn't open password file");
    //fwrite(hashed, 1, sizeof(hashed), password_f);
    //free(buff);
    //free(hashed);
    //fclose(password_f);
    gtk_widget_hide(create_key_window);
    return TRUE;
}

gboolean on_connect_but_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{/*
    FILE *password_f = fopen("../../.password", "r");
    if(password_f == NULL)
        err(-1, "Couldn't open password file");
    char *buff = malloc(sizeof(char) * strlen(gtk_entry_get_text(password_entry1)));
    sprintf(buff, gtk_entry_get_text(password_entry1));
    char *hashed = sha384_data(buff, strlen(buff));
    char *buff_hashed = malloc(strlen(hashed) * sizeof(char));
    fread(buff_hashed, 1, strlen(hashed), password_f);
    if(strcmp(buff_hashed, hashed) != 0)
    {
        free(buff_hashed);
        free(hashed);
        free(buff);
        fclose(password_f);
        gtk_label_set_text(password_error_label, "Wrong password");
    }
    else
    {
        free(buff_hashed);
        free(hashed);
        free(buff);
        fclose(password_f);*/
        update_labels();
        load_contacts_from_file();
        load_transactions_from_file();
        gtk_widget_hide(connection_window);
        gtk_widget_show(window);
    //}
    return TRUE;
}

void update_labels()
{

    Wallet *wallet = get_my_wallet();
    double money = wallet->amount / 100000000.0f;
    if(strtod(gtk_label_get_text(balance_1), NULL) != money)
    {
        char buff1[30];
        sprintf(buff1, "%lf", money);
        gtk_label_set_text(balance_1, buff1);
        gtk_label_set_text(balance_2, buff1);
    }
    /*if(atol(gtk_label_get_text(stake_label1)) != Get_Stake)
    {
        char buff2[30];
        sprintf(buff, "%lu",Get_Stake);
        gtk_label_set_text(stake_label1, buff2);
        gtk_label_set_text(stake_label2, buff2);
        gtk_label_set_text(stake_label3, buff2);
    }
    */
}
