<?php

namespace Syn\Storage;

use Syn\Exception\IOException;
use Syn\Php\Args;
use Syn\Php\File;

/**
 * For writing .ini files.
 *
 * IniWriter is an append-only interface.  The .ini file is constructed in memory and written once
 * to disk.
 *
 * @see parse_ini_file()
 * @see parse_ini_string()
 */

class IniWriter
{
  private $ini = '';

  /**
   * Add a comment line.
   *
   * Comments may not contain newlines.
   *
   * @param string|NULL $comment
   * @throws InvalidArgumentException
   */
  public function comment($comment)
  {
    Args::opt_string($comment);
    Args::requires(strpbrk($comment, "\n\r") === false);

    $this->ini .= "; $comment\n";
  }

  /**
   * Start a section.
   *
   * Section names may not contain newlines.
   *
   * @param string $section
   * @throws InvalidArgumentException
   */
  public function section($section)
  {
    Args::req_string($section);
    Args::requires(strpbrk($section, "\n\r") === false);

    $this->ini .= "[$section]\n";
  }

  /**
   * Add a key-value pair.
   *
   * Add "global" key-values before adding sections.  Once a section is defined, all key-values
   * thereafter are part of that section.
   *
   * Keys and values may not contain newlines.  Keys may not contain equal signs ("=") or
   * semi-colons (";").
   *
   * @param string $section
   * @throws InvalidArgumentException
   */
  public function key_value($key, $value)
  {
    Args::req_string($key);
    Args::req_scalar($value);
    Args::requires(strpbrk($key, "\n\r=;") === false);
    if (is_string($value))
      Args::requires(strpbrk($value, "\n\r") === false);

    $this->ini .= "$key=";
    $this->ini .= (is_numeric($value) ? $value : "\"$value\"");
    $this->ini .= "\n";
  }

  /**
   * Write the INI to a file.
   *
   * @param File $file
   * @throws IOException
   */
  public function write($file)
  {
    Args::req_is_a($file, File::class);

    if (file_put_contents($file, $this->ini, LOCK_EX) === false)
      throw IOException::for_last_error("Unable to write to $file");
  }
}
