/* unity-panel.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel.h"

#include "unity-panel-battery-icon-private.h"
#include "unity-panel-calendar-label-private.h"
#include "unity-panel-power-profiles-icon-private.h"
#include "unity-panel-sheet-private.h"

struct _UnityPanel
{
  UnityStrip parent_instance;

  GtkToggleButton *indicator;
  UnityPanelSheet *sheet;
};

G_DEFINE_FINAL_TYPE (UnityPanel, unity_panel, UNITY_TYPE_STRIP)

static void
sheet_open_cb (UnityPanel *self)
{
  gtk_toggle_button_set_active (self->indicator, unity_panel_sheet_get_open (self->sheet));
}

static UnityPanelSheet *
ensure_sheet (UnityPanel *self)
{
  if (self->sheet != NULL)
    return self->sheet;

  self->sheet = unity_panel_sheet_new (gtk_window_get_application (GTK_WINDOW (self)));
  g_object_bind_property (self, "layer", self->sheet, "layer", G_BINDING_SYNC_CREATE);
  g_signal_connect_object (self->sheet, "notify::open", G_CALLBACK (sheet_open_cb), self, G_CONNECT_SWAPPED);

  return self->sheet;
}

static GdkMonitor *
current_monitor (UnityPanel *self)
{
  GdkMonitor *monitor = unity_window_get_gdkmonitor (UNITY_WINDOW (self));
  GdkSurface *surface = gtk_native_get_surface (GTK_NATIVE (self));

  if (monitor == NULL && surface != NULL)
    monitor = gdk_display_get_monitor_at_surface (gtk_widget_get_display (GTK_WIDGET (self)), surface);

  return monitor;
}

static void
indicator_active_cb (UnityPanel *self)
{
  UnityPanelSheet  *sheet;
  GdkMonitor       *monitor;
  graphene_point_t  centre;

  if (!gtk_toggle_button_get_active (self->indicator))
    {
      if (self->sheet != NULL)
        unity_panel_sheet_popdown (self->sheet);
      return;
    }

  sheet   = ensure_sheet (self);
  monitor = current_monitor (self);
  if (unity_panel_sheet_get_open (sheet) || monitor == NULL)
    return;

  if (!gtk_widget_compute_point (GTK_WIDGET (self->indicator), GTK_WIDGET (self),
                                 &GRAPHENE_POINT_INIT (gtk_widget_get_width (GTK_WIDGET (self->indicator)) / 2.0f, 0),
                                 &centre))
    return;

  unity_panel_sheet_popup (sheet, monitor, gtk_widget_get_height (GTK_WIDGET (self)), centre.x);
}

static void
unity_panel_dispose (GObject *object)
{
  UnityPanel *self = UNITY_PANEL (object);

  if (self->sheet != NULL)
    gtk_window_destroy (GTK_WINDOW (g_steal_pointer (&self->sheet)));
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_TYPE_PANEL);

  G_OBJECT_CLASS (unity_panel_parent_class)->dispose (object);
}

static void
unity_panel_class_init (UnityPanelClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_dispose;

  g_type_ensure (UNITY_PANEL_TYPE_BATTERY_ICON);
  g_type_ensure (UNITY_PANEL_TYPE_CALENDAR_LABEL);
  g_type_ensure (UNITY_PANEL_TYPE_POWER_PROFILES_ICON);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanel, indicator);
  gtk_widget_class_bind_template_callback (widget_class, indicator_active_cb);
  gtk_widget_class_set_css_name (widget_class, "unity-panel");
}

static void
unity_panel_init (UnityPanel *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

UnityPanel *
unity_panel_new (GtkApplication *app)
{
  g_return_val_if_fail (GTK_IS_APPLICATION (app), NULL);

  return g_object_new (UNITY_TYPE_PANEL, "application", app, NULL);
}
