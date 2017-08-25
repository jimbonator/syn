<?php

namespace Syn\Php;

use InvalidArgumentException;

/**
 * Function / method argument checking.
 */

class Args
{
  /**
   * Determines if $mixed may be coerced into a string.
   *
   * NULL is not considered coercible to a string for argument-checking purposes.
   *
   * @return boolean
   */
  public static function is_stringable($mixed)
  {
    return is_scalar($mixed) || (is_object($mixed) && method_exists($mixed, '__toString'));
  }

  /**
   * Requires $mixed to be a boolean.
   */
  public static function req_bool($mixed)
  {
    static::req($mixed, 'is_bool', 'Argument is not a boolean');
  }

  /**
   * Requires $mixed to be a boolean or NULL.
   */
  public static function opt_bool($mixed)
  {
    static::opt($mixed, 'is_bool', 'Argument is not a boolean or NULL');
  }

  /**
   * Requires $mixed to be an integer.
   */
  public static function req_int($mixed)
  {
    static::req($mixed, 'is_int', 'Argument is not an integer');
  }

  /**
   * Requires $mixed to be an integer or NULL.
   */
  public static function opt_int($mixed)
  {
    static::opt($mixed, 'is_int', 'Argument is not an integer or NULL');
  }

  /**
   * Requires $mixed to be a string.
   */
  public static function req_string($mixed)
  {
    static::req($mixed, 'is_string', 'Argument is not a string');
  }

  /**
   * Requires $mixed to be a string or NULL.
   */
  public static function opt_string($mixed)
  {
    static::opt($mixed, 'is_string', 'Argument is not a string or NULL');
  }

  /**
   * Requires $mixed to be coerced to a string.
   */
  public static function req_stringable($mixed)
  {
    static::req($mixed, 'static::is_stringable', 'Argument is not coercible to a string');
  }

  /**
   * Requires $mixed to be coerced to a string or NULL.
   */
  public static function opt_stringable($mixed)
  {
    static::opt($mixed, 'static::is_stringable', 'Argument is not coercible to a string or NULL');
  }

  /**
   * Requires $mixed to be a scalar.
   */
  public static function req_scalar($mixed)
  {
    static::req($mixed, 'is_scalar', 'Argument is not a scalar');
  }

  /**
   * Requires $mixed to be a scalar or NULL.
   */
  public static function opt_scalar($mixed)
  {
    static::opt($mixed, 'is_scalar', 'Argument is not a scalar or NULL');
  }

  /**
   * Requires $mixed to be an array.
   */
  public static function req_array($mixed)
  {
    static::req($mixed, 'is_array', 'Argument is not an array');
  }

  /**
   * Requires $mixed to be an array or NULL.
   */
  public static function opt_array($mixed)
  {
    static::opt($mixed, 'is_array', 'Argument is not an array or NULL');
  }

  /**
   * Requires $mixed to be an object.
   */
  public static function req_object($mixed)
  {
    static::req($mixed, 'is_object', 'Argument is not an object');
  }

  /**
   * Requires $mixed to be an object or NULL.
   */
  public static function opt_object($mixed)
  {
    static::opt($mixed, 'is_object', 'Argument is not an object or NULL');
  }

  /**
   * Requires $mixed to be an object instance of the specified class, trait, or interface.
   */
  public static function req_is_a($mixed, $class)
  {
    static::req($mixed, function ($mixed) use ($class) {
      return $mixed instanceof $class;
    }, "Argument is not an instance of $class");
  }

  /**
   * Requires $mixed to be NULL or an object instance of the specified class, trait, or interface.
   */
  public static function opt_is_a($mixed, $class)
  {
    static::opt($mixed, function ($mixed) use ($class) {
      return $mixed instanceof $class;
    }, "Argument is not NULL or an instance of $class");
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
