/* unity-panel-network-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_NETWORK (unity_panel_network_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelNetwork, unity_panel_network, UNITY_PANEL, NETWORK, GObject)

G_END_DECLS
