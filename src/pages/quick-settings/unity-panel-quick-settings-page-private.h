/* unity-panel-quick-settings-page-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_QUICK_SETTINGS_PAGE (unity_panel_quick_settings_page_get_type ())

G_DECLARE_FINAL_TYPE (UnityPanelQuickSettingsPage, unity_panel_quick_settings_page, UNITY_PANEL, QUICK_SETTINGS_PAGE, AdwBin)

G_END_DECLS
