<?php

/**
 * @file
 * Items used for MailChimp campaigns.
 */

// -----------------------------------------------------------------------------
// Constants

define('EMAILMONKEY_BASIC_CAMPAIGN_FIELDS', array(
  'id', 'type', 'archive_url', 'status', 'recipients.list_id',
  'recipients.list_name', 'settings.title', 'settings.subject_line',
  'settings.from_name', 'settings.reply_to', 'recipients.segment_text',
  'recipients.segment_opts.saved_segment_id', 'recipients.recipient_count'));

// -----------------------------------------------------------------------------
// Functions

function emailmonkey_campaigns_create_basic_campaign($list_id, $segment_id, $subject_line, $from_name, $reply_to, $content, $campaign_name=NULL) {
  $campaign_type = Mailchimp\MailchimpCampaigns::CAMPAIGN_TYPE_REGULAR;
  $recipients['list_id'] = $list_id;
  $settings['subject_line'] = $subject_line;
  $settings['inline_css'] = TRUE;
  $settings['from_name'] = $from_name;
  $settings['reply_to'] = $reply_to;

  if (!empty($segment_id)) {
    $recipients['segment_opts']['saved_segment_id'] = intval($segment_id);
  }

  if (!empty($campaign_name)) {
    $settings['title'] = $campaign_name;
  }

  $campaign = emailmonkey_api('campaigns', 'addCampaign', $campaign_type, $recipients, $settings);

  try {
    emailmonkey_campaigns_set_campaign_content($campaign->id, $content);
  }
  catch (EmailMonkey_Exception $e) {
    try {
      emailmonkey_api('campaigns', 'delete', $campaign->id);
    }
    catch (EmailMonkey_Exception $e) {
      throw new EmailMonkey_Exception(t('Failed to delete the partially created campaign %name. It should be deleted from the MailChimp website.', array('%name' => $subject_line)), $e);
    }

    throw $e;
  }

  return $campaign;
}

function emailmonkey_campaigns_update_basic_campaign($campaign, $list, $values) {
  $valid_keys = array('list_id', 'saved_segment_id', 'subject_line', 'from_name', 'reply_to');
  $recipients = array();
  $settings = array();

  if (!empty($values['list_id'])) {
    $recipients['list_id'] = $values['list_id'];
  }
  else {
    $recipients['list_id'] = $campaign->recipients->list_id;
  }

  if (!empty($values['saved_segment_id'])) {
    $recipients['segment_opts']['saved_segment_id'] = $values['saved_segment_id'];
  }

  if (!empty($values['subject_line'])) {
    $settings['subject_line'] = $values['subject_line'];
  }
  else {
    $settings['subject_line'] = $campaign->settings->subject_line;
  }

  if (!empty($values['from_name'])) {
    $settings['from_name'] = $values['from_name'];
  }
  else {
    $settings['from_name'] = $list->campaign_defaults->from_name;
  }

  if (!empty($values['reply_to'])) {
    $settings['reply_to'] = $values['reply_to'];
  }
  else {
    $settings['reply_to'] = $list->campaign_defaults->from_email;
  }

  emailmonkey_api('campaigns', 'updateCampaign', $campaign->id, $campaign->type, $recipients, $settings);
}

function emailmonkey_campaigns_set_campaign_content($campaign_id, $content) {
  $parameters = array('html' => $content);

  emailmonkey_api('campaigns', 'setCampaignContent', $campaign_id, $parameters);
}

function emailmonkey_campaigns_test_campaign($campaign_id, $recipients) {
  $type = 'html';

  emailmonkey_api('campaigns', 'sendTest', $campaign_id, $recipients, $type);
}

function emailmonkey_campaigns_get_campaign_basics($campaign_id) {
  $parameters = array('fields' => implode(',', EMAILMONKEY_BASIC_CAMPAIGN_FIELDS));

  return emailmonkey_api('campaigns', 'getCampaign', $campaign_id, $parameters);
}

function emailmonkey_campaigns_send_campaign($campaign_id) {
  emailmonkey_api('campaigns', 'send', $campaign_id);
}

function emailmonkey_campaigns_is_sent_status($status) {
  // This function defines "sent" with regards to this module, not in general.
  $statuses = array('paused', 'schedule', 'sending', 'sent');

  return in_array($status, $statuses);
}

function emailmonkey_campaigns_get_campaign_basics_for_node($node, $account_id) {
  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  return emailmonkey_campaigns_get_campaign_basics($campaign_id);
}

function emailmonkey_campaigns_delete_campaign_for_node($node, $account_id) {
  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  emailmonkey_api('campaigns', 'delete', $campaign_id);
}
