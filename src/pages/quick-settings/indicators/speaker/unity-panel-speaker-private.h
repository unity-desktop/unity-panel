/* unity-panel-speaker-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_SPEAKER (unity_panel_speaker_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelSpeaker, unity_panel_speaker, UNITY_PANEL, SPEAKER, GObject)

G_END_DECLS
