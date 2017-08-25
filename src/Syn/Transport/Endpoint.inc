<?php

namespace Syn\Transport;

use Syn\Php\Args;

/**
 *
 */

class Endpoint
{
  /**
   * @property string
   */
  public $host;

  /**
   * @property int
   */
  public $port;

  /**
   * @property string
   */
  public $proto;

  /**
   * @param string $host
   * @param int $port
   * @param string $proto
   */
  public function __construct($host, $port, $proto = 'http')
  {
    Args::req_string($host);
    Args::req_intable($port);
    Args::req_string($proto);

    $this->host = $host;
    $this->port = intval($port);
    $this->proto = $proto;
  }

  /**
   * The API URL for this endpoint.
   *
   * $params is ignored if $cgi is NULL.
   *
   * @param string|null $cgi
   * @param array|NULL $params
   * @return string
   */
  public function url($cgi = null, $params = null)
  {
    Args::opt_string($cgi);
    Args::opt_array($params);

    if (!isset($cgi))
      return "{$this->proto}://{$this->host}:{$this->port}";

    if (isset($params) && !empty($params))
      $query = '?' . http_build_query($params);

    return "{$this->proto}://{$this->host}:{$this->port}/webapi/$cgi$query";
  }

  /**
   * Returns Endpoint as a human-readable string.
   */
  public function __toString()
  {
    return $this->url();
  }
}
