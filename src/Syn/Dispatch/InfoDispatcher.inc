<?php

namespace Syn\Dispatch;

use Syn\Php\Args;
use Syn\Storage\Config;
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
    $config = Config::for_current_user();
    $config->load();

    $endpoint = $config->endpoint();
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
