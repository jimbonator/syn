<?php

namespace Syn\Dispatch;

use Syn\Exception\ReportableException;
use Syn\Php\Args;

/**
 * Abstract command dispatcher.
 *
 * Dispatchers should add themselves to Registry's master list.
 */

abstract class AbstractDispatcher
{
  /**
   * Topic string selecting this dispatcher.
   *
   * @property string
   */
  public $topic;

  /**
   * @param string $topic
   */
  public function __construct($topic)
  {
    Args::req_string($topic);

    $this->topic = $topic;
  }

  /**
   * Returns a list of topics this dispatcher supports.
   *
   * @return array
   */
  public abstract static function topics();

  /**
   * Returns an instance of this dispatcher.
   *
   * @param string $topic
   * @return Dispatcher
   */
  public abstract static function factory($topic);

  /**
   * Dispatch (execute) the command.
   *
   * @param string|NULL $cmd
   * @param array $params
   * @return int|string
   * @throws RuntimeException
   * @throws Exception
   */
  public function dispatch($cmd, $params)
  {
    Args::opt_string($cmd);
    Args::req_array($params);

    $method = !isset($cmd) ? 'cmd_default' : "cmd_$cmd";
    $method = str_replace('-', '_', $method);

    if (!method_exists($this, $method))
      return "{$this->topic}: Unknown command \"$cmd\"";

    try {
      return $this->{$method}($params);
    } catch (ReportableException $re) {
      return sprintf("%s %s: %s", $this->topic, $cmd, $re->getMessage());
    }
  }
}
