/* unity-panel-dark-style-tile-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_DARK_STYLE_TILE (unity_panel_dark_style_tile_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelDarkStyleTile, unity_panel_dark_style_tile, UNITY_PANEL, DARK_STYLE_TILE, GtkToggleButton)

G_END_DECLS
