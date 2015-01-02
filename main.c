#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>
#include <cairo.h>

/*FALTA:
  - Abrir o manual (dar mensagem de erro em caso de falha E compatibilidade com Windows/Mac)

*/






gboolean
help_manual(void)
{
  short int x;
  //x = system("xdg-open GCDmanual.pdf");
  /*if (x==-1)
    {
  */
  return TRUE;
}


gboolean
on_expose_event (GtkWidget       *widget ,
                 GdkEventExpose  *event  ,
                 gpointer         data   )
{
  cairo_t        *cr ;
  gchar           texto[128] ;
  gint            width, height;

  static double   pos1x = 100., pos1y = 80., pos2x = 600., pos2y = 100.;
  static double   s1x = 1., s1y = 0., s2x = 0., s2y = 0.; 
  static double   velscale1=4, velscale2=0;
  double r1=30, r2=50;

  gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
  cr = gdk_cairo_create(widget->window);

  cairo_move_to (cr, 20, 20);
  cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size (cr, 18);
  sprintf (texto, "Exemplo de funcionamento de GTK+ Cairo                 Velocidade: %d\n",
           (gint) velscale1);
  cairo_show_text (cr, texto);

  /* Circulo1 */

  cairo_set_source_rgb (cr, 0., 0., 0.);
  cairo_set_line_width (cr, 4.0);
  cairo_arc (cr, pos1x, pos1y, r1, 0., 2. * M_PI);
  cairo_stroke_preserve (cr);
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_fill (cr);

  if ((pos1x < 30.) || ((pos1x + 30.) > width))
    s1x = - s1x;


  if ((pos1y < 30.) || ((pos1y + 30.) > height))
    s1y = - s1y;
  /* Circulo2 */

  cairo_set_source_rgb (cr, 0., 0., 0.);
  cairo_set_line_width (cr, 4.0);
  cairo_arc (cr, pos2x, pos2y, r2, 0., 2. * M_PI);
  cairo_stroke_preserve (cr);
  cairo_set_source_rgb (cr, 0, 1, 0);
  cairo_fill (cr);

  if ((pos2x < r2) || ((pos2x + r2) > width))
    s2x = - s2x;


  if ((pos2y < r2) || ((pos2y + r2) > height))
    s2y = - s2y;
		
  if ((pos1y-pos2y)*(pos1y-pos2y)+(pos1x-pos2x)*(pos1x-pos2x) <= (r1+r2)*(r1+r2))
    {
      printf("COLISAO");
      //angulovelrel
      printf("%lf", atan2((s2y*velscale2-s1y*velscale1),(s2x*velscale2-s1x*velscale1)));
      printf("%lf", atan2((pos2y-pos1y),(pos2x-pos1x)));
    }
  else
    {	
      pos1x = pos1x + s1x * velscale1;
      pos1y = pos1y + s1y * velscale1;


		
      pos2x = pos2x + s2x * velscale2;
      pos2y = pos2y + s2y * velscale2;
    }


  cairo_stroke(cr);
  cairo_destroy(cr);

  return FALSE;
}

static gboolean
time_handler (GtkWidget *widget)
{
  if (widget->window == NULL) 
    return FALSE;

  gtk_widget_queue_draw(widget);

  return TRUE;
}


int
main(int argc, char **argv)
{

  GtkWidget *janela;
  GtkWidget *vbox1, *hbox2, *vbox3_sett, *vbox3_field;
  GtkWidget *vboxseparator1;
  GtkWidget *menubar, *lvl1file, *lvl2file, *lvl1help, *lvl2help;
  GtkWidget *file_new, * file_open, *file_sep1, *file_quit;
  GtkWidget *help_manual, *help_about;
  GtkWidget *toolbar;
  GtkToolItem *tb_restart, *tb_pause, *tb_undo, *tb_redo, *tb_sep1;
  GtkWidget *statusbar;



  gtk_init(&argc, &argv);

  janela=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(janela), "Grande Colisor de Discos");
  gtk_window_set_position (GTK_WINDOW (janela), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW(janela), 800, 600); 

  g_signal_connect_swapped (G_OBJECT(janela), "destroy",
                            G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect (janela, "expose-event", G_CALLBACK(on_expose_event), NULL);


  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_box_set_homogeneous (GTK_BOX (vbox1), FALSE);
  gtk_container_add (GTK_CONTAINER(janela), vbox1);


  /* MENU */

  menubar = gtk_menu_bar_new ();
  gtk_box_pack_start (GTK_BOX (vbox1), menubar, FALSE, TRUE, 0);

  lvl1file= gtk_menu_item_new_with_mnemonic ("_Ficheiro"); 
  gtk_menu_shell_append (GTK_MENU_SHELL(menubar), lvl1file); 
  lvl2file = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (lvl1file), lvl2file);

  file_new = gtk_image_menu_item_new_from_stock (GTK_STOCK_NEW, NULL);
