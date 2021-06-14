#include "ui/ui.h"

static GtkWidget *window;
static GtkWidget *invest_window;
static GtkWidget *recover_window;
static GtkWidget *add_contact_window;
static GtkWidget *connection_window;
static GtkWidget *create_key_window;

static GtkButton *transa_but;
static GtkButton *invest_but1;
static GtkButton *invest_but2;
static GtkButton *recover_but1;
static GtkButton *recover_but2;
static GtkButton *add_contact_but1;
static GtkButton *add_contact_but2;
static GtkButton *create_key_but1;
static GtkButton *create_key_but2;
static GtkButton *connect_but;
static GtkButton *prev_block_but;
static GtkButton *next_block_but;


GtkLabel *balance_1;
GtkLabel *balance_2;
GtkLabel *stake_label1;
GtkLabel *stake_label2;
GtkLabel *stake_label3;
GtkLabel *synchro_label;
GtkLabel *block_amount_label;
GtkLabel *connections_label;
GtkLabel *mempool_label;
GtkLabel *public_key_label;
GtkLabel *password_error_label;
GtkLabel *latest_block_name1;
GtkLabel *latest_block_name2;
GtkLabel *latest_block_name3;
GtkLabel *error_label;
GtkLabel *block_height_label;
GtkLabel *transa_number_label;
GtkLabel *total_transa_label;
GtkLabel *magic_label;
GtkLabel *prev_block_valid_label;
GtkLabel *nb_validators_label;
GtkLabel *block_error_label;
GtkLabel *block_time_label;
GtkLabel *validators_votes_label;
GtkEntry *transa_amount;
GtkEntry *recipient_key;
GtkEntry *asset_entry;
GtkEntry *cause_entry;
GtkEntry *invest_entry;
GtkEntry *recover_entry;
GtkEntry *name_entry_con;
GtkEntry *public_key_entry_con;
GtkEntry *password_entry1;
GtkEntry *password_entry2;
GtkEntry *key_entry;
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
GtkProgressBar *progress_bar_blockchain;

size_t block_height = 0;

