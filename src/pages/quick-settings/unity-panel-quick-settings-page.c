/* unity-panel-quick-settings-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-quick-settings-page-private.h"

#include "unity-panel-sheet-page-private.h"

struct _UnityPanelQuickSettingsPage
{
  AdwBin parent_instance;

  AdwToggle *toggle;
};

static void unity_panel_quick_settings_page_sheet_page_init (UnityPanelSheetPageInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (UnityPanelQuickSettingsPage, unity_panel_quick_settings_page, ADW_TYPE_BIN,
                               G_IMPLEMENT_INTERFACE (UNITY_PANEL_TYPE_SHEET_PAGE, unity_panel_quick_settings_page_sheet_page_init))

static AdwToggle *
unity_panel_quick_settings_page_get_toggle (UnityPanelSheetPage *page)
{
  UnityPanelQuickSettingsPage *self = UNITY_PANEL_QUICK_SETTINGS_PAGE (page);

  if (self->toggle == NULL)
    {
      g_autoptr (GtkBuilder) builder = gtk_builder_new_from_resource ("/org/unity/panel/unity-panel-quick-settings-toggle.ui");

      self->toggle = g_object_ref (ADW_TOGGLE (gtk_builder_get_object (builder, "toggle")));
    }

  return self->toggle;
}

static void
unity_panel_quick_settings_page_sheet_page_init (UnityPanelSheetPageInterface *iface)
{
  iface->get_toggle = unity_panel_quick_settings_page_get_toggle;
}

static void
unity_panel_quick_settings_page_dispose (GObject *object)
{
  UnityPanelQuickSettingsPage *self = UNITY_PANEL_QUICK_SETTINGS_PAGE (object);

  g_clear_object (&self->toggle);
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_QUICK_SETTINGS_PAGE);

  G_OBJECT_CLASS (unity_panel_quick_settings_page_parent_class)->dispose (object);
}

static void
unity_panel_quick_settings_page_class_init (UnityPanelQuickSettingsPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_quick_settings_page_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-quick-settings-page.ui");
  gtk_widget_class_set_css_name (widget_class, "unity-panel-quick-settings-page");
}

static void
unity_panel_quick_settings_page_init (UnityPanelQuickSettingsPage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
