/* unity-panel-brightness-slider-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_BRIGHTNESS_SLIDER (unity_panel_brightness_slider_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelBrightnessSlider, unity_panel_brightness_slider, UNITY_PANEL, BRIGHTNESS_SLIDER, AdwBin)

G_END_DECLS
