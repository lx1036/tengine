
// compile:
// cc -l curl -o main main.c 
// gcc -l curl -o main main.c

#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://baidu.com");
    res = curl_easy_perform(curl);

    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  return 0;
}

// <html>
// <meta http-equiv="refresh" content="0;url=http://www.baidu.com/">
// </html>