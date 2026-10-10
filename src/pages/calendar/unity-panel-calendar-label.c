/* unity-panel-calendar-label.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-calendar-label-private.h"

#include <libgnome-desktop/gnome-wall-clock.h>

struct _UnityPanelCalendarLabel
{
  AdwBin parent_instance;

  GtkLabel       *label;
  GnomeWallClock *clock;
};

G_DEFINE_FINAL_TYPE (UnityPanelCalendarLabel, unity_panel_calendar_label, ADW_TYPE_BIN)

static void
unity_panel_calendar_label_dispose (GObject *object)
{
  UnityPanelCalendarLabel *self = UNITY_PANEL_CALENDAR_LABEL (object);

  g_clear_object (&self->clock);
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_CALENDAR_LABEL);

  G_OBJECT_CLASS (unity_panel_calendar_label_parent_class)->dispose (object);
}

static void
unity_panel_calendar_label_class_init (UnityPanelCalendarLabelClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_calendar_label_dispose;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-calendar-label.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelCalendarLabel, label);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-calendar-label");
}

static void
unity_panel_calendar_label_init (UnityPanelCalendarLabel *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->clock = gnome_wall_clock_new ();
  g_object_bind_property (self->clock, "clock", self->label, "label", G_BINDING_SYNC_CREATE);
}
