#/usr/bin/perl
my $req= "GET /httptest/wikipedia_russia.html HTTP/1.1\r\n".
  "User-Agent: YandexTank/1.1.1\r\n".
  "Host: localhost\r\n".
  "Accept-Encoding: gzip, deflate\r\n".
  "Connection: Close\r\n".
  "\r\n";
print length($req)."\n".$req;
