<?php

namespace Syn\Dispatch;

use RuntimeException;
use Syn\Php\Args;

/**
 * Dispatcher for "config" topic.
 */

class ConfigureDispatcher extends AbstractDispatcher
{
  const DEFAULT_PORT = 5000;

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
    echo "configure\n";
  }

  protected function cmd_set_host($params)
  {
    $host = Arrays::required($params, 'host', new RuntimeException('host parameter required'));
    $port = Arrays::optional($params, 'port', static::DEFAULT_PORT);
  }
}
