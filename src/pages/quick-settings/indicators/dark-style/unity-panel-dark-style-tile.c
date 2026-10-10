/* unity-panel-dark-style-tile.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-dark-style-tile-private.h"

#include <gdesktop-enums.h>

#define SCHEMA_ID "org.gnome.desktop.interface"

struct _UnityPanelDarkStyleTile
{
  GtkToggleButton parent_instance;

  GtkLabel  *subtitle;
  GSettings *settings;
};

G_DEFINE_FINAL_TYPE (UnityPanelDarkStyleTile, unity_panel_dark_style_tile, GTK_TYPE_TOGGLE_BUTTON)

static void
scheme_changed_cb (UnityPanelDarkStyleTile *self)
{
  GDesktopColorScheme scheme = g_settings_get_enum (self->settings, "color-scheme");

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (self), scheme == G_DESKTOP_COLOR_SCHEME_PREFER_DARK);
}

static void
toggled_cb (UnityPanelDarkStyleTile *self)
{
  gboolean            active = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self));
  GDesktopColorScheme scheme;

  gtk_label_set_label (self->subtitle, active ? "On" : "Off");

  if (self->settings == NULL)
    return;

  scheme = g_settings_get_enum (self->settings, "color-scheme");
  if ((scheme == G_DESKTOP_COLOR_SCHEME_PREFER_DARK) != active)
    g_settings_set_enum (self->settings, "color-scheme",
                         active ? G_DESKTOP_COLOR_SCHEME_PREFER_DARK : G_DESKTOP_COLOR_SCHEME_DEFAULT);
}

static void
unity_panel_dark_style_tile_dispose (GObject *object)
{
  UnityPanelDarkStyleTile *self = UNITY_PANEL_DARK_STYLE_TILE (object);

  g_clear_object (&self->settings);
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_DARK_STYLE_TILE);

  G_OBJECT_CLASS (unity_panel_dark_style_tile_parent_class)->dispose (object);
}

static void
unity_panel_dark_style_tile_class_init (UnityPanelDarkStyleTileClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_dark_style_tile_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-dark-style-tile.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelDarkStyleTile, subtitle);
  gtk_widget_class_bind_template_callback (widget_class, toggled_cb);
}

static void
unity_panel_dark_style_tile_init (UnityPanelDarkStyleTile *self)
{
  g_autoptr (GSettingsSchema) schema = g_settings_schema_source_lookup (g_settings_schema_source_get_default (), SCHEMA_ID, TRUE);

  gtk_widget_init_template (GTK_WIDGET (self));

  gtk_widget_set_visible (GTK_WIDGET (self), schema != NULL);
  if (schema == NULL)
    return;

  self->settings = g_settings_new (SCHEMA_ID);
  g_signal_connect_object (self->settings, "changed::color-scheme", G_CALLBACK (scheme_changed_cb), self, G_CONNECT_SWAPPED);

  scheme_changed_cb (self);
  toggled_cb (self);
}