void *setup(void *args)
{
    infos_st *infos = args;
    struct stat st = {0};

    if (stat("data/ui", &st) == -1)
    {
        mkdir("data/ui", 0700);
    }
    if (stat("data/contact", &st) == -1)
    {
        mkdir("data/contact", 0700);
    }

    GtkBuilder *builder;
    GError *err = NULL;

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "data/pepitas.glade", &err) == 0)
    {
        fprintf(stderr, "Error adding build from file. Error: %s\n", err->message);
        pthread_exit(NULL);
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Pepitas"));
    invest_window = GTK_WIDGET(gtk_builder_get_object(builder, "invest_window"));
    recover_window = GTK_WIDGET(gtk_builder_get_object(builder, "recover_window"));
    add_contact_window = GTK_WIDGET(gtk_builder_get_object(builder, "add_contact_window"));
    connection_window = GTK_WIDGET(gtk_builder_get_object(builder, "connection_window"));
    create_key_window = GTK_WIDGET(gtk_builder_get_object(builder, "create_key_window"));


    transa_but = GTK_BUTTON(gtk_builder_get_object(builder, "transa_but"));

    invest_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "invest_but1"));
    invest_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "invest_but2"));

    recover_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "recover_but1"));
    recover_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "recover_but2"));

    prev_block_but = GTK_BUTTON(gtk_builder_get_object(builder, "prev_block_but"));
    next_block_but = GTK_BUTTON(gtk_builder_get_object(builder, "next_block_but"));

    invest_entry = GTK_ENTRY(gtk_builder_get_object(builder, "invest_entry"));
    recover_entry = GTK_ENTRY(gtk_builder_get_object(builder, "recover_entry"));

    key_entry = GTK_ENTRY(gtk_builder_get_object(builder, "key_entry"));

    transa_amount = GTK_ENTRY(gtk_builder_get_object(builder, "transa_amount"));
    recipient_key = GTK_ENTRY(gtk_builder_get_object(builder, "recipient_key"));
    asset_entry = GTK_ENTRY(gtk_builder_get_object(builder, "asset_entry"));
    cause_entry = GTK_ENTRY(gtk_builder_get_object(builder, "cause_entry"));
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

    create_key_but1 = GTK_BUTTON(gtk_builder_get_object(builder, "create_key_but1"));
    create_key_but2 = GTK_BUTTON(gtk_builder_get_object(builder, "create_key_but2"));
    connect_but = GTK_BUTTON(gtk_builder_get_object(builder, "connect_but"));

    public_key_label = GTK_LABEL(gtk_builder_get_object(builder, "public_key_label"));
    balance_1 = GTK_LABEL(gtk_builder_get_object(builder, "balance_1"));
    balance_2 = GTK_LABEL(gtk_builder_get_object(builder, "balance_2"));
    stake_label1 = GTK_LABEL(gtk_builder_get_object(builder, "stake_label1"));
    stake_label2 = GTK_LABEL(gtk_builder_get_object(builder, "stake_label2"));
    stake_label3 = GTK_LABEL(gtk_builder_get_object(builder, "stake_label3"));
    password_error_label = GTK_LABEL(gtk_builder_get_object(builder, "password_error_label"));
    synchro_label = GTK_LABEL(gtk_builder_get_object(builder, "synchro_label"));
    block_amount_label= GTK_LABEL(gtk_builder_get_object(builder, "block_amount_label"));
    latest_block_name1 = GTK_LABEL(gtk_builder_get_object(builder, "latest_block_name1"));
    latest_block_name2 = GTK_LABEL(gtk_builder_get_object(builder, "latest_block_name2"));
    latest_block_name3 = GTK_LABEL(gtk_builder_get_object(builder, "latest_block_name3"));
    connections_label = GTK_LABEL(gtk_builder_get_object(builder, "connections_label"));
    mempool_label = GTK_LABEL(gtk_builder_get_object(builder, "mempool_label"));
    error_label = GTK_LABEL(gtk_builder_get_object(builder, "error_label"));

    block_height_label = GTK_LABEL(gtk_builder_get_object(builder, "block_height_label"));
    transa_number_label = GTK_LABEL(gtk_builder_get_object(builder, "transa_number_label"));
    total_transa_label = GTK_LABEL(gtk_builder_get_object(builder, "total_transa_label"));
    magic_label = GTK_LABEL(gtk_builder_get_object(builder, "magic_label"));
    prev_block_valid_label = GTK_LABEL(gtk_builder_get_object(builder, "prev_block_valid_label"));
    nb_validators_label = GTK_LABEL(gtk_builder_get_object(builder, "nb_validators_label"));
    block_error_label = GTK_LABEL(gtk_builder_get_object(builder, "block_error_label"));
    block_time_label = GTK_LABEL(gtk_builder_get_object(builder, "block_time_label"));
    validators_votes_label = GTK_LABEL(gtk_builder_get_object(builder, "validators_votes_label"));

    progress_bar_blockchain = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar_blockchain"));

    gtk_widget_hide(invest_window);
    gtk_widget_hide(recover_window);
    gtk_widget_hide(add_contact_window);
    gtk_widget_hide(window);
    gtk_widget_hide(create_key_window);

    g_signal_connect(transa_but, "clicked", G_CALLBACK(on_transaction_button_press), NULL);
    g_signal_connect(invest_but1, "clicked", G_CALLBACK(on_invest_button1_press), NULL);
    g_signal_connect(invest_but2, "clicked", G_CALLBACK(on_invest_button2_press), NULL);
    g_signal_connect(recover_but1, "clicked", G_CALLBACK(on_recover_button1_press), NULL);
    g_signal_connect(recover_but2, "clicked", G_CALLBACK(on_recover_button2_press), NULL);
    g_signal_connect(add_contact_but1, "clicked", G_CALLBACK(on_add_contact_button1_press), NULL);
    g_signal_connect(add_contact_but2, "clicked", G_CALLBACK(add_contact), NULL);
    g_signal_connect(create_key_but1, "clicked", G_CALLBACK(on_create_key_but1_press), NULL);
    g_signal_connect(create_key_but2, "clicked", G_CALLBACK(on_create_key_but2_press), NULL);
    g_signal_connect(connect_but, "clicked", G_CALLBACK(on_connect_but_press), NULL);
    g_signal_connect(password_entry1, "activate", G_CALLBACK(on_connect_but_press), NULL);
    g_signal_connect(next_block_but, "clicked", G_CALLBACK(set_block_viewer_plus), NULL);
    g_signal_connect(prev_block_but, "clicked", G_CALLBACK(set_block_viewer_minus), NULL);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    g_signal_connect(window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(G_OBJECT(builder));

    // SETUP
    gtk_label_set_text(latest_block_name1, "");
    gtk_label_set_text(latest_block_name2, "");
    gtk_label_set_text(latest_block_name3, "");

    set_block_viewer(0);

    gtk_widget_show(connection_window);

    infos->as_epoch = 0;

    gtk_main();

    pthread_exit(NULL);
}

