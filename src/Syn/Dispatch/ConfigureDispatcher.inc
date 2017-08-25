<?php

namespace Syn\Dispatch;

use Syn\Exception\ReportableException;
use Syn\Php\Args;
use Syn\Php\Arrays;
use Syn\Storage\Config;

/**
 * Dispatcher for "config" topic.
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
      return 'No host information configured';

    echo "Endpoint: $endpoint\n";
  }

  protected function cmd_endpoint($params)
  {
    $host = Arrays::required($params, 0, new ReportableException('host parameter required'));
    $port = Arrays::optional($params, 1, Config::DEFAULT_PORT);

    $config = Config::for_current_user();
    $config->set_hostinfo($host, intval($port));

    $config->save();
  }
}
