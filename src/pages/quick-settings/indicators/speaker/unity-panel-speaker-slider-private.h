/* unity-panel-speaker-slider-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_SPEAKER_SLIDER (unity_panel_speaker_slider_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelSpeakerSlider, unity_panel_speaker_slider, UNITY_PANEL, SPEAKER_SLIDER, AdwBin)

G_END_DECLS
