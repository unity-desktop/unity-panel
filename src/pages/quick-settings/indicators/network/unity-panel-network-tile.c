/* unity-panel-network-tile.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-network-tile-private.h"

#include "unity-panel-network-private.h"

struct _UnityPanelNetworkTile
{
  GtkToggleButton parent_instance;

  UnityPanelNetwork *network;
};

G_DEFINE_FINAL_TYPE (UnityPanelNetworkTile, unity_panel_network_tile, GTK_TYPE_TOGGLE_BUTTON)

static void
unity_panel_network_tile_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_NETWORK_TILE);

  G_OBJECT_CLASS (unity_panel_network_tile_parent_class)->dispose (object);
}

static void
unity_panel_network_tile_class_init (UnityPanelNetworkTileClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_network_tile_dispose;

  g_type_ensure (UNITY_PANEL_TYPE_NETWORK);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-network-tile.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelNetworkTile, network);
}

static void
unity_panel_network_tile_init (UnityPanelNetworkTile *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
