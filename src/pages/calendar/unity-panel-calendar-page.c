/* unity-panel-calendar-page.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-calendar-page-private.h"

#include "unity-panel-sheet-page-private.h"

struct _UnityPanelCalendarPage
{
  AdwBin parent_instance;

  AdwToggle   *toggle;
  GtkCalendar *calendar;
  GtkLabel    *date;
  GtkButton   *open;
  GSettings   *settings;
};

static void unity_panel_calendar_page_sheet_page_init (UnityPanelSheetPageInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (UnityPanelCalendarPage, unity_panel_calendar_page, ADW_TYPE_BIN,
                               G_IMPLEMENT_INTERFACE (UNITY_PANEL_TYPE_SHEET_PAGE, unity_panel_calendar_page_sheet_page_init))

static void
date_cb (UnityPanelCalendarPage *self)
{
  g_autoptr (GDateTime) date = gtk_calendar_get_date (self->calendar);
  g_autofree gchar *text     = g_date_time_format (date, "%A, %-d %B %Y");

  gtk_label_set_label (self->date, text);
}

static void
show_today (UnityPanelCalendarPage *self)
{
  g_autoptr (GDateTime) now = g_date_time_new_now_local ();

  gtk_calendar_set_date (self->calendar, now);
  date_cb (self);
}

static void
open_clicked_cb (UnityPanelCalendarPage *self)
{
  g_autoptr (GAppInfo) app                = g_app_info_get_default_for_type ("text/calendar", FALSE);
  g_autoptr (GdkAppLaunchContext) context = gdk_display_get_app_launch_context (gtk_widget_get_display (GTK_WIDGET (self)));
  g_autoptr (GError) error                = NULL;

  if (app != NULL && !g_app_info_launch (app, NULL, G_APP_LAUNCH_CONTEXT (context), &error))
    g_warning ("Could not open the calendar: %s", error->message);

  gtk_widget_activate_action (GTK_WIDGET (self), "sheet.close", NULL);
}

static AdwToggle *
unity_panel_calendar_page_get_toggle (UnityPanelSheetPage *page)
{
  UnityPanelCalendarPage *self = UNITY_PANEL_CALENDAR_PAGE (page);

  if (self->toggle == NULL)
    {
      g_autoptr (GtkBuilder) builder = gtk_builder_new_from_resource ("/org/unity/panel/unity-panel-calendar-toggle.ui");

      self->toggle = g_object_ref (ADW_TOGGLE (gtk_builder_get_object (builder, "toggle")));
    }

  return self->toggle;
}

static void
unity_panel_calendar_page_sheet_page_init (UnityPanelSheetPageInterface *iface)
{
  iface->get_toggle = unity_panel_calendar_page_get_toggle;
}

static void
unity_panel_calendar_page_unmap (GtkWidget *widget)
{
  GTK_WIDGET_CLASS (unity_panel_calendar_page_parent_class)->unmap (widget);

  show_today (UNITY_PANEL_CALENDAR_PAGE (widget));
}

static void
unity_panel_calendar_page_dispose (GObject *object)
{
  UnityPanelCalendarPage *self = UNITY_PANEL_CALENDAR_PAGE (object);

  g_clear_object (&self->toggle);
  g_clear_object (&self->settings);
  gtk_widget_dispose_template (GTK_WIDGET (object), UNITY_PANEL_TYPE_CALENDAR_PAGE);

  G_OBJECT_CLASS (unity_panel_calendar_page_parent_class)->dispose (object);
}

static void
unity_panel_calendar_page_class_init (UnityPanelCalendarPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = unity_panel_calendar_page_dispose;
  widget_class->unmap   = unity_panel_calendar_page_unmap;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/unity/panel/unity-panel-calendar-page.ui");
  gtk_widget_class_bind_template_child (widget_class, UnityPanelCalendarPage, calendar);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelCalendarPage, date);
  gtk_widget_class_bind_template_child (widget_class, UnityPanelCalendarPage, open);
  gtk_widget_class_bind_template_callback (widget_class, date_cb);
  gtk_widget_class_bind_template_callback (widget_class, open_clicked_cb);
  gtk_widget_class_set_css_name (widget_class, "unity-panel-calendar-page");
}

static void
unity_panel_calendar_page_init (UnityPanelCalendarPage *self)
{
  g_autoptr (GAppInfo) app = g_app_info_get_default_for_type ("text/calendar", FALSE);

  gtk_widget_init_template (GTK_WIDGET (self));

  self->settings = g_settings_new ("org.gnome.desktop.calendar");
  g_settings_bind (self->settings, "show-weekdate", self->calendar, "show-week-numbers", G_SETTINGS_BIND_GET);
  gtk_widget_set_visible (GTK_WIDGET (self->open), app != NULL);

  show_today (self);
}