#if GTK_CHECK_VERSION(2,18,0)
  gtk_image_menu_item_set_always_show_image (GTK_IMAGE_MENU_ITEM (file_new), TRUE);
#endif

  file_open = gtk_image_menu_item_new_from_stock (GTK_STOCK_OPEN, NULL);
#if GTK_CHECK_VERSION(2,18,0)
  gtk_image_menu_item_set_always_show_image (GTK_IMAGE_MENU_ITEM (file_open), TRUE);
#endif

  file_sep1= gtk_separator_menu_item_new();
  file_quit= gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
#if GTK_CHECK_VERSION(2,18,0)
  gtk_image_menu_item_set_always_show_image (GTK_IMAGE_MENU_ITEM (file_quit), TRUE);
#endif

  gtk_menu_shell_append (GTK_MENU_SHELL (lvl2file), file_new);
  gtk_menu_shell_append (GTK_MENU_SHELL (lvl2file), file_open);
  gtk_menu_shell_append (GTK_MENU_SHELL (lvl2file), file_sep1);
  gtk_menu_shell_append (GTK_MENU_SHELL (lvl2file), file_quit);



  //Ajuda

  lvl1help = gtk_menu_item_new_with_mnemonic ("_Ajuda");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), lvl1help);
  lvl2help = gtk_menu_new();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (lvl1help), lvl2help);
  help_manual=gtk_menu_item_new_with_label("Manual de ajuda");
  help_about= gtk_menu_item_new_with_label("Sobre...");

  gtk_menu_shell_append(GTK_MENU_SHELL(lvl2help), help_manual);
  gtk_menu_shell_append(GTK_MENU_SHELL(lvl2help), help_about);



  //Menu Callbacks

  g_signal_connect(G_OBJECT(file_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(help_manual), "activate", G_CALLBACK(help_manual), NULL);



  /* TOOLBAR */

  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
  gtk_container_set_border_width (GTK_CONTAINER (toolbar), 0);

  tb_undo = gtk_tool_button_new_from_stock (GTK_STOCK_UNDO);
  gtk_widget_set_name (GTK_WIDGET (tb_undo), "undo");
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), tb_undo, -1);

  tb_redo = gtk_tool_button_new_from_stock (GTK_STOCK_REDO);
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), tb_redo, -1);

  tb_sep1 = gtk_separator_tool_item_new ();
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), tb_sep1, -1);

  tb_pause = gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PAUSE);
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), tb_pause, -1);

  tb_restart = gtk_tool_button_new_from_stock (GTK_STOCK_REFRESH);
  gtk_toolbar_insert (GTK_TOOLBAR (toolbar), tb_restart, -1);

  /*tb_sep = gtk_separator_tool_item_new ();
    gtk_toolbar_insert (GTK_TOOLBAR (toolbar), tb_sep, -1);*/
  
  gtk_box_pack_start (GTK_BOX(vbox1), toolbar, FALSE, FALSE, 0); 

  /* HBOX2 */

  vboxseparator1 = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox1), vboxseparator1, FALSE, TRUE, 0);
  gtk_widget_show (vboxseparator1);

  hbox2= gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(vbox1), hbox2); 

  /* STATUSBAR */

  statusbar = gtk_statusbar_new();
  gtk_box_pack_end (GTK_BOX (vbox1), statusbar, FALSE, TRUE, 0);
  gtk_statusbar_push  (GTK_STATUSBAR (statusbar), 1, "  Isto e' uma     statusbar.");

  gtk_widget_show_all (janela);
  gtk_main();


  /* ÁREA DE TRABALHO */

  vbox3_field = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (hbox2), vbox3_field);

  vbox3_sett = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (hbox2), vbox3_sett);

  gtk_widget_set_app_paintable (vbox3_field, TRUE);
  
  }
