/* unity-panel-microphone-icon.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-microphone-icon-private.h"

#include "unity-panel-microphone-private.h"

struct _UnityPanelMicrophoneIcon
{
  AdwBin parent_instance;

  UnityPanelMicrophone *model;
};

G_DEFINE_FINAL_TYPE (UnityPanelMicrophoneIcon, unity_panel_microphone_icon, ADW_TYPE_BIN)

static void
update (UnityPanelMicrophoneIcon *self)
{
  gboolean present = FALSE;
  gboolean in_use  = FALSE;
  gboolean mute    = FALSE;

  g_object_get (self->model, "present", &present, "in-use", &in_use, "mute", &mute, NULL);

  gtk_widget_set_visible (GTK_WIDGET (self), present && (mute || in_use));
}

static void
unity_panel_microphone_icon_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_MICROPHONE_ICON);

  G_OBJECT_CLASS (unity_panel_microphone_icon_parent_class)->dispose (object);
}

static void
unity_panel_microphone_icon_class_init (UnityPanelMicrophoneIconClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_microphone_icon_dispose;

  g_type_ensure (UNITY_PANEL_TYPE_MICROPHONE);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-microphone-icon.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelMicrophoneIcon, model);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-microphone-icon");
}

static void
unity_panel_microphone_icon_init (UnityPanelMicrophoneIcon *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  g_signal_connect_object (self->model, "notify", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
  update (self);
}
