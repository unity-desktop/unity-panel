/* unity-panel-speaker-icon.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-speaker-icon-private.h"

#include "unity-panel-speaker-private.h"

struct _UnityPanelSpeakerIcon
{
  AdwBin parent_instance;

  UnityPanelSpeaker *model;
};

G_DEFINE_FINAL_TYPE (UnityPanelSpeakerIcon, unity_panel_speaker_icon, ADW_TYPE_BIN)

static void
unity_panel_speaker_icon_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_SPEAKER_ICON);

  G_OBJECT_CLASS (unity_panel_speaker_icon_parent_class)->dispose (object);
}

static void
unity_panel_speaker_icon_class_init (UnityPanelSpeakerIconClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_speaker_icon_dispose;

  g_type_ensure (UNITY_PANEL_TYPE_SPEAKER);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-speaker-icon.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSpeakerIcon, model);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-speaker-icon");
}

static void
unity_panel_speaker_icon_init (UnityPanelSpeakerIcon *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
