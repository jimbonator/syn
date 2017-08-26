<?php

namespace Syn\Transport;

/**
 * Assorted connection parameters.
 *
 * The setters are generally forgiving in reporting errors but strict in accepting values.
 */

class ConnectionParams
{
  const DEFAULT_TIMEOUT_SEC = 8;

  public $connect_timeout_sec = self::DEFAULT_TIMEOUT_SEC;
  public $timeout_sec = self::DEFAULT_TIMEOUT_SEC;
  public $verify_ssl = false;

  public function set_connect_timeout($sec)
  {
    $sec = intval($sec);
    if ($sec > 0)
      $this->connect_timeout_sec = $sec;
  }

  public function set_timeout_sec($sec)
  {
    $sec = intval($sec);
    if ($sec > 0)
      $this->timeout_sec = $sec;
  }

  public function set_verify_ssl($verify_ssl)
  {
    if (isset($verify_ssl))
      $this->verify_ssl = boolval($verify_ssl);
  }

  /**
   * Human-readable output.
   */
  public function __toString()
  {
    return sprintf(
      "connect timeout = %ds, timeout = %ds, verify SSL = %s",
      $this->connect_timeout_sec,
      $this->timeout_sec,
      $this->verify_ssl ? 'ON' : 'OFF'
    );
  }
}
