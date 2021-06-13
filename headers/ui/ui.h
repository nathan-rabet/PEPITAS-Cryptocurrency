#ifndef UI_H
#define UI_H

#include <dirent.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <time.h>
#include "network/network.h"
#include "cryptosystem/rsa.h"
#include "cryptosystem/hash.h"
#include "blockchain/wallet.h"
#include "blockchain/block.h"
#include "client.h"

extern GtkLabel *balance_1;
extern GtkLabel *balance_2;
extern GtkLabel *stake_label1;
extern GtkLabel *stake_label2;
extern GtkLabel *stake_label3;
extern GtkLabel *synchro_label;
extern GtkLabel *block_amount_label;
extern GtkLabel *connections_label;
extern GtkLabel *mempool_label;
struct blockinfo
{
    size_t height;
    size_t transactions;
};

struct blockinfo blocksinfo[3];


/**
 * @brief Setups the gtk widgets for the GUI
 * @return int Returns 1 if there is an error, 0 otherwise
 */
void* setup(void* args);

/**
 * @brief Destroys the window when it is closed
 * 
 * @param widget The main window of the GUI
 * @return gboolean Error code
 */
gboolean on_main_window_delete(GtkWidget *widget, __attribute__ ((unused)) gpointer data);

/**
 * @brief Quits GTK when the program ends
 * 
 * 
 */
void on_main_window_destroy(__attribute ((unused)) GtkWidget *widget, __attribute__ ((unused)) gpointer data);

/**
 * @brief Will be used when the transaction function is ready
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error code
 */
gboolean on_transaction_button_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/**
 * @brief Opens the invest window
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean 
 */
gboolean on_invest_button1_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/**
 * @brief Resets the entry in the invest window and closes it, will later be used for the invest function
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error Code
 */
gboolean on_invest_button2_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/**
 * @brief Opens the recover window
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error code
 */
gboolean on_recover_button1_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/**
 * @brief Resets the entry in the recover window and closes it, will later be used for the recover function
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error code
 */
gboolean on_recover_button2_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/**
 * @brief Opens the contact window
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error code
 */
gboolean on_add_contact_button1_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

/**
 * @brief Adds a contact to the treeview if the entrys weren't empty, and closes the contact window
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error code
 */
gboolean add_contact(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

void change_label_text(GtkLabel *label, char *text);
gboolean on_create_key_but1_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean on_create_key_but2_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean on_connect_but_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void add_contacts_from_file(char *name, char *public_key);
void load_contacts_from_file();
void add_contact_to_combobox(char *name);
void update_labels();
void add_transaction_with_pkey(double amount, char *public_key, char *date);
void add_transaction_with_contact(double amount, char *public_key, char *date);
void add_transaction_from_file(double amount, char *public_key, char *date);
void load_transaction_from_file();
char *get_public_key_from_contacts(const char *name);
void change_label_text(GtkLabel *label, char* text);
void add_new_blockinfo(size_t height, size_t transaction);
void update_sync(size_t actual, size_t final);
gboolean set_block_viewer_plus(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean set_block_viewer_minus(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void set_block_viewer(int height);
#endif
