<?php

namespace Syn\Dispatch;

use Syn\Config\Storage;
use Syn\Php\Args;

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

    $hostinfo = $storage->hostinfo();
    if (!$hostinfo)
      return 'Not configured';

    $host = $hostinfo['host'];
    $port = $hostinfo['port'];

    $api_params = [
      'api' => 'SYNO.API.Info',
      'version' => 1,
      'method' => 'query',
      'query' => 'SYNO.API.Auth',
    ];

    $url = "http://$host/webapi/query.cgi?" . http_build_query($api_params);

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, true);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT_MS, 8000);
    curl_setopt($ch, CURLOPT_TIMEOUT_MS, 8000);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
    curl_setopt($ch, CURLOPT_AUTOREFERER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);

    $response = curl_exec($ch);
    $info = curl_getinfo($ch);

    if ($response === false)
      echo curl_error($ch);
    var_dump($response);
    var_dump($info);

    curl_close($ch);

    return 0;
  }
}
