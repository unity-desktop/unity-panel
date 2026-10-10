/* unity-panel-sheet-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <unity-window.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_SHEET (unity_panel_sheet_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelSheet, unity_panel_sheet, UNITY_PANEL, SHEET, UnityWindow)

UnityPanelSheet *unity_panel_sheet_new      (GtkApplication  *app);

gboolean         unity_panel_sheet_get_open (UnityPanelSheet *self);

void             unity_panel_sheet_popup    (UnityPanelSheet *self,
                                             GdkMonitor      *monitor,
                                             gint             top,
                                             gdouble          x);

void             unity_panel_sheet_popdown  (UnityPanelSheet *self);

G_END_DECLS
