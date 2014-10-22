#!/usr/bin/perl
my $data ="username=igor&password=igor";
my $req= "POST /auth? HTTP/1.0\r\n".
  "User-Agent: yandex-tank/1.1.1\r\n".
  "Host: localhost:8090\r\n".
  "Content-Length:".length ($data)."\r\n".
  "Content-Type: application/x-www-form-urlencoded\r\n".
  "Connection: Close\r\n".
  "\r\n".
  $data."\r\n".
  "\r\n";
print length($req)."\n".$req;
