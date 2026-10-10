/* unity-panel-microphone-icon-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_MICROPHONE_ICON (unity_panel_microphone_icon_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelMicrophoneIcon, unity_panel_microphone_icon, UNITY_PANEL, MICROPHONE_ICON, AdwBin)

G_END_DECLS
