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

  /**
   * Assign the for value $key to $src if present in $dst.
   *
   * @param &array $dst
   * @param array $src
   * @param string $key
   * @return boolean            If assigned
   */
  public static function assign_if_set(&$dst, $src, $key)
  {
    Args::req_array($dst);
    Args::req_array($src);
    Args::opt_scalar($key);

    if (!isset($src[$key]))
      return false;

    $dst[$key] = $src[$key];

    return true;
  }
}
