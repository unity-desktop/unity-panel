/* unity-panel-battery-icon.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-battery-icon-private.h"

#include <astal-battery.h>

struct _UnityPanelBatteryIcon
{
  AdwBin parent_instance;

  GtkImage           *icon;
  GtkLabel           *label;
  AstalBatteryDevice *battery;
};

G_DEFINE_FINAL_TYPE (UnityPanelBatteryIcon, unity_panel_battery_icon, ADW_TYPE_BIN)

static void
update (UnityPanelBatteryIcon *self)
{
  g_autofree gchar *text = NULL;

  if (self->battery == NULL)
    {
      gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
      return;
    }

  text = g_strdup_printf ("%.0f%%", astal_battery_device_get_percentage (self->battery) * 100.0);
  gtk_image_set_from_icon_name (self->icon, astal_battery_device_get_battery_icon_name (self->battery));
  gtk_label_set_label (self->label, text);
  gtk_widget_set_visible (GTK_WIDGET (self),
                          astal_battery_device_get_is_battery (self->battery) &&
                          astal_battery_device_get_is_present (self->battery));
}

static void
unity_panel_battery_icon_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_BATTERY_ICON);

  G_OBJECT_CLASS (unity_panel_battery_icon_parent_class)->dispose (object);
}

static void
unity_panel_battery_icon_class_init (UnityPanelBatteryIconClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_battery_icon_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-battery-icon.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelBatteryIcon, icon);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelBatteryIcon, label);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-battery-icon");
}

static void
unity_panel_battery_icon_init (UnityPanelBatteryIcon *self)
{
  const gchar *signals[] = { "notify::battery-icon-name", "notify::percentage", "notify::is-battery", "notify::is-present" };

  gtk_widget_init_template (GTK_WIDGET (self));

  self->battery = astal_battery_get_default ();
  for (guint i = 0; self->battery != NULL && i < G_N_ELEMENTS (signals); i++)
    g_signal_connect_object (self->battery, signals[i], G_CALLBACK (update), self, G_CONNECT_SWAPPED);

  update (self);
}
