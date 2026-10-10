/* unity-panel-speaker-slider.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-speaker-slider-private.h"

#include "unity-panel-speaker-private.h"

struct _UnityPanelSpeakerSlider
{
  AdwBin parent_instance;

  GtkButton         *button;
  UnityPanelSpeaker *model;
};

G_DEFINE_FINAL_TYPE (UnityPanelSpeakerSlider, unity_panel_speaker_slider, ADW_TYPE_BIN)

static void
update (UnityPanelSpeakerSlider *self)
{
  gboolean present = FALSE;
  gboolean mute    = FALSE;

  g_object_get (self->model, "present", &present, "mute", &mute, NULL);

  gtk_widget_set_tooltip_text (GTK_WIDGET (self->button), mute ? "Unmute" : "Mute");
  gtk_widget_set_visible (GTK_WIDGET (self), present);
}

static void
mute_clicked_cb (UnityPanelSpeakerSlider *self)
{
  gboolean mute = FALSE;

  g_object_get (self->model, "mute", &mute, NULL);
  g_object_set (self->model, "mute", !mute, NULL);
}

static void
unity_panel_speaker_slider_dispose (GObject *object)
{
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_SPEAKER_SLIDER);

  G_OBJECT_CLASS (unity_panel_speaker_slider_parent_class)->dispose (object);
}

static void
unity_panel_speaker_slider_class_init (UnityPanelSpeakerSliderClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_speaker_slider_dispose;

  g_type_ensure (UNITY_PANEL_TYPE_SPEAKER);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-speaker-slider.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSpeakerSlider, button);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelSpeakerSlider, model);
  gtk_widget_class_bind_template_callback (widget_class, mute_clicked_cb);
}

static void
unity_panel_speaker_slider_init (UnityPanelSpeakerSlider *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  g_signal_connect_object (self->model, "notify", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
  update (self);
}
