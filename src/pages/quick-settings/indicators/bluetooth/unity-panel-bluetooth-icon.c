/* unity-panel-bluetooth-icon.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-bluetooth-icon-private.h"

#include <astal-bluetooth.h>

struct _UnityPanelBluetoothIcon
{
  AdwBin parent_instance;

  GtkImage                *image;
  AstalBluetoothBluetooth *bluetooth;
};

G_DEFINE_FINAL_TYPE (UnityPanelBluetoothIcon, unity_panel_bluetooth_icon, ADW_TYPE_BIN)

static void
update (UnityPanelBluetoothIcon *self)
{
  if (self->bluetooth == NULL ||
      astal_bluetooth_bluetooth_get_adapter (self->bluetooth) == NULL ||
      !astal_bluetooth_bluetooth_get_is_powered (self->bluetooth))
    {
      gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
      return;
    }

  gtk_image_set_from_icon_name (self->image,
                                astal_bluetooth_bluetooth_get_is_connected (self->bluetooth)
                                ? "bluetooth-active-symbolic"
                                : "bluetooth-symbolic");
  gtk_widget_set_visible (GTK_WIDGET (self), TRUE);
}

static void
unity_panel_bluetooth_icon_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_BLUETOOTH_ICON);

  G_OBJECT_CLASS (unity_panel_bluetooth_icon_parent_class)->dispose (object);
}

static void
unity_panel_bluetooth_icon_class_init (UnityPanelBluetoothIconClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_bluetooth_icon_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-bluetooth-icon.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelBluetoothIcon, image);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-bluetooth-icon");
}

static void
unity_panel_bluetooth_icon_init (UnityPanelBluetoothIcon *self)
{
  const gchar *signals[] = { "notify::adapter", "notify::is-powered", "notify::is-connected" };

  gtk_widget_init_template (GTK_WIDGET (self));

  self->bluetooth = astal_bluetooth_get_default ();
  for (guint i = 0; self->bluetooth != NULL && i < G_N_ELEMENTS (signals); i++)
    g_signal_connect_object (self->bluetooth, signals[i], G_CALLBACK (update), self, G_CONNECT_SWAPPED);

  update (self);
}