void change_label_text(GtkLabel *label, char* text)
{
    gtk_label_set_text(label, text);
}

gboolean set_block_viewer_plus(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event, 
                    __attribute__ ((unused)) gpointer user_data)
{
    block_height += 1;
    if(get_block(block_height) == NULL)
    { 
        block_height -= 1;
        return TRUE;
    }
    set_block_viewer(block_height);
    return TRUE;

}

gboolean set_block_viewer_minus(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event, 
                    __attribute__ ((unused)) gpointer user_data)
{
    if (block_height > 0)
    {
        block_height -= 1;
        if(get_block(block_height) == NULL)
        { 
            block_height += 1;
            return TRUE;
        } 
        set_block_viewer(block_height);
    }
    return TRUE;
}

void set_block_viewer(int height)
{
    Block *block = get_block(height);
    if (block == NULL)
        return;
    BlockData block_data = block->block_data;

    size_t amount = 0;
    for(int i = 0; i < block_data.nb_transactions; i++)
        amount += block_data.transactions[i]->transaction_data.amount;

    char block_height_str[128];
    sprintf(block_height_str, "Block n°%lu", block_data.height);
    char transa_number[128];
    sprintf(transa_number, "%u", block_data.nb_transactions);
    char total_transa[128];
    sprintf(total_transa, "%lu Pepitas", amount);
    char nb_validators[128];
    sprintf(nb_validators, "%d", block_data.nb_validators);
    char magic[128];
    sprintf(magic, "%d", block_data.magic);
    char prev_block_is_valid[128];
    sprintf(prev_block_is_valid, "%d", block_data.is_prev_block_valid);
    char block_time[30];
    const time_t date = block_data.block_timestamp;
    sprintf(block_time, "%s", ctime(&date));
    char validators_votes[513];
    int index = 0;
    for (int i = 0; i <= block_data.nb_validators/8; i++)
    {
        for (char j = 0; j < (i == block_data.nb_validators/8? block_data.nb_validators%8 : 8); j++)
        {
            sprintf(validators_votes + index, "%d", (block->validators_votes[i] & (1 << j)) >> j);
            index++;
        }
    }
    validators_votes[index] = 0;
    


    gtk_label_set_text(block_height_label, block_height_str);
    gtk_label_set_text(transa_number_label, transa_number);
    gtk_label_set_text(total_transa_label, total_transa);
    gtk_label_set_text(magic_label, magic);
    gtk_label_set_text(prev_block_valid_label, prev_block_is_valid);
    gtk_label_set_text(nb_validators_label, nb_validators);
    gtk_label_set_text(block_time_label, block_time);
    gtk_label_set_text(validators_votes_label, validators_votes);
    gtk_label_set_text(block_error_label, "");

}

