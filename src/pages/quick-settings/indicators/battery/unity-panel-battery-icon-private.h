/* unity-panel-battery-icon-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_BATTERY_ICON (unity_panel_battery_icon_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelBatteryIcon, unity_panel_battery_icon, UNITY_PANEL, BATTERY_ICON, AdwBin)

G_END_DECLS
