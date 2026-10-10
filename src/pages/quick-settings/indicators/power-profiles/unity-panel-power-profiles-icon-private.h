/* unity-panel-power-profiles-icon-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_POWER_PROFILES_ICON (unity_panel_power_profiles_icon_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelPowerProfilesIcon, unity_panel_power_profiles_icon, UNITY_PANEL, POWER_PROFILES_ICON, AdwBin)

G_END_DECLS
