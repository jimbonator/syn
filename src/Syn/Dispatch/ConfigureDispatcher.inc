<?php

namespace Syn\Dispatch;

use Syn\Exception\ReportableException;
use Syn\Php\Args;
use Syn\Php\Arrays;
use Syn\Storage\Config;

/**
 * Dispatcher for "config" topic.
 *
 * $cmd corresponds to the Config set_* method (which, in turn, corresponds to sections in the
 * configuration file).  Parameters are in the form "key=value".
 */

class ConfigureDispatcher extends AbstractDispatcher
{
  /**
   * @see Dispatcher::topics()
   */
  public static function topics()
  {
    return [ 'config', 'configure' ];
  }

  protected function cmd_default($params)
  {
    $config = Config::for_current_user();
    $config->load();

    $endpoint = $config->endpoint();
    if (!isset($endpoint))
      return 'No host endpoint configured';

    echo "Endpoint: $endpoint\n";
  }

  protected function cmd_endpoint($params)
  {
    Args::req_array($params);

    $kv = static::cmdline_to_keyvalue($params);

    $config = Config::for_current_user();
    $config->load();
    $config->set_endpoint($kv);
    $config->save();

    $endpoint = $config->endpoint();
    if (!isset($endpoint))
      return 'Endpoint not saved';

    echo "Endpoint: $endpoint\n";
  }

  /**
   * Converts command-line arguments to an array of key-value pairs.
   *
   * @param array $params
   * @return array
   * @throws ReportableException
   */
  private static function cmdline_to_keyvalue($params)
  {
    Args::req_array($params);

    $kv = [];
    foreach ($params as $param) {
      $split = explode('=', $param, 2);
      if (count($split) != 2)
        throw new ReportableException('Configuration is in the form of "key=value"');

      $kv[$split[0]] = $split[1];
    }

    return $kv;
  }
}
