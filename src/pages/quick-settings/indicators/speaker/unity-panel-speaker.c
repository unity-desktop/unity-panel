/* unity-panel-speaker.c
 *
 * Copyright 2026 Muqtadir
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "unity-panel-speaker-private.h"

#include <astal-wp.h>

#define EPSILON 0.001

struct _UnityPanelSpeaker
{
  GObject parent_instance;

  AstalWpWp       *wp;
  AstalWpEndpoint *endpoint;

  gchar           *icon_name;
  gdouble          volume;
  gboolean         mute;
  gboolean         present;
};

G_DEFINE_FINAL_TYPE (UnityPanelSpeaker, unity_panel_speaker, G_TYPE_OBJECT)

typedef enum
{
  PROP_ICON_NAME = 1,
  PROP_VOLUME,
  PROP_MUTE,
  PROP_PRESENT,
} UnityPanelSpeakerProperty;

static GParamSpec *properties[PROP_PRESENT + 1];

static void
set_boolean (UnityPanelSpeaker *self,
             gboolean          *field,
             gboolean           value,
             guint              prop)
{
  if (*field == value)
    return;

  *field = value;
  g_object_notify_by_pspec (G_OBJECT (self), properties[prop]);
}

static void
update (UnityPanelSpeaker *self)
{
  AstalWpNode *node = self->endpoint != NULL ? ASTAL_WP_NODE (self->endpoint) : NULL;

  if (node != NULL && g_set_str (&self->icon_name, astal_wp_node_get_volume_icon (node)))
    g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_ICON_NAME]);

  if (node != NULL && ABS (self->volume - astal_wp_node_get_volume (node)) > EPSILON)
    {
      self->volume = astal_wp_node_get_volume (node);
      g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_VOLUME]);
    }

  set_boolean (self, &self->mute, node != NULL && astal_wp_node_get_mute (node), PROP_MUTE);
  set_boolean (self, &self->present, node != NULL, PROP_PRESENT);
}

static void
endpoint_cb (UnityPanelSpeaker *self)
{
  const gchar     *signals[] = { "notify::volume", "notify::mute", "notify::volume-icon" };
  AstalWpEndpoint *endpoint  = astal_wp_wp_get_default_speaker (self->wp);

  if (endpoint != self->endpoint)
    {
      if (self->endpoint != NULL)
        g_signal_handlers_disconnect_by_data (self->endpoint, self);
      g_set_object (&self->endpoint, endpoint);

      for (guint i = 0; endpoint != NULL && i < G_N_ELEMENTS (signals); i++)
        g_signal_connect_object (endpoint, signals[i], G_CALLBACK (update), self, G_CONNECT_SWAPPED);
    }

  update (self);
}

static void
unity_panel_speaker_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  UnityPanelSpeaker *self = UNITY_PANEL_SPEAKER (object);

  switch ((UnityPanelSpeakerProperty) prop_id)
    {
    case PROP_ICON_NAME:
      g_value_set_string (value, self->icon_name);
      break;

    case PROP_VOLUME:
      g_value_set_double (value, self->volume);
      break;

    case PROP_MUTE:
      g_value_set_boolean (value, self->mute);
      break;

    case PROP_PRESENT:
      g_value_set_boolean (value, self->present);
      break;
    }
}

static void
unity_panel_speaker_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  UnityPanelSpeaker *self = UNITY_PANEL_SPEAKER (object);

  switch ((UnityPanelSpeakerProperty) prop_id)
    {
    case PROP_VOLUME:
      if (self->endpoint != NULL && ABS (astal_wp_node_get_volume (ASTAL_WP_NODE (self->endpoint)) - g_value_get_double (value)) > EPSILON)
        astal_wp_node_set_volume (ASTAL_WP_NODE (self->endpoint), g_value_get_double (value));
      break;

    case PROP_MUTE:
      if (self->endpoint != NULL && astal_wp_node_get_mute (ASTAL_WP_NODE (self->endpoint)) != g_value_get_boolean (value))
        astal_wp_node_set_mute (ASTAL_WP_NODE (self->endpoint), g_value_get_boolean (value));
      break;

    case PROP_ICON_NAME:
    case PROP_PRESENT:
      break;
    }
}

static void
unity_panel_speaker_dispose (GObject *object)
{
  UnityPanelSpeaker *self = UNITY_PANEL_SPEAKER (object);

  if (self->endpoint != NULL)
    g_signal_handlers_disconnect_by_data (self->endpoint, self);
  g_clear_object (&self->endpoint);

  G_OBJECT_CLASS (unity_panel_speaker_parent_class)->dispose (object);
}

static void
unity_panel_speaker_finalize (GObject *object)
{
  UnityPanelSpeaker *self = UNITY_PANEL_SPEAKER (object);

  g_free (self->icon_name);

  G_OBJECT_CLASS (unity_panel_speaker_parent_class)->finalize (object);
}

static void
unity_panel_speaker_class_init (UnityPanelSpeakerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose      = unity_panel_speaker_dispose;
  object_class->finalize     = unity_panel_speaker_finalize;
  object_class->get_property = unity_panel_speaker_get_property;
  object_class->set_property = unity_panel_speaker_set_property;

  properties[PROP_ICON_NAME] = g_param_spec_string ("icon-name", NULL, NULL, NULL,
                                                    G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_VOLUME]    = g_param_spec_double ("volume", NULL, NULL, 0.0, G_MAXDOUBLE, 0.0,
                                                    G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_MUTE]      = g_param_spec_boolean ("mute", NULL, NULL, FALSE,
                                                     G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  properties[PROP_PRESENT]   = g_param_spec_boolean ("present", NULL, NULL, FALSE,
                                                     G_PARAM_READABLE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  g_object_class_install_properties (object_class, G_N_ELEMENTS (properties), properties);
}

static void
unity_panel_speaker_init (UnityPanelSpeaker *self)
{
  self->wp = astal_wp_get_default ();
  if (self->wp == NULL)
    return;

  g_signal_connect_object (self->wp, "notify::default-speaker", G_CALLBACK (endpoint_cb), self, G_CONNECT_SWAPPED);

  endpoint_cb (self);
}
