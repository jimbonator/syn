<?php

namespace Php;

use InvalidArgumentException;

/**
 * Function / method argument checking.
 */

class Args
{
  /**
   * Requires $mixed to be a string or NULL.
   */
  public static function opt_string($mixed)
  {
    static::opt($mixed, 'is_string', 'Argument is not a string or NULL');
  }

  /**
   * Requires $mixed to be an array.
   */
  public static function req_array($mixed)
  {
    static::req($mixed, 'is_array', 'Argument is not an array');
  }

  /**
   * Requires $mixed to be an object.
   */
  public static function req_object($mixed)
  {
    static::req($mixed, 'is_object', 'Argument is not an object');
  }

  /**
   * Generic required argument test.
   *
   * @param mixed $mixed
   * @param callable $test
   * @param string $failure
   * @throws InvalidArgumentException
   */
  protected static function req($mixed, $test, $failure)
  {
    if (!call_user_func($test, $mixed))
      throw new InvalidArgumentException($failure);
  }

  /**
   * Generic optional argument test.
   *
   * @param mixed $mixed
   * @param callable $test
   * @param string $failure
   * @throws InvalidArgumentException
   */
  protected static function opt($mixed, $test, $failure)
  {
    if (!is_null($mixed) && !call_user_func($test, $mixed))
      throw new InvalidArgumentException($failure);
  }
}
