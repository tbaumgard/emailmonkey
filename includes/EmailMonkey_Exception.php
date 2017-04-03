<?php
/**
 * @file
 * File for the EmailMonkey_Exception class declaration.
 */

class EmailMonkey_Exception extends Exception
{

  public function __construct($message, $previous_exception=NULL)
  {
    parent::__construct($message, 0, $previous_exception);
  }

}
