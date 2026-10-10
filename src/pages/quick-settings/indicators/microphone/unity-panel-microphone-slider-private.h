/* unity-panel-microphone-slider-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_MICROPHONE_SLIDER (unity_panel_microphone_slider_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelMicrophoneSlider, unity_panel_microphone_slider, UNITY_PANEL, MICROPHONE_SLIDER, AdwBin)

G_END_DECLS
