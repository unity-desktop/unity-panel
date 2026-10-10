/* unity-panel-network-icon.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-network-icon-private.h"

#include "unity-panel-network-private.h"

struct _UnityPanelNetworkIcon
{
  AdwBin parent_instance;

  UnityPanelNetwork *network;
};

G_DEFINE_FINAL_TYPE (UnityPanelNetworkIcon, unity_panel_network_icon, ADW_TYPE_BIN)

static void
unity_panel_network_icon_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_NETWORK_ICON);

  G_OBJECT_CLASS (unity_panel_network_icon_parent_class)->dispose (object);
}

static void
unity_panel_network_icon_class_init (UnityPanelNetworkIconClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_network_icon_dispose;

  g_type_ensure (UNITY_PANEL_TYPE_NETWORK);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-network-icon.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelNetworkIcon, network);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-network-icon");
}

static void
unity_panel_network_icon_init (UnityPanelNetworkIcon *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
