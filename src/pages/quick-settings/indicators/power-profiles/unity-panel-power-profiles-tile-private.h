/* unity-panel-power-profiles-tile-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_POWER_PROFILES_TILE (unity_panel_power_profiles_tile_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelPowerProfilesTile, unity_panel_power_profiles_tile, UNITY_PANEL, POWER_PROFILES_TILE, GtkToggleButton)

G_END_DECLS
