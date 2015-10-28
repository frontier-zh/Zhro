__author__ = 'Administrator'
# -*- coding: UTF-8 -*-
import sys
import os
import logging
import http_conn
import parse_match
import parse_xml
import oper_database
reload(sys)
sys.setdefaultencoding('utf-8')



if __name__ == '__main__':

    logging.basicConfig(format='%(asctime)s | %(message)s',
            filename=os.path.join('.', 'log.txt'),
            level=logging.DEBUG)

    dbo = oper_database.oper_db()

    try:
        parseXml = parse_xml.xml('taskandfields.txt')
        taskConf = parseXml.getConfig()

        logging.debug('Task and field \n%s\n%s', str(taskConf['task']), str(taskConf['field']))
        for href in open('url.txt', 'r'):
            websrc = http_conn.curl(href)
            parse_match.pickMatch(websrc.decode('utf-8'), taskConf, dbo)
    except Exception, e:
        logging.debug('run error %s', str(e))

    dbo.oper_Closed()
    print 'Task execute finished..'