#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief Setups the gtk widgets for the GUI
 * @return int Returns 1 if there is an error, 0 otherwise
 */
int setup();

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
 * @brief Hides the private key of the user, or shows it if it was already hidden
 * 
 * @param widget unused
 * @param event unused
 * @param user_data unused
 * @return gboolean Error code
 */
gboolean on_pkey_button_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);

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


#endif