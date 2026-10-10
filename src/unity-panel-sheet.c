/* unity-panel-sheet.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-sheet-private.h"

#include <unity-edge-sheet.h>

#include "unity-panel-calendar-page-private.h"
#include "unity-panel-quick-settings-page-private.h"
#include "unity-panel-sheet-page-private.h"

struct _UnityPanelSheet
{
  UnityWindow parent_instance;

  UnityEdgeSheet *sheet;
  GtkWidget      *body;
  AdwToggleGroup *pages;
  AdwViewStack   *stack;
};

G_DEFINE_FINAL_TYPE (UnityPanelSheet, unity_panel_sheet, UNITY_TYPE_WINDOW)

typedef enum
{
  PROP_OPEN = 1,
} UnityPanelSheetProperty;

static GParamSpec *properties[PROP_OPEN + 1];

static void
open_cb (UnityPanelSheet *self)
{
  g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_OPEN]);
}

static gboolean
escape_cb (GtkWidget *widget,
           GVariant  *args,
           gpointer   user_data)
{
  unity_panel_sheet_popdown (UNITY_PANEL_SHEET (widget));

  return GDK_EVENT_STOP;
}

static void
sheet_size_cb (UnityPanelSheet *self)
{
  if (unity_edge_sheet_get_open (self->sheet) || unity_edge_sheet_get_sheet_size (self->sheet) > 0)
    return;

  gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
  adw_toggle_group_set_active (self->pages, 0);
}

static void
add_toggles (UnityPanelSheet *self)
{
  g_autoptr (GtkSelectionModel) pages = adw_view_stack_get_pages (self->stack);

  for (guint i = 0; i < g_list_model_get_n_items (G_LIST_MODEL (pages)); i++)
    {
      g_autoptr (AdwViewStackPage) page = g_list_model_get_item (G_LIST_MODEL (pages), i);
      GtkWidget                   *child = adw_view_stack_page_get_child (page);
      AdwToggle                   *toggle;

      if (!UNITY_PANEL_IS_SHEET_PAGE (child))
        continue;

      toggle = unity_panel_sheet_page_get_toggle (UNITY_PANEL_SHEET_PAGE (child));
      adw_view_stack_page_set_name (page, adw_toggle_get_name (toggle));
      adw_toggle_group_add (self->pages, g_object_ref (toggle));
    }

  adw_toggle_group_set_active (self->pages, 0);
}

static void
unity_panel_sheet_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_SHEET);

  G_OBJECT_CLASS (unity_panel_sheet_parent_class)->dispose (object);
}

static void
unity_panel_sheet_get_property (GObject    *object,
                                guint       prop_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
  UnityPanelSheet *self = UNITY_PANEL_SHEET (object);

  switch ((UnityPanelSheetProperty) prop_id)
    {
    case PROP_OPEN:
      g_value_set_boolean (value, unity_panel_sheet_get_open (self));
      break;
    }
}

static void
unity_panel_sheet_class_init (UnityPanelSheetClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose      = unity_panel_sheet_dispose;
  object_class->get_property = unity_panel_sheet_get_property;

  properties[PROP_OPEN] = g_param_spec_boolean ("open", NULL, NULL, FALSE,
                                                G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, G_N_ELEMENTS (properties), properties);

  g_type_ensure (UNITY_TYPE_EDGE_SHEET);
  g_type_ensure (UNITY_PANEL_TYPE_QUICK_SETTINGS_PAGE);
  g_type_ensure (UNITY_PANEL_TYPE_CALENDAR_PAGE);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-sheet.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSheet, sheet);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSheet, body);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSheet, pages);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSheet, stack);
  gtk_widget_class_bind_template_callback (widget_class, open_cb);
  gtk_widget_class_bind_template_callback (widget_class, sheet_size_cb);
  gtk_widget_class_add_binding (widget_class, GDK_KEY_Escape, 0, escape_cb, NULL);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-sheet");
}

static void
unity_panel_sheet_init (UnityPanelSheet *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  add_toggles (self);
}

UnityPanelSheet *
unity_panel_sheet_new (GtkApplication *app)
{
  g_return_val_if_fail (GTK_IS_APPLICATION (app), NULL);

  return g_object_new (UNITY_PANEL_TYPE_SHEET, "application", app, NULL);
}

gboolean
unity_panel_sheet_get_open (UnityPanelSheet *self)
{
  g_return_val_if_fail (UNITY_PANEL_IS_SHEET (self), FALSE);

  return unity_edge_sheet_get_open (self->sheet);
}

void
unity_panel_sheet_popup (UnityPanelSheet *self,
                         GdkMonitor      *monitor,
                         gint             top,
                         gdouble          x)
{
  GdkRectangle area;
  gint         width;

  g_return_if_fail (UNITY_PANEL_IS_SHEET (self));
  g_return_if_fail (GDK_IS_MONITOR (monitor));

  gdk_monitor_get_geometry (monitor, &area);
  gtk_widget_measure (self->body, GTK_ORIENTATION_HORIZONTAL, -1, NULL, &width, NULL, NULL);
  if (area.width > width)
    unity_edge_sheet_set_align (self->sheet, CLAMP ((x - width / 2.0) / (area.width - width), 0.0, 1.0));

  unity_window_set_gdkmonitor (UNITY_WINDOW (self), monitor);
  unity_window_set_margin_top (UNITY_WINDOW (self), top);
  gtk_window_present (GTK_WINDOW (self));
  unity_edge_sheet_set_open (self->sheet, TRUE);
  gtk_widget_grab_focus (GTK_WIDGET (self->pages));
}

void
unity_panel_sheet_popdown (UnityPanelSheet *self)
{
  g_return_if_fail (UNITY_PANEL_IS_SHEET (self));

  unity_edge_sheet_set_open (self->sheet, FALSE);
}
