/* unity-panel-network.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-network-private.h"

#include <astal-network.h>

#define WIRED_ICON   "network-wired-symbolic"
#define WIFI_ICON    "network-wireless-signal-good-symbolic"
#define OFFLINE_ICON "network-offline-symbolic"

struct _UnityPanelNetwork
{
  GObject parent_instance;

  AstalNetworkNetwork *network;
  AstalNetworkWifi    *wifi;
  AstalNetworkWired   *wired;

  gchar               *icon_name;
  gchar               *subtitle;
  gboolean             available;
  gboolean             present;
  gboolean             has_wifi;
  gboolean             wifi_enabled;
};

G_DEFINE_FINAL_TYPE (UnityPanelNetwork, unity_panel_network, G_TYPE_OBJECT)

typedef enum
{
  PROP_ICON_NAME = 1,
  PROP_SUBTITLE,
  PROP_AVAILABLE,
  PROP_PRESENT,
  PROP_HAS_WIFI,
  PROP_WIFI_ENABLED,
} UnityPanelNetworkProperty;

static GParamSpec *properties[PROP_WIFI_ENABLED + 1];

static void
set_string (UnityPanelNetwork *self,
            gchar            **field,
            const gchar       *value,
            guint              prop)
{
  if (g_set_str (field, value))
    g_object_notify_by_pspec (G_OBJECT (self), properties[prop]);
}

static void
set_boolean (UnityPanelNetwork *self,
             gboolean          *field,
             gboolean           value,
             guint              prop)
{
  if (*field == value)
    return;

  *field = value;
  g_object_notify_by_pspec (G_OBJECT (self), properties[prop]);
}

static const gchar *
icon_name (UnityPanelNetwork *self)
{
  const gchar *icon = NULL;

  switch (astal_network_network_get_primary (self->network))
    {
    case ASTAL_NETWORK_PRIMARY_WIRED:
      if (self->wired != NULL)
        icon = astal_network_wired_get_icon_name (self->wired);
      return icon != NULL && *icon != '\0' ? icon : WIRED_ICON;

    case ASTAL_NETWORK_PRIMARY_WIFI:
      if (self->wifi != NULL)
        icon = astal_network_wifi_get_icon_name (self->wifi);
      return icon != NULL && *icon != '\0' ? icon : WIFI_ICON;

    case ASTAL_NETWORK_PRIMARY_UNKNOWN:
      return OFFLINE_ICON;
    }

  return OFFLINE_ICON;
}

static const gchar *
subtitle (UnityPanelNetwork *self)
{
  const gchar *ssid;

  if (self->wired != NULL && astal_network_wired_get_internet (self->wired) == ASTAL_NETWORK_INTERNET_CONNECTED)
    return "Connected";

  if (self->wifi == NULL || !astal_network_wifi_get_enabled (self->wifi))
    return "Off";

  ssid = astal_network_wifi_get_ssid (self->wifi);

  return ssid != NULL && *ssid != '\0' ? ssid : "Not Connected";
}

static void
update (UnityPanelNetwork *self)
{
  gboolean wifi_enabled = self->wifi != NULL && astal_network_wifi_get_enabled (self->wifi);

  set_string (self, &self->icon_name, icon_name (self), PROP_ICON_NAME);
  set_string (self, &self->subtitle, subtitle (self), PROP_SUBTITLE);
  set_boolean (self, &self->has_wifi, self->wifi != NULL, PROP_HAS_WIFI);
  set_boolean (self, &self->present, self->wifi != NULL || self->wired != NULL, PROP_PRESENT);
  set_boolean (self, &self->wifi_enabled, wifi_enabled, PROP_WIFI_ENABLED);
  set_boolean (self, &self->available,
               astal_network_network_get_primary (self->network) != ASTAL_NETWORK_PRIMARY_UNKNOWN || wifi_enabled,
               PROP_AVAILABLE);
}

static void
watch (UnityPanelNetwork *self,
       gpointer          *slot,
       gpointer           device,
       const gchar      **signals)
{
  if (*slot == device)
    return;

  if (*slot != NULL)
    g_signal_handlers_disconnect_by_data (*slot, self);
  g_set_object ((GObject **) slot, device);

  for (guint i = 0; device != NULL && signals[i] != NULL; i++)
    g_signal_connect_object (device, signals[i], G_CALLBACK (update), self, G_CONNECT_SWAPPED);
}

static void
devices_cb (UnityPanelNetwork *self)
{
  const gchar *wifi_signals[]  = { "notify::icon-name", "notify::enabled", "notify::ssid", "notify::state", NULL };
  const gchar *wired_signals[] = { "notify::icon-name", "notify::internet", "notify::state", NULL };

  watch (self, (gpointer *) &self->wifi, astal_network_network_get_wifi (self->network), wifi_signals);
  watch (self, (gpointer *) &self->wired, astal_network_network_get_wired (self->network), wired_signals);
  update (self);
}

static void
unity_panel_network_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  UnityPanelNetwork *self = UNITY_PANEL_NETWORK (object);

  switch ((UnityPanelNetworkProperty) prop_id)
    {
    case PROP_ICON_NAME:
      g_value_set_string (value, self->icon_name);
      break;

    case PROP_SUBTITLE:
      g_value_set_string (value, self->subtitle);
      break;

    case PROP_AVAILABLE:
      g_value_set_boolean (value, self->available);
      break;

    case PROP_PRESENT:
      g_value_set_boolean (value, self->present);
      break;

    case PROP_HAS_WIFI:
      g_value_set_boolean (value, self->has_wifi);
      break;

    case PROP_WIFI_ENABLED:
      g_value_set_boolean (value, self->wifi_enabled);
      break;
    }
}

static void
unity_panel_network_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  UnityPanelNetwork *self = UNITY_PANEL_NETWORK (object);

  switch ((UnityPanelNetworkProperty) prop_id)
    {
    case PROP_WIFI_ENABLED:
      if (self->wifi != NULL && astal_network_wifi_get_enabled (self->wifi) != g_value_get_boolean (value))
        astal_network_wifi_set_enabled (self->wifi, g_value_get_boolean (value));
      break;

    case PROP_ICON_NAME:
    case PROP_SUBTITLE:
    case PROP_AVAILABLE:
    case PROP_PRESENT:
    case PROP_HAS_WIFI:
      break;
    }
}

static void
unity_panel_network_dispose (GObject *object)
{
  UnityPanelNetwork *self = UNITY_PANEL_NETWORK (object);

  if (self->wifi != NULL)
    g_signal_handlers_disconnect_by_data (self->wifi, self);
  if (self->wired != NULL)
    g_signal_handlers_disconnect_by_data (self->wired, self);
  g_clear_object (&self->wifi);
  g_clear_object (&self->wired);

  G_OBJECT_CLASS (unity_panel_network_parent_class)->dispose (object);
}

static void
unity_panel_network_finalize (GObject *object)
{
  UnityPanelNetwork *self = UNITY_PANEL_NETWORK (object);

  g_free (self->icon_name);
  g_free (self->subtitle);

  G_OBJECT_CLASS (unity_panel_network_parent_class)->finalize (object);
}

static void
unity_panel_network_class_init (UnityPanelNetworkClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose      = unity_panel_network_dispose;
  object_class->finalize     = unity_panel_network_finalize;
  object_class->get_property = unity_panel_network_get_property;
  object_class->set_property = unity_panel_network_set_property;

  properties[PROP_ICON_NAME]    = g_param_spec_string ("icon-name", NULL, NULL, OFFLINE_ICON,
                                                       G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_SUBTITLE]     = g_param_spec_string ("subtitle", NULL, NULL, "Off",
                                                       G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_AVAILABLE]    = g_param_spec_boolean ("available", NULL, NULL, FALSE,
                                                        G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_PRESENT]      = g_param_spec_boolean ("present", NULL, NULL, FALSE,
                                                        G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_HAS_WIFI]     = g_param_spec_boolean ("has-wifi", NULL, NULL, FALSE,
                                                        G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_WIFI_ENABLED] = g_param_spec_boolean ("wifi-enabled", NULL, NULL, FALSE,
                                                        G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, G_N_ELEMENTS (properties), properties);
}

static void
unity_panel_network_init (UnityPanelNetwork *self)
{
  const gchar *signals[] = { "notify::primary", "notify::wifi", "notify::wired" };

  self->icon_name = g_strdup (OFFLINE_ICON);
  self->subtitle  = g_strdup ("Off");
  self->network   = astal_network_get_default ();
  if (self->network == NULL)
    return;

  for (guint i = 0; i < G_N_ELEMENTS (signals); i++)
    g_signal_connect_object (self->network, signals[i], G_CALLBACK (devices_cb), self, G_CONNECT_SWAPPED);

  devices_cb (self);
}
