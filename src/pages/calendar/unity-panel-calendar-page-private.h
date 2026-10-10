/* unity-panel-calendar-page-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_CALENDAR_PAGE (unity_panel_calendar_page_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelCalendarPage, unity_panel_calendar_page, UNITY_PANEL, CALENDAR_PAGE, AdwBin)

G_END_DECLS
