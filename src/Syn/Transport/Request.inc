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
  const TIMEOUT_MS = 8000;

  private $ch;
  private $endpoint;
  private $cgi;
  private $params;

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
  }

  function __destruct()
  {
    curl_close($this->ch);
  }

  public function exec()
  {
    curl_reset($this->ch);
    curl_setopt($this->ch, CURLOPT_URL, $this->endpoint->url($this->cgi, $this->params));
    curl_setopt($this->ch, CURLOPT_HEADER, false);
    curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($this->ch, CURLOPT_CONNECTTIMEOUT_MS, static::TIMEOUT_MS);
    curl_setopt($this->ch, CURLOPT_TIMEOUT_MS, static::TIMEOUT_MS);
    curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, true);
    curl_setopt($this->ch, CURLOPT_AUTOREFERER, true);
    // necessary?
    curl_setopt($this->ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER, 0);

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
