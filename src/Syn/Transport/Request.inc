<?php

namespace Syn\Transport;

use RuntimeException;
use Syn\Exception\IOException;
use Syn\Php\Args;

/**
 *
 */

class Request
{
  private $ch;
  private $endpoint;
  private $cgi;
  private $params;
  private $cx_params;

  /**
   * @param Endpoint $endpoint
   */
  public function __construct($endpoint, $cgi, $params = null)
  {
    Args::req_is_a($endpoint, Endpoint::class);
    Args::req_string($cgi);
    Args::opt_array($params);

    $this->ch = curl_init();
    if ($this->ch === false)
      throw new RuntimeException('Unable to allocate cURL handle');
    $this->endpoint = $endpoint;
    $this->cgi = $cgi;
    $this->params = $params;
    $this->cx_params = new ConnectionParams();
  }

  function __destruct()
  {
    curl_close($this->ch);
  }

  /**
   * Set connection parameters for request.
   *
   * @param ConnectionParams $cx_params
   */
  public function set_connection_params($cx_params)
  {
    Args::req_is_a($cx_params, ConnectionParams::class);

    $this->cx_params = $cx_params;
  }

  public function exec()
  {
    curl_reset($this->ch);
    curl_setopt($this->ch, CURLOPT_URL, $this->endpoint->url($this->cgi, $this->params));

    curl_setopt($this->ch, CURLOPT_HEADER, false);
    curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, true);
    curl_setopt($this->ch, CURLOPT_AUTOREFERER, true);

    curl_setopt($this->ch, CURLOPT_CONNECTTIMEOUT_MS, $this->cx_params->connect_timeout_sec * 1000);
    curl_setopt($this->ch, CURLOPT_TIMEOUT_MS, $this->cx_params->timeout_sec * 1000);

    curl_setopt($this->ch, CURLOPT_SSL_VERIFYHOST, $this->cx_params->verify_ssl);
    curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER, $this->cx_params->verify_ssl);

    $json = curl_exec($this->ch);
    $info = curl_getinfo($this->ch);

    if ($json === false)
      throw $this->io_exception();

    return json_decode($json);
  }

  private function io_exception()
  {
    $msg = sprintf("%s (%d): %s", curl_error($this->ch), curl_errno($this->ch), $this->endpoint);

    return new IOException($msg, curl_errno($this->ch));
  }
}