void add_new_blockinfo(size_t height, size_t transaction)
{
    if (blocksinfo[0].height < height) {
        blocksinfo[2] = blocksinfo[1];
        blocksinfo[1] = blocksinfo[0];
        blocksinfo[0].height = height;
        blocksinfo[0].transactions = transaction;
        char tmp[100];
        sprintf(tmp, "Block n°%lu\n%lu transactions", blocksinfo[0].height, blocksinfo[0].transactions);
        gtk_label_set_text(latest_block_name1, tmp);
        sprintf(tmp, "Block n°%lu\n%lu transactions", blocksinfo[1].height, blocksinfo[1].transactions);
        gtk_label_set_text(latest_block_name2, tmp);
        sprintf(tmp, "Block n°%lu\n%lu transactions", blocksinfo[2].height, blocksinfo[2].transactions);
        gtk_label_set_text(latest_block_name3, tmp);
    }
}

void update_sync(size_t actual, size_t final){
    if (actual >= final)
    {
        gtk_progress_bar_set_fraction(progress_bar_blockchain, (gdouble)1);
        change_label_text(synchro_label, "Synchronized");
        char tmp[100];
        sprintf(tmp, "%lu blocks", final);
        change_label_text(block_amount_label, tmp);
        return;
    }
    gdouble percent = (double)actual/(double)final;
    gtk_progress_bar_set_fraction(progress_bar_blockchain, percent);
    change_label_text(synchro_label, "Synchronization");
    char tmp[100];
    sprintf(tmp, "%lu out of %lu blocks", actual, final);
    change_label_text(block_amount_label, "");
    
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
    if(strcmp(gtk_entry_get_text(transa_amount), "") == 0)
    {
        gtk_label_set_text(error_label, "Invalid transaction amount");
    }
    else if(strcmp(gtk_entry_get_text(recipient_key), "") == 0 && gtk_combo_box_get_active(contacts_combo) == -1)
    {
        gtk_label_set_text(error_label, "Invalid transaction recipient");
    }
     //Call to transaction function
    else if(strcmp(gtk_entry_get_text(transa_amount), ""))
     {
        const time_t date = time(NULL);
        char *time_str = ctime(&date);
        double amount = strtod(gtk_entry_get_text(transa_amount), NULL);
        char *public_key;
        if(strcmp(gtk_entry_get_text(recipient_key), ""))
        {
            public_key = (char *)gtk_entry_get_text(recipient_key);
            add_transaction_with_pkey(amount, public_key, time_str);
#ifndef TEST
            char *asset = calloc(1, 512);
            strcpy(asset, gtk_entry_get_text(asset_entry));
            char *cause = calloc(1, 512);
            strcpy(cause, gtk_entry_get_text(cause_entry));
            new_transaction(T_TYPE_DEFAULT, public_key, (size_t)(amount * 10e5), asset, cause);
            free(asset);
            free(cause);
#endif
        }
        else if(gtk_combo_box_get_active(contacts_combo) != -1)
        {
            char *name = "";
            GtkTreeModel *p_model = gtk_combo_box_get_model(contacts_combo);
            GtkTreeIter iter;
            if(gtk_combo_box_get_active_iter(contacts_combo, &iter))
                gtk_tree_model_get(p_model, &iter, 0, &name, -1);
            public_key = get_public_key_from_contacts(name);
            add_transaction_with_contact(amount, public_key, time_str);
#ifndef TEST
            char *asset = calloc(1, 512);
            strcpy(asset, gtk_entry_get_text(asset_entry));
            char *cause = calloc(1, 512);
            strcpy(cause, gtk_entry_get_text(cause_entry));
            new_transaction(T_TYPE_DEFAULT, public_key, (size_t)(amount * 10e5), asset, cause);
            free(asset);
            free(cause);
#endif
            free(public_key);
        }
     }
     gtk_entry_set_text(transa_amount, "");
     gtk_entry_set_text(recipient_key, "");
     gtk_entry_set_text(asset_entry, "");
     gtk_entry_set_text(cause_entry, "");
     gtk_label_set_text(error_label, "Transaction send!");
     sleep(1);
     gtk_label_set_text(error_label, "");
    

     return TRUE;
}

