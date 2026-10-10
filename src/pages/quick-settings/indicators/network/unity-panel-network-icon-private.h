/* unity-panel-network-icon-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_NETWORK_ICON (unity_panel_network_icon_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelNetworkIcon, unity_panel_network_icon, UNITY_PANEL, NETWORK_ICON, AdwBin)

G_END_DECLS
