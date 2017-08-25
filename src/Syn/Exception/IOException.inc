<?php

namespace Syn\Exception;

use Exception;
use Syn\Php\Args;

/**
 * I/O-related errors.
 */

class IOException extends Exception
{
  /**
   * Create an IOException with the last file error reported in the message and error code.
   *
   * @param string $msg
   * @return IOException
   */
  public static function for_last_error($msg)
  {
    Args::req_string($msg);

    $err = error_get_last();

    return new static("$msg ({$err['message']} at {$err['file']}:{$err['line']}", $err['type']);
  }
}
