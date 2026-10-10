/* unity-panel-network-tile-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_NETWORK_TILE (unity_panel_network_tile_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelNetworkTile, unity_panel_network_tile, UNITY_PANEL, NETWORK_TILE, GtkToggleButton)

G_END_DECLS
