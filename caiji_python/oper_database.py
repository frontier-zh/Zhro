__author__ = 'Administrator'
# -*- coding: UTF-8 -*-
import MySQLdb
import sys
import logging
reload(sys)
sys.setdefaultencoding('utf-8')

class oper_db:

    def __init__(self):
        self.db = None
        try:
            self.db = MySQLdb.connect(host='127.0.0.1', user='sa', passwd='1111', db='data_get', charset="utf8")
            cursor = self.db.cursor()
            cursor.execute("SELECT now()")
            logging.debug('Mysql db conn succ: %s', str(cursor.fetchone()))
            print cursor.fetchone()
        except Exception, e:
            print 'mysql db conn fail:', str(e)
            logging.debug('Mysql db conn fail: %s', str(e))
            self.db.rollback()

    def oper_Insert(self, tblnm, dataset):

        cursor = self.db.cursor()

        sql = 'INSERT INTO ' + tblnm + '('
        val = ''
        for key in dataset.keys():
            sql += key + ','
            val += '%s,'
        sql = sql[:len(sql)-1]
        val = val[:len(val)-1]
        sql += ') VALUES (' + val
        sql += ');'

        try:
            cursor.execute(sql, dataset.values())
            self.db.commit()
        except Exception, e:
            print 'mysql except throwing:', str(e)
            logging.debug('Mysql operator execption: %s', str(e))
            self.db.rollback()

    def oper_Closed(self):
        print "close db connection"
        logging.debug('Mysql db conn closed')
        self.db.close()
