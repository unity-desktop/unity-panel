/* unity-panel-microphone-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_MICROPHONE (unity_panel_microphone_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelMicrophone, unity_panel_microphone, UNITY_PANEL, MICROPHONE, GObject)

G_END_DECLS
