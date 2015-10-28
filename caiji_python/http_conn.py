#encoding:utf-8
import sys
import logging
import StringIO, re
try:
	import pycurl	# 非内置模块
except:
	pass

def curl(url, debug=False, **kwargs):
	while 1:
		try:
			s = StringIO.StringIO()
			c = pycurl.Curl()
			c.setopt(pycurl.URL, url)
			c.setopt(pycurl.REFERER, url)
			c.setopt(pycurl.FOLLOWLOCATION, True)
			c.setopt(pycurl.TIMEOUT, 60)
			c.setopt(pycurl.ENCODING, 'gzip')
			c.setopt(pycurl.USERAGENT, 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/31.0.1650.63 Safari/537.36')
			c.setopt(pycurl.NOSIGNAL, True)
			c.setopt(pycurl.WRITEFUNCTION, s.write)
			for k, v in kwargs.iteritems():
                            c.setopt(vars(pycurl)[k], v)
			c.perform()
			c.close()
			return s.getvalue()
		except Exception, e:
			logging.debug('http request execption: %s', str(e))
			logging.debug('http failed url: %s', str(url))
			if debug:
				raise
			continue

# for href in open('file.txt', 'r'):
#     html = curl(href)
#     print html