void add_transaction_with_pkey(double amount, char *public_key, char *date)
{
        FILE *th_f = fopen("data/ui/.transa_history", "a");
        if(th_f == NULL)
            err(-1, "Couldn't open transaction history file");

        GtkTreeIter iter;

        gtk_tree_store_append(ts_th, &iter, NULL);
        gtk_tree_store_set(ts_th, &iter, 0, amount, -1);
        gtk_tree_store_set(ts_th, &iter, 1, public_key, -1);
        gtk_tree_store_set(ts_th, &iter, 2, date, -1);
        fprintf(th_f, "%lf", amount);
        fprintf(th_f, "%s", public_key);
        fprintf(th_f, "\r");
        fprintf(th_f, "%s", date);

        fclose(th_f);
}

void add_transaction_with_contact(double amount, char *public_key, char *date)
{
        FILE *th_f = fopen("data/ui/.transa_history", "a");
        if(th_f == NULL)
            err(-1, "Couldn't open transaction history file");

        GtkTreeIter iter;

        gtk_tree_store_append(ts_th, &iter, NULL);
        gtk_tree_store_set(ts_th, &iter, 0, amount, -1);
        gtk_tree_store_set(ts_th, &iter, 1, public_key, -1);
        gtk_tree_store_set(ts_th, &iter, 2, date, -1);
        fprintf(th_f, "%lf", amount);
        fprintf(th_f, "%s", public_key);
        fprintf(th_f, "\r");
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
    char buff[2000];
    struct stat st = {0};
    if (stat("data/ui/.transa_history", &st) == -1)
    {
        FILE *th_f2 = fopen("data/ui/.transa_history", "w");
        fclose(th_f2);
    }
    FILE *th_f = fopen("data/ui/.transa_history", "r");
    if(th_f == NULL)
        err(-1, "Couldn't open transaction history file");

    while(1)
    {
        double amount = 0;
        char *public_key = malloc(sizeof(buff));
        char *date = malloc(sizeof(buff));
        fscanf(th_f, "%lf", &amount);
        fscanf(th_f, "%[^\r]", public_key);
        fscanf(th_f, "%[^\n]", date);
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
    const time_t date = time(NULL);
    char *time_str = ctime(&date);
    double amount = strtod(gtk_entry_get_text(invest_entry), NULL);
    add_transaction_with_pkey(amount, "Add Stake", time_str);
#ifndef TEST
    char *asset = calloc(1, 512);
    char *cause = calloc(1, 512);
    new_transaction(T_TYPE_ADD_STAKE, NULL, (size_t)(amount * 10e5), asset, cause);
    free(asset);
    free(cause);
#endif

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
    const time_t date = time(NULL);
    char *time_str = ctime(&date);
    double amount = strtod(gtk_entry_get_text(recover_entry), NULL);
    add_transaction_with_pkey(amount, "Withdrow Stake", time_str);
#ifndef TEST
    char *asset = calloc(1, 512);
    char *cause = calloc(1, 512);
    new_transaction(T_TYPE_WITHDRAW_STAKE, NULL, (size_t)(amount * 10e5), asset, cause);
    free(asset);
    free(cause);
#endif
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
    if(strcmp(name, "") && strcmp(public_key ,""))
    {
        char file[300] = "data/contact/";
        strcpy(file + strlen(file), name);
        FILE *contacts_f = fopen(file, "w");
        if(contacts_f == NULL)
            err(-1, "Couldn't open contacts file");

        fprintf(contacts_f, "%s", public_key);

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
    DIR *d;
    struct dirent *dir;
    d = opendir("data/contact/");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG)
            {
                strcpy(buff, "data/contact/");
                FILE *contacts_f = fopen(strcat(buff, dir->d_name), "r");
                if(contacts_f == NULL)
                    err(-1, "Couldn't open contacts file");
                char *public_key = malloc(2000);
                fread(public_key, 2000, 1, contacts_f);
                add_contacts_from_file(dir->d_name , public_key);
                add_contact_to_combobox(dir->d_name);
                free(public_key);
                fclose(contacts_f);
            }
        }
        closedir(d);
    }
}

char *get_public_key_from_contacts(const char *name)
{
    char buff[300];
    strcpy(buff, "data/contact/");
    int contacts_f = open(strcat(buff, name), O_RDONLY);
    if(contacts_f == 0)
        return NULL;

    char *public_key = malloc(2000);
    size_t r = read(contacts_f, public_key, 2000);
    public_key[r+1] = '\0';
    
    close(contacts_f);
    return public_key;

}



