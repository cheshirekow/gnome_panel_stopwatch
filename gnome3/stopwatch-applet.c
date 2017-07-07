/*
 *  \file   hello.c
 *
 *  \date   Jul 13, 2011
 *  \author Josh Bialkowski (jbialk@mit.edu)
 *  \brief  
 */

#include <string.h>

#include <gtk/gtk.h>
#include <panel-applet.h>

static GTimeVal        start;
static GTimeVal        end;


/**
 *  \brief  When a button is pressed, the stopwatch will restart
 */
static gboolean
on_button_press (GtkWidget      *event_box,
                     GdkEventButton *event,
                     gpointer        data)
{
    /* Don't react to anything other than the left mouse button;
       return FALSE so the event is passed to the default handler */
    if (event->button != 1)
        return FALSE;

    g_get_current_time(&start);

    return TRUE;
}




/**
 *  \brief  callback for the timeout function, checks the current time and
 *          calculates how long it's been since it started
 */
static gint
timeout_callback( gpointer data )
{
    static gchar    string[51];
    static int      first=1;

    g_get_current_time(&end);

    GtkWidget* label = data;

    int sec = end.tv_sec - start.tv_sec;
    int min = sec / 60;
    int hr  = min / 60;
    int day = hr / 24;

    sec -= min*60;
    min -= hr*60;

    if(day > 0)
        g_snprintf(string, 50, "%id %ih %im %is", day, hr, min, sec);
    else if(hr > 0)
        g_snprintf(string, 50, "%ih %im %is", hr, min, sec);
    else if(min > 0)
        g_snprintf(string, 50, "%im %is", min, sec);
    else if(sec > 0)
        g_snprintf(string, 50, "%is", sec);
    else
        g_snprintf(string, 50, "%s", "stopwatch");

    gtk_label_set_text( GTK_LABEL(label), string);

    return 1;
}







static gboolean
stopwatch_applet_fill (PanelApplet *applet)
{
    g_get_current_time(&start);

    GtkWidget *label = gtk_label_new ("Stopwatch");

    g_timeout_add( 1000, timeout_callback, label );

    GtkWidget* image = gtk_image_new_from_file ("/usr/share/pixmaps/timer-applet.png");
    GdkPixbuf* pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
    GdkPixbuf* scaled = gdk_pixbuf_scale_simple(pixbuf, 18,18,GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image),scaled);

    GtkWidget* event_box = gtk_event_box_new ();
    gtk_container_add (GTK_CONTAINER (event_box), image);

    g_signal_connect (G_OBJECT (event_box),
                      "button_press_event",
                      G_CALLBACK (on_button_press),
                      image);

    GtkWidget* hbox = gtk_hbox_new(FALSE,1);

    gtk_container_add(GTK_CONTAINER(hbox),event_box);
    gtk_container_add(GTK_CONTAINER(hbox),label);

    gtk_container_add( GTK_CONTAINER(applet), hbox );


    gtk_widget_show_all (GTK_WIDGET (applet));


    return TRUE;
}


/**
 *  \brief  main entry point
 */
static gboolean
stopwatch_applet_factory_callback(PanelApplet *applet, 
                                    const gchar *iid,
                                    gpointer data)
{
    gboolean retval = FALSE;

    if (g_strcmp0 (iid, "StopwatchApplet") == 0)
        retval = stopwatch_applet_fill (applet);

    return retval;
}

PANEL_APPLET_OUT_PROCESS_FACTORY ("StopwatchAppletFactory",
                                  PANEL_TYPE_APPLET,
                                  stopwatch_applet_factory_callback,
                                  NULL)
