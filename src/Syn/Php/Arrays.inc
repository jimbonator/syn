<?php

namespace Syn\Php;

use Exception;

/**
 * Utilities for dealing with PHP arrays.
 */

class Arrays
{
  /**
   * Returns the array's key's value or throws an Exception.
   *
   * @param array $arr
   * @param scalar|NULL $key
   * @param Exception $exception
   * @return mixed
   * @throws Exception
   */
  public static function required($arr, $key, $exception)
  {
    Args::req_array($arr);
    Args::opt_scalar($key);
    Args::req_is_a($exception, Exception::class);

    if (!isset($arr[$key]))
      throw $exception;

    return $arr[$key];
  }

  /**
   * Returns the array's key's value or a default value.
   *
   * @param array $arr
   * @param scalar|NULL $key
   * @param mixed $default
   * @return mixed
   */
  public static function optional($arr, $key, $default = null)
  {
    Args::req_array($arr);
    Args::opt_scalar($key);

    return isset($arr[$key]) ? $arr[$key] : $default;
  }
}
