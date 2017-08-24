<?php

namespace Syn\Dispatch;

use RuntimeException;
use Syn\Php\Args;
use Syn\Php\Arrays;

/**
 * Registry of all command dispatchers.
 */

class Registry
{
  /**
   * Master list of all dispatchers.
   */
  public static $DISPATCHERS = [
    ConfigureDispatcher::class,
  ];

  private $map = [];

  /**
   *
   */
  public function __construct()
  {
    foreach (static::$DISPATCHERS as $dispatch_class) {
      foreach ($dispatch_class::topics() as $topic) {
        if (isset($this->map[$topic]))
          throw new RuntimeException("{$dispatch_class} shares topic \"$topic\" with {$this->map[$topic]}");

        $this->map[$topic] = $dispatch_class;
      }
    }
  }

  /**
   * The singleton instance of Registry.
   *
   * @return Registry
   */
  public static function instance()
  {
    static $instance = null;
    if (!$instance)
      $instance = new Registry();

    return $instance;
  }

  /**
   * Convert a topic string to a dispatcher.
   *
   * @param string $topic
   * @return AbstractDispatcher|null
   */
  public function lookup($topic)
  {
    Args::req_string($topic);

    $dispatch_class = Arrays::optional($this->map, $topic);

    return isset($dispatch_class) ? $dispatch_class::factory($topic) : null;
  }
}
