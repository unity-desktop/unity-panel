/* unity-panel-sheet-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-sheet-page-private.h"

G_DEFINE_INTERFACE (UnityPanelSheetPage, unity_panel_sheet_page, GTK_TYPE_WIDGET)

static void
unity_panel_sheet_page_default_init (UnityPanelSheetPageInterface *iface)
{
}

AdwToggle *
unity_panel_sheet_page_get_toggle (UnityPanelSheetPage *self)
{
  g_return_val_if_fail (UNITY_PANEL_IS_SHEET_PAGE (self), NULL);

  return UNITY_PANEL_SHEET_PAGE_GET_IFACE (self)->get_toggle (self);
}
