<?php

namespace Syn\Dispatch;

use Syn\Config\Storage;
use Syn\Php\Args;
use Syn\Transport\Request;

/**
 *
 */

class InfoDispatcher extends AbstractDispatcher
{
  /**
   * @inheritDoc
   */
  public static function topics()
  {
    return [ 'info' ];
  }

  protected function cmd_default($params)
  {
    $storage = Storage::for_current_user();
    $storage->load();

    $endpoint = $storage->endpoint();
    if (!$endpoint)
      return 'Not configured';

    $api_params = [
      'api' => 'SYNO.API.Info',
      'version' => 1,
      'method' => 'query',
      'query' => 'SYNO.API.Auth',
    ];

    $request = new Request($endpoint, 'query.cgi', $api_params);
    $response = $request->exec();
    var_dump($response);

    return 0;
  }
}
