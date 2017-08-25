<?php

namespace Syn\Php;

/**
 * Basic file and filename manipulation.
 */

class File
{
  /**
   * @property string
   */
  public $path;

  /**
   * @param string $filename
   */
  public function __construct($path)
  {
    Args::req_string($path);

    // watch for root
    $this->path = static::normalize($path);
  }

  /**
   * Factory for fluent programming.
   *
   * @param string $path
   * @return File
   */
  public static function for_path($path)
  {
    return new static($path);
  }

  /**
   * Normalize a filename.
   *
   * @param string $filename
   * @return string
   */
  public static function normalize($filename)
  {
    Args::req_string($filename);

    return ($filename !== DIRECTORY_SEPARATOR) ? rtrim($filename, DIRECTORY_SEPARATOR) : $filename;
  }

  /**
   * Child file.
   *
   * @param string\File $child
   * @return File
   */
  public function child($child)
  {
    Args::req_stringable($child);

    return new static($this->path . DIRECTORY_SEPARATOR . $child);
  }

  /**
   * @see $path
   */
  public function __toString()
  {
    return $this->path;
  }
}
