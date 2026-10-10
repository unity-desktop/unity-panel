/* unity-panel-bluetooth-icon-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_BLUETOOTH_ICON (unity_panel_bluetooth_icon_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelBluetoothIcon, unity_panel_bluetooth_icon, UNITY_PANEL, BLUETOOTH_ICON, AdwBin)

G_END_DECLS
