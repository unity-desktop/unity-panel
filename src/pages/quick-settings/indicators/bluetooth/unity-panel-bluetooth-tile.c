/* unity-panel-bluetooth-tile.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-bluetooth-tile-private.h"

#include <astal-bluetooth.h>

struct _UnityPanelBluetoothTile
{
  GtkToggleButton parent_instance;

  GtkImage                *icon;
  GtkLabel                *subtitle;
  AstalBluetoothBluetooth *bluetooth;
};

G_DEFINE_FINAL_TYPE (UnityPanelBluetoothTile, unity_panel_bluetooth_tile, GTK_TYPE_TOGGLE_BUTTON)

static gchar *
connected_text (UnityPanelBluetoothTile *self)
{
  g_autoptr (GList) devices = astal_bluetooth_bluetooth_get_devices (self->bluetooth);
  AstalBluetoothDevice *first = NULL;
  guint                 count = 0;

  for (GList *l = devices; l != NULL; l = l->next)
    {
      if (!astal_bluetooth_device_get_connected (l->data))
        continue;

      if (count++ == 0)
        first = l->data;
    }

  if (count == 0)
    return g_strdup ("On");

  if (count == 1)
    {
      g_autofree gchar *alias = astal_bluetooth_device_get_alias (first);

      if (alias != NULL && *alias != '\0')
        return g_steal_pointer (&alias);
    }

  return g_strdup_printf ("%u devices", count);
}

static void
update (UnityPanelBluetoothTile *self)
{
  g_autofree gchar *text = NULL;
  gboolean          powered;

  if (self->bluetooth == NULL || astal_bluetooth_bluetooth_get_adapter (self->bluetooth) == NULL)
    {
      gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
      return;
    }

  powered = astal_bluetooth_bluetooth_get_is_powered (self->bluetooth);
  text    = powered ? connected_text (self) : g_strdup ("Off");

  gtk_image_set_from_icon_name (self->icon,
                                !powered ? "bluetooth-disabled-symbolic"
                                : astal_bluetooth_bluetooth_get_is_connected (self->bluetooth) ? "bluetooth-active-symbolic"
                                : "bluetooth-symbolic");
  gtk_label_set_label (self->subtitle, text);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (self), powered);
  gtk_widget_set_visible (GTK_WIDGET (self), TRUE);
}

static void
watch_device (UnityPanelBluetoothTile *self,
              AstalBluetoothDevice    *device)
{
  g_signal_connect_object (device, "notify::connected", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
  g_signal_connect_object (device, "notify::alias", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
}

static void
device_added_cb (UnityPanelBluetoothTile *self,
                 AstalBluetoothDevice    *device)
{
  watch_device (self, device);
  update (self);
}

static void
toggled_cb (UnityPanelBluetoothTile *self)
{
  AstalBluetoothAdapter *adapter;
  gboolean               active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self));

  if (self->bluetooth == NULL)
    return;

  adapter = astal_bluetooth_bluetooth_get_adapter (self->bluetooth);
  if (adapter != NULL && astal_bluetooth_bluetooth_get_is_powered (self->bluetooth) != active)
    astal_bluetooth_adapter_set_powered (adapter, active);
}

static void
unity_panel_bluetooth_tile_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_BLUETOOTH_TILE);

  G_OBJECT_CLASS (unity_panel_bluetooth_tile_parent_class)->dispose (object);
}

static void
unity_panel_bluetooth_tile_class_init (UnityPanelBluetoothTileClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_bluetooth_tile_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-bluetooth-tile.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelBluetoothTile, icon);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelBluetoothTile, subtitle);
  gtk_widget_class_bind_template_callback (widget_class, toggled_cb);
}

static void
unity_panel_bluetooth_tile_init (UnityPanelBluetoothTile *self)
{
  const gchar      *signals[] = { "notify::adapter", "notify::is-powered", "notify::is-connected", "device-removed" };
  g_autoptr (GList) devices   = NULL;

  gtk_widget_init_template (GTK_WIDGET (self));

  self->bluetooth = astal_bluetooth_get_default ();
  if (self->bluetooth == NULL)
    {
      update (self);
      return;
    }

  for (guint i = 0; i < G_N_ELEMENTS (signals); i++)
    g_signal_connect_object (self->bluetooth, signals[i], G_CALLBACK (update), self, G_CONNECT_SWAPPED);
  g_signal_connect_object (self->bluetooth, "device-added", G_CALLBACK (device_added_cb), self, G_CONNECT_SWAPPED);

  devices = astal_bluetooth_bluetooth_get_devices (self->bluetooth);
  for (GList *l = devices; l != NULL; l = l->next)
    watch_device (self, l->data);

  update (self);
}
