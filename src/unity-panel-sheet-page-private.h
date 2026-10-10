/* unity-panel-sheet-page-private.h
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define UNITY_PANEL_TYPE_SHEET_PAGE (unity_panel_sheet_page_get_type ())

G_DECLARE_INTERFACE (UnityPanelSheetPage, unity_panel_sheet_page, UNITY_PANEL, SHEET_PAGE, GtkWidget)

struct _UnityPanelSheetPageInterface
{
  GTypeInterface parent_iface;

  AdwToggle *(*get_toggle) (UnityPanelSheetPage *self);
};

AdwToggle *unity_panel_sheet_page_get_toggle (UnityPanelSheetPage *self);

G_END_DECLS
