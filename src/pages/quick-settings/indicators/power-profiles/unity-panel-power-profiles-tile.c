/* unity-panel-power-profiles-tile.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-power-profiles-tile-private.h"

#include <astal-power-profiles.h>

#define BALANCED "balanced"

struct _UnityPanelPowerProfilesTile
{
  GtkToggleButton parent_instance;

  GtkImage                        *icon;
  GtkLabel                        *title;
  GtkLabel                        *subtitle;
  AstalPowerProfilesPowerProfiles *power;
  gchar                           *profile;
};

G_DEFINE_FINAL_TYPE (UnityPanelPowerProfilesTile, unity_panel_power_profiles_tile, GTK_TYPE_TOGGLE_BUTTON)

typedef enum
{
  PROP_PROFILE = 1,
} UnityPanelPowerProfilesTileProperty;

static GParamSpec *properties[PROP_PROFILE + 1];

static gboolean
has_profile (UnityPanelPowerProfilesTile *self)
{
  AstalPowerProfilesProfile *profiles;
  gint                       n     = 0;
  gboolean                   found = FALSE;

  profiles = astal_power_profiles_power_profiles_get_profiles (self->power, &n);
  for (gint i = 0; i < n; i++)
    {
      found = found || g_strcmp0 (profiles[i].profile, self->profile) == 0;
      astal_power_profiles_profile_destroy (&profiles[i]);
    }
  g_free (profiles);

  return found;
}

static const gchar *
profile_title (const gchar *profile)
{
  if (g_strcmp0 (profile, "power-saver") == 0)
    return "Power Saver";
  if (g_strcmp0 (profile, "performance") == 0)
    return "Performance";

  return "Balanced";
}

static void
update (UnityPanelPowerProfilesTile *self)
{
  g_autofree gchar *active = NULL;
  g_autofree gchar *icon   = NULL;

  if (self->power == NULL || self->profile == NULL)
    {
      gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
      return;
    }

  active = astal_power_profiles_power_profiles_get_active_profile (self->power);
  icon   = g_strdup_printf ("power-profile-%s-symbolic", self->profile);

  gtk_image_set_from_icon_name (self->icon, icon);
  gtk_label_set_label (self->title, profile_title (self->profile));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (self), g_strcmp0 (active, self->profile) == 0);
  gtk_label_set_label (self->subtitle, gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self)) ? "On" : "Off");
  gtk_widget_set_visible (GTK_WIDGET (self), has_profile (self));
}

static void
toggled_cb (UnityPanelPowerProfilesTile *self)
{
  g_autofree gchar *active = NULL;
  const gchar      *wanted;

  if (self->power == NULL || self->profile == NULL)
    return;

  active = astal_power_profiles_power_profiles_get_active_profile (self->power);
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self)))
    wanted = self->profile;
  else if (g_strcmp0 (active, self->profile) == 0)
    wanted = BALANCED;
  else
    return;

  if (g_strcmp0 (active, wanted) != 0)
    astal_power_profiles_power_profiles_set_active_profile (self->power, wanted);
}

static void
unity_panel_power_profiles_tile_get_property (GObject    *object,
                                              guint       prop_id,
                                              GValue     *value,
                                              GParamSpec *pspec)
{
  UnityPanelPowerProfilesTile *self = UNITY_PANEL_POWER_PROFILES_TILE (object);

  switch ((UnityPanelPowerProfilesTileProperty) prop_id)
    {
    case PROP_PROFILE:
      g_value_set_string (value, self->profile);
      break;
    }
}

static void
unity_panel_power_profiles_tile_set_property (GObject      *object,
                                              guint         prop_id,
                                              const GValue *value,
                                              GParamSpec   *pspec)
{
  UnityPanelPowerProfilesTile *self = UNITY_PANEL_POWER_PROFILES_TILE (object);

  switch ((UnityPanelPowerProfilesTileProperty) prop_id)
    {
    case PROP_PROFILE:
      if (g_set_str (&self->profile, g_value_get_string (value)))
        {
          update (self);
          g_object_notify_by_pspec (object, pspec);
        }
      break;
    }
}

static void
unity_panel_power_profiles_tile_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_POWER_PROFILES_TILE);

  G_OBJECT_CLASS (unity_panel_power_profiles_tile_parent_class)->dispose (object);
}

static void
unity_panel_power_profiles_tile_finalize (GObject *object)
{
  UnityPanelPowerProfilesTile *self = UNITY_PANEL_POWER_PROFILES_TILE (object);

  g_free (self->profile);

  G_OBJECT_CLASS (unity_panel_power_profiles_tile_parent_class)->finalize (object);
}

static void
unity_panel_power_profiles_tile_class_init (UnityPanelPowerProfilesTileClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose      = unity_panel_power_profiles_tile_dispose;
  object_class->finalize     = unity_panel_power_profiles_tile_finalize;
  object_class->get_property = unity_panel_power_profiles_tile_get_property;
  object_class->set_property = unity_panel_power_profiles_tile_set_property;

  properties[PROP_PROFILE] = g_param_spec_string ("profile", NULL, NULL, NULL,
                                                  G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, G_N_ELEMENTS (properties), properties);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-power-profiles-tile.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelPowerProfilesTile, icon);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelPowerProfilesTile, title);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelPowerProfilesTile, subtitle);
  gtk_widget_class_bind_template_callback (widget_class, toggled_cb);
}

static void
unity_panel_power_profiles_tile_init (UnityPanelPowerProfilesTile *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->power = astal_power_profiles_get_default ();
  if (self->power != NULL)
    g_signal_connect_object (self->power, "notify::active-profile", G_CALLBACK (update), self, G_CONNECT_SWAPPED);

  update (self);
}
