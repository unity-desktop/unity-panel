/* unity-panel-power-profiles-icon.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-power-profiles-icon-private.h"

#include <astal-power-profiles.h>

struct _UnityPanelPowerProfilesIcon
{
  AdwBin parent_instance;

  GtkImage                        *image;
  AstalPowerProfilesPowerProfiles *power;
};

G_DEFINE_FINAL_TYPE (UnityPanelPowerProfilesIcon, unity_panel_power_profiles_icon, ADW_TYPE_BIN)

static void
update (UnityPanelPowerProfilesIcon *self)
{
  g_autofree gchar *active = NULL;
  g_autofree gchar *icon   = NULL;

  if (self->power == NULL)
    {
      gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
      return;
    }

  active = astal_power_profiles_power_profiles_get_active_profile (self->power);
  icon   = astal_power_profiles_power_profiles_get_icon_name (self->power);

  gtk_image_set_from_icon_name (self->image, icon);
  gtk_widget_set_visible (GTK_WIDGET (self), active != NULL && *active != '\0' && g_strcmp0 (active, "balanced") != 0);
}

static void
unity_panel_power_profiles_icon_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_POWER_PROFILES_ICON);

  G_OBJECT_CLASS (unity_panel_power_profiles_icon_parent_class)->dispose (object);
}

static void
unity_panel_power_profiles_icon_class_init (UnityPanelPowerProfilesIconClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_power_profiles_icon_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-power-profiles-icon.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelPowerProfilesIcon, image);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-power-profiles-icon");
}

static void
unity_panel_power_profiles_icon_init (UnityPanelPowerProfilesIcon *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->power = astal_power_profiles_get_default ();
  if (self->power != NULL)
    {
      g_signal_connect_object (self->power, "notify::active-profile", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
      g_signal_connect_object (self->power, "notify::icon-name", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
    }

  update (self);
}
