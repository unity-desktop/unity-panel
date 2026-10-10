/* unity-panel-calendar-label-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_CALENDAR_LABEL (unity_panel_calendar_label_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelCalendarLabel, unity_panel_calendar_label, UNITY_PANEL, CALENDAR_LABEL, AdwBin)

G_END_DECLS
