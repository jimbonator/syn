<?php

namespace Syn\Config;

use Syn\Exception\IOException;
use Syn\Php\Args;
use Syn\Php\Arrays;
use Syn\Php\File;
use Syn\Transport\Endpoint;

/**
 *
 */

class Storage
{
  const HOME_FILENAME = '.syn';
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
    $ini = '';

    static::write_comment($ini, 'syn configuration file');
    static::write_comment($ini, "DO NOT MODIFY UNLESS YOU KNOW WHAT YOU'RE DOING");
    foreach ($this->config as $key => $value) {
      if (is_array($value)) {
        static::write_section($ini, $key);
        foreach ($value as $seckey => $secvalue)
          static::write_key($ini, $seckey, $secvalue);
      } else {
        static::write_key($ini, $key, $value);
      }
    }

    if (file_put_contents($this->file, $ini, LOCK_EX) === false)
      throw new IOException("Unable to write to {$this->file}");
  }

  private static function write_comment(&$ini, $comment)
  {
    Args::req_string($ini);
    Args::opt_string($comment);

    $ini .= '; ';
    if (isset($comment))
      $ini .= $comment;
    $ini .= "\n";
  }

  private static function write_section(&$ini, $section)
  {
    Args::req_string($ini);
    Args::req_string($section);

    $ini .= "\n[$section]\n";
  }

  private static function write_key(&$ini, $key, $value)
  {
    Args::req_string($ini);
    Args::req_string($key);
    Args::req_scalar($value);

    $ini .= "$key=";
    $ini .= (is_numeric($value) ? $value : "\"$value\"");
    $ini .= "\n";
  }

  /**
   * API endpoint.
   *
   * @return Endpoint|null
   */
  public function endpoint()
  {
    $hostinfo = Arrays::optional($this->config, 'hostinfo');
    if (!is_array($hostinfo) || empty($hostinfo))
      return null;

    // port default
    if (!isset($hostinfo['port']))
      $hostinfo['port'] = self::DEFAULT_PORT;

    return new Endpoint($hostinfo['host'], intval($hostinfo['port']));
  }

  /**
   * Set host information.
   *
   * @param string $host
   * @param int $port
   */
  public function set_hostinfo($host, $port = self::DEFAULT_PORT)
  {
    Args::req_string($host);
    Args::req_int($port);

    $this->config['hostinfo'] = [ 'host' => $host, 'port' => $port ];
  }
}