gboolean on_create_key_but1_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    struct stat st = {0};
    if (stat("data/ui/.password", &st) == -1)
    {
        gtk_widget_show(create_key_window);
        return TRUE;
    }
    change_label_text(password_error_label, "Account already exist");
    return TRUE;

}

gboolean on_create_key_but2_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    size_t bufflen = strlen(gtk_entry_get_text(password_entry2));
    if (bufflen < 5){
        change_label_text(password_error_label, "Invalid password\n(min 5 characters)");
        gtk_widget_hide(create_key_window);
        return TRUE;
    }
    char *buff = malloc(sizeof(char) * bufflen);
    memcpy(buff, gtk_entry_get_text(password_entry2), bufflen);
    size_t keylen = strlen(gtk_entry_get_text(key_entry));
    if (keylen)
    {
        char *buffkey = malloc(keylen);
        memcpy(buff, gtk_entry_get_text(key_entry), keylen);

        FILE* rsa_public_file = fopen("data/keys/rsa.pub", "wb");
        FILE* rsa_private_file = fopen("data/keys/rsa", "wb");

        if (!rsa_private_file || !rsa_public_file)
            err(errno, "Impossible to write 'data/keys/rsa.pub' and data/keys/rsa files");

        if (fwrite(buffkey, keylen, 1, rsa_private_file) == 0)
            err(errno, "Impossible to write data in 'data/keys/rsa'");
        fclose(rsa_private_file);

        if (fwrite(buffkey, keylen, 1, rsa_private_file) == 0)
            err(errno, "Impossible to write data in 'data/keys/rsa.pub'");
        fclose(rsa_public_file);
    }
    get_keys(buff);
    char *hashed = sha384_data(buff, bufflen);
    FILE *password_f = fopen("data/ui/.password", "wb");
    if(password_f == NULL)
        err(-1, "Couldn't open password file");
    fwrite(hashed, 48, 1, password_f);
    free(buff);
    free(hashed);
    fclose(password_f);
    gtk_widget_hide(create_key_window);
    return TRUE;
}

gboolean on_connect_but_press(__attribute__ ((unused)) GtkWidget *widget,
                    __attribute__ ((unused)) GdkEventKey *event,
                    __attribute__ ((unused)) gpointer user_data)
{
    struct stat st = {0};
    if (stat("data/ui/.password", &st) == -1)
    {
        change_label_text(password_error_label, "No password, please create an account");
        return TRUE;
    }
    FILE *password_f = fopen("data/ui/.password", "r");
    if(password_f == NULL)
        err(-1, "Couldn't open password file");

    size_t bufflen = strlen(gtk_entry_get_text(password_entry1));
    if (bufflen < 5){
        change_label_text(password_error_label, "Invalid password\n(min 5 characters)");
        gtk_widget_hide(create_key_window);
        return TRUE;
    }

    char *buff = malloc(sizeof(char) * bufflen);
    memcpy(buff, gtk_entry_get_text(password_entry1), bufflen);
    char *hashed = sha384_data(buff, bufflen);
    char *buff_hashed = malloc(48 * sizeof(char));
    fread(buff_hashed, 1, 48, password_f);
    if(strncmp(buff_hashed, hashed, 48))
    {
        free(buff_hashed);
        free(hashed);
        free(buff);
        fclose(password_f);
        gtk_label_set_text(password_error_label, "Wrong password");
    }
    else
    {
        get_keys(buff);
        free(buff_hashed);
        free(hashed);
        free(buff);
        fclose(password_f);
        update_labels();
        load_contacts_from_file();
        load_transactions_from_file();
        gtk_widget_hide(connection_window);
        gtk_widget_show(window);
    }
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
    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, wallet->pub_key);
    char temp[5000];
    int rsa_size = BIO_pending(pubkey);
    BIO_read(pubkey, temp, rsa_size);
    temp[rsa_size] = 0;
    gtk_label_set_text(public_key_label, temp);
    BIO_free_all(pubkey);
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
