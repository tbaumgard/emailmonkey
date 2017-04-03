<?php

/**
 * @file
 * Items used for MailChimp list segments.
 */

// -----------------------------------------------------------------------------
// Constants

define('EMAILMONKEY_BASIC_SEGMENTS_FIELDS', array(
  'list_id', 'segments.id', 'segments.name', 'segments.member_count',
  'segments.list_id', 'segments.type', 'total_items'));

define('EMAILMONKEY_BASIC_SEGMENT_FIELDS', array(
  'id', 'name', 'member_count', 'list_id', 'type'));

// -----------------------------------------------------------------------------
// Functions

function emailmonkey_segments_get_segments_basics($list_id, $max_segments=NULL, $offset=NULL) {
  $parameters = array('fields' => implode(',', EMAILMONKEY_BASIC_SEGMENTS_FIELDS));

  if (isset($max_segments)) {
    $parameters['count'] = $max_segments;
  }

  if (isset($offset)) {
    $parameters['offset'] = $offset;
  }

  return emailmonkey_api('segments', 'getSegments', $list_id, $parameters);
}

function emailmonkey_segments_get_segment_basics($list_id, $segment_id) {
  $parameters = array('fields' => implode(',', EMAILMONKEY_BASIC_SEGMENT_FIELDS));

  return emailmonkey_api('segments', 'getSegment', $list_id, $segment_id, $parameters);
}

function emailmonkey_segments_get_segment_count($list_id) {
  // count = 0 returns all of the segments.
  $parameters = array('count' => 1, 'fields' => 'total_items');
  $segments = emailmonkey_api('segments', 'getSegments', $list_id, $parameters);

  return $segments->total_items;
}
