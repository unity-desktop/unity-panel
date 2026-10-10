/* unity-panel-bluetooth-tile-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_BLUETOOTH_TILE (unity_panel_bluetooth_tile_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelBluetoothTile, unity_panel_bluetooth_tile, UNITY_PANEL, BLUETOOTH_TILE, GtkToggleButton)

G_END_DECLS
