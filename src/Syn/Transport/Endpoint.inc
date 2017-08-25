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
   */
  public function __construct($host, $port, $proto = 'http')
  {
    Args::req_string($host);
    Args::req_int($port);
    Args::req_string($proto);

    $this->host = $host;
    $this->port = $port;
    $this->proto = $proto;
  }

  /**
   * The API URL for this endpoint.
   *
   * @param string $cgi
   * @param array|NULL $params
   * @return string
   */
  public function url($cgi, $params = null)
  {
    Args::req_string($cgi);
    Args::opt_array($params);

    if (isset($params) && !empty($params))
      $query = '?' . http_build_query($params);

    return "{$this->proto}://{$this->host}:{$this->port}/webapi/$cgi$query";
  }
}
