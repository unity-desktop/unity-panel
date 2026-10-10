/* unity-panel-speaker-icon-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_SPEAKER_ICON (unity_panel_speaker_icon_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelSpeakerIcon, unity_panel_speaker_icon, UNITY_PANEL, SPEAKER_ICON, AdwBin)

G_END_DECLS
