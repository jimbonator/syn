<?php

namespace Syn\Storage;

use Syn\Exception\IOException;
use Syn\Exception\ReportableException;
use Syn\Php\Args;
use Syn\Php\Arrays;
use Syn\Php\File;
use Syn\Transport\ConnectionParams;
use Syn\Transport\Endpoint;

/**
 *
 */

class Config
{
  const HOME_FILENAME = '.syn';

  const DEFAULT_PROTO = 'http';
  const DEFAULT_PORT = 5000;

  private $file;
  private $config = [];

  /**
   * @param File $file
   */
  protected function __construct($file)
  {
    Args::req_is_a($file, File::class);

    $this->file = $file;
  }

  /**
   * Configuration storage for current user.
   *
   * @return Storage
   */
  public static function for_current_user()
  {
    $home = getenv('HOME');
    if (!strlen($home))
      throw new RuntimeException('HOME environment variable must be set');

    $file = File::for_path($home)->child(static::HOME_FILENAME);

    return new static($file);
  }

  /**
   * Read configuration into memory.
   *
   * Unsaved changes will be discarded.
   *
   * @throws IOException
   */
  public function load()
  {
    $config = parse_ini_file($this->file, true);
    if ($config !== false)
      $this->config = $config;
  }

  /**
   * Write configuration to backing store.
   *
   * @throws IOException
   */
  public function save()
  {
    $ini = new IniWriter();

    $ini->comment('syn configuration file');
    $ini->comment("DO NOT MODIFY UNLESS YOU KNOW WHAT YOU'RE DOING");

    foreach ($this->config as $key => $value) {
      if (is_array($value)) {
        $ini->section($key);
        foreach ($value as $seckey => $secvalue)
          $ini->key_value($seckey, $secvalue);
      } else {
        $ini->key_value($key, $value);
      }
    }

    $umask = umask(077);
    try {
      $ini->write($this->file);
    } finally {
      umask($umask);
    }
  }

  /**
   * API endpoint.
   *
   * @return Endpoint|null
   * @throws ReportableException
   */
  public function endpoint()
  {
    $endpoint = Arrays::required($this->config, 'endpoint', new ReportableException('No endpoint configured'));

    $host = Arrays::required($endpoint, 'host', new ReportableException('No host configured'));
    $port = Arrays::optional($endpoint, 'port', static::DEFAULT_PORT);
    $proto = Arrays::optional($endpoint, 'proto', static::DEFAULT_PROTO);

    return new Endpoint($host, $port, $proto);
  }

  /**
   * API connection parameters.
   *
   * @return ConnectionParams
   */
  public function connection_params()
  {
    $cx = Arrays::optional($this->config, 'cx', []);

    $params = new ConnectionParams();
    $params->set_connect_timeout(Arrays::optional($cx, 'connect-timeout'));
    $params->set_timeout_sec(Arrays::optional($cx, 'timeout'));
    $params->set_verify_ssl(Arrays::optional($cx, 'verify-ssl'));

    return $params;
  }

  /**
   * Set API endpoint.
   *
   * Required parameters: host
   * Optional parameters: port, proto
   *
   * @param array $kv
   * @throws ReportableException
   */
  public function set_endpoint($kv)
  {
    Args::req_array($kv);

    $endpoint = Arrays::optional($this->config, 'endpoint', []);

    Arrays::assign_if_set($endpoint, $kv, 'host');
    Arrays::assign_if_set($endpoint, $kv, 'port', 'intval');
    Arrays::assign_if_set($endpoint, $kv, 'proto');

    Arrays::required($endpoint, 'host', new ReportableException('"host" parameter required'));
    $proto = Arrays::optional($endpoint, 'proto');
    if ($proto && !in_array($proto, [ 'http', 'https' ]))
      throw new ReportableException('"proto" parameter may only be "http" or "https"');

    $this->config['endpoint'] = $endpoint;
  }

  /**
   * Set API connection parameters.
   *
   * Optional parameters: connect-timeout, timeout, verify-ssl
   *
   * @param array $kv
   * @throws ReportableException
   */
  public function set_connection_params($kv)
  {
    Args::req_array($kv);

    $cx = Arrays::optional($this->config, 'cx', []);

    Arrays::assign_if_set($cx, $kv, 'connect-timeout', 'intval');
    Arrays::assign_if_set($cx, $kv, 'timeout', 'intval');
    Arrays::assign_if_set($cx, $kv, 'verify-ssl', 'Syn\Storage\IniWriter::boolval');

    if (count($cx))
      $this->config['cx'] = $cx;
  }
}
