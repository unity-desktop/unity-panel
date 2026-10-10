/* unity-panel-translucency-tile-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_TRANSLUCENCY_TILE (unity_panel_translucency_tile_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelTranslucencyTile, unity_panel_translucency_tile, UNITY_PANEL, TRANSLUCENCY_TILE, GtkToggleButton)

G_END_DECLS
