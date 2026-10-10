/* unity-panel-translucency-tile.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-translucency-tile-private.h"

#define SCHEMA_ID "org.unity.stylesheets"

struct _UnityPanelTranslucencyTile
{
  GtkToggleButton parent_instance;

  GtkLabel  *subtitle;
  GSettings *settings;
};

G_DEFINE_FINAL_TYPE (UnityPanelTranslucencyTile, unity_panel_translucency_tile, GTK_TYPE_TOGGLE_BUTTON)

static void
toggled_cb (UnityPanelTranslucencyTile *self)
{
  gtk_label_set_label (self->subtitle, gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self)) ? "On" : "Off");
}

static void
unity_panel_translucency_tile_dispose (GObject *object)
{
  UnityPanelTranslucencyTile *self = UNITY_PANEL_TRANSLUCENCY_TILE (object);

  g_clear_object (&self->settings);
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_TRANSLUCENCY_TILE);

  G_OBJECT_CLASS (unity_panel_translucency_tile_parent_class)->dispose (object);
}

static void
unity_panel_translucency_tile_class_init (UnityPanelTranslucencyTileClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_translucency_tile_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-translucency-tile.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelTranslucencyTile, subtitle);
  gtk_widget_class_bind_template_callback (widget_class, toggled_cb);
}

static void
unity_panel_translucency_tile_init (UnityPanelTranslucencyTile *self)
{
  g_autoptr (GSettingsSchema) schema = g_settings_schema_source_lookup (g_settings_schema_source_get_default (), SCHEMA_ID, TRUE);

  gtk_widget_init_template (GTK_WIDGET (self));

  gtk_widget_set_visible (GTK_WIDGET (self), schema != NULL);
  if (schema == NULL)
    return;

  self->settings = g_settings_new (SCHEMA_ID);
  g_settings_bind (self->settings, "translucency", self, "active", G_SETTINGS_BIND_DEFAULT);

  toggled_cb (self);
}
