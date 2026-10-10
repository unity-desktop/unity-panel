/* unity-panel.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <unity-strip.h>

G_BEGIN_DECLS

#define UNITY_TYPE_PANEL (unity_panel_get_type ())

/**
 * UnityPanel:
 *
 * The top panel of the Unity session: a strip along the top edge of one monitor,
 * with the spatial toggle at the start and the system indicator at the end.
 */
G_DECLARE_FINAL_TYPE (UnityPanel, unity_panel, UNITY, PANEL, UnityStrip)

/**
 * unity_panel_new:
 * @app: the #GtkApplication the panel belongs to
 *
 * Creates a panel. Set #UnityWindow:gdkmonitor to choose its monitor, or build it
 * from the factory of unity_window_present_for_each_monitor() to have one panel on
 * every monitor.
 *
 * Returns: (transfer none): a new #UnityPanel
 */
UnityPanel *unity_panel_new (GtkApplication *app);

G_END_DECLS
