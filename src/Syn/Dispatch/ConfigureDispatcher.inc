<?php

namespace Syn\Dispatch;

use Syn\Config\Storage;
use Syn\Exception\ReportableException;
use Syn\Php\Args;
use Syn\Php\Arrays;

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

  /**
   * @see Dispatcher::factory()
   */
  public static function factory($topic)
  {
    return new ConfigureDispatcher($topic);
  }

  protected function cmd_default($params)
  {
    $storage = Storage::for_current_user();
    $storage->load();

    $hostinfo = $storage->hostinfo();
    if (!isset($hostinfo) || empty($hostinfo))
      return 'No host information configured';

    echo "Host: {$hostinfo['host']}:{$hostinfo['port']}\n";
  }

  protected function cmd_hostinfo($params)
  {
    $host = Arrays::required($params, 0, new ReportableException('host parameter required'));
    $port = Arrays::optional($params, 1, Storage::DEFAULT_PORT);

    $storage = Storage::for_current_user();
    $storage->set_hostinfo($host, $port);

    $storage->save();
  }
}
