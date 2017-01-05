import httplib2, urllib

def http_cookie():
    url = 'http://127.0.0.1:8080/login'
    body = {'username': 'foo', 'password': 'bar'}  
    headers = {'Content-type': 'application/x-www-form-urlencoded'}  

    http = httplib2.Http()
    response, content = http.request(url, 'POST', headers=headers, body=urllib.urlencode(body))
    print '--------------------'
    print response
    print content
    print '--------------------'

    url = 'http://127.0.0.1:8080/index'
    headers = {'Cookie': response['set-cookie']}
    response, content = http.request(url, 'GET', headers=headers)
    print '--------------------'
    print response
    print content
    print '--------------------'


def download_ova():
    url = "http://downdb.51voa.com/201701/2712-takeover.mp3"
    proxy = httplib2.ProxyInfo(httplib2.socks.PROXY_TYPE_HTTP, "proxy.zte.com.cn", 80)
    http = httplib2.Http(proxy_info=proxy)
    response, content = http.request(url, 'GET')
    with open("./2712-takeover.mp3", "wb+") as f:
        f.write(content)

if __name__ == "__main__":
    download_ova()