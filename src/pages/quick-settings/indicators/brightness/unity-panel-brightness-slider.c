/* unity-panel-brightness-slider.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-brightness-slider-private.h"

#include <astal-brightness.h>

#define EPSILON 0.001

struct _UnityPanelBrightnessSlider
{
  AdwBin parent_instance;

  GtkAdjustment             *adjustment;
  AstalBrightnessBrightness *brightness;
  AstalBrightnessDevice     *screen;
};

G_DEFINE_FINAL_TYPE (UnityPanelBrightnessSlider, unity_panel_brightness_slider, ADW_TYPE_BIN)

static void
update (UnityPanelBrightnessSlider *self)
{
  gdouble value;

  if (self->screen == NULL)
    {
      gtk_widget_set_visible (GTK_WIDGET (self), FALSE);
      return;
    }

  value = astal_brightness_device_get_brightness (self->screen);
  if (ABS (gtk_adjustment_get_value (self->adjustment) - value) > EPSILON)
    gtk_adjustment_set_value (self->adjustment, value);
  gtk_widget_set_visible (GTK_WIDGET (self), TRUE);
}

static void
screen_cb (UnityPanelBrightnessSlider *self)
{
  AstalBrightnessDevice *screen = NULL;

  if (self->brightness != NULL)
    screen = astal_brightness_brightness_get_screen (self->brightness);

  if (screen != self->screen)
    {
      if (self->screen != NULL)
        g_signal_handlers_disconnect_by_data (self->screen, self);
      g_set_object (&self->screen, screen);

      if (screen != NULL)
        g_signal_connect_object (screen, "notify::brightness", G_CALLBACK (update), self, G_CONNECT_SWAPPED);
    }

  update (self);
}

static void
value_changed_cb (UnityPanelBrightnessSlider *self)
{
  gdouble value = gtk_adjustment_get_value (self->adjustment);

  if (self->screen != NULL && ABS (astal_brightness_device_get_brightness (self->screen) - value) > EPSILON)
    astal_brightness_device_set_brightness (self->screen, value);
}

static void
unity_panel_brightness_slider_dispose (GObject *object)
{
  UnityPanelBrightnessSlider *self = UNITY_PANEL_BRIGHTNESS_SLIDER (object);

  if (self->screen != NULL)
    g_signal_handlers_disconnect_by_data (self->screen, self);
  g_clear_object (&self->screen);
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_BRIGHTNESS_SLIDER);

  G_OBJECT_CLASS (unity_panel_brightness_slider_parent_class)->dispose (object);
}

static void
unity_panel_brightness_slider_class_init (UnityPanelBrightnessSliderClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_brightness_slider_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-brightness-slider.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelBrightnessSlider, adjustment);
  gtk_widget_class_bind_template_callback (widget_class, value_changed_cb);
}

static void
unity_panel_brightness_slider_init (UnityPanelBrightnessSlider *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->brightness = astal_brightness_get_default ();
  if (self->brightness != NULL)
    g_signal_connect_object (self->brightness, "notify::screen", G_CALLBACK (screen_cb), self, G_CONNECT_SWAPPED);

  screen_cb (self);
}
