__author__ = 'Administrator'
# -*- coding: UTF-8 -*-
import sys
import oper_database
import logging
reload(sys)
sys.setdefaultencoding('utf-8')

def pickMatch(websrc, config, dbo):

    #dbo = oper_database.oper_db()
    task = config['task']
    fields = config['field']
    pos_beginkeyword = 0
    pos_loopkeyword = 0
    pos_loopkeyword2 = 0
    pos_endkeyword = 0
    bkeywd = ''
    ekeywd = ''
    loopkeywd = task['loopkeyword']
    global_keywd = task['beginkeyword']
    websrc = websrc[websrc.find(global_keywd)+len(global_keywd):]
    print  websrc
    while pos_loopkeyword != -1:
        if loopkeywd:
            if pos_loopkeyword == 0:
                pos_loopkeyword = websrc.find(loopkeywd, pos_loopkeyword)
                pos_loopkeyword2 = websrc.find(loopkeywd, pos_loopkeyword + len(loopkeywd))
                pos_beginkeyword = pos_loopkeyword
            elif pos_loopkeyword > 0 and pos_loopkeyword2 != -1:
                pos_loopkeyword2 = websrc.find(loopkeywd, pos_loopkeyword + len(loopkeywd))
                pos_beginkeyword = pos_loopkeyword
            else:
                pos_loopkeyword = -1
        else:
            pos_loopkeyword = -1
        result = {}

        for field in fields:
            name = field['name']
            bkeywd = field['beginkeyword']
            ekeywd = field['endkeyword']
            ikeywd = field['iskeyword']
            pos_beginkeyword = websrc.find(bkeywd, pos_beginkeyword)
            if pos_beginkeyword != -1 and (pos_loopkeyword2 == -1 or (pos_beginkeyword < pos_loopkeyword2)):
                pos_endkeyword = websrc.find(ekeywd, pos_beginkeyword)
                if pos_endkeyword != -1 and (pos_loopkeyword2 == -1 or (pos_endkeyword < pos_loopkeyword2)):
                    str = websrc[pos_beginkeyword + len(bkeywd):pos_endkeyword]
                    pos_beginkeyword = pos_endkeyword + len(ekeywd)
                    if ikeywd and ikeywd != '0' and not str:
                        result.clear()
                        break
                    else:
                        result[name] = str.strip()

                else:
                    str = websrc[pos_beginkeyword + len(bkeywd):pos_beginkeyword + len(bkeywd) + 20]
                    if ikeywd and ikeywd != '0' and not str:
                        result.clear()
                        break
                    else:
                        result[name] = str.strip()
            else:
                pos_beginkeyword = pos_endkeyword
                if ikeywd and ikeywd != '0':
                    result.clear()
                    break
        if result:
            print result
            dbo.oper_Insert('maploc', result)
        if pos_loopkeyword != -1 and pos_loopkeyword2 != -1:
            pos_loopkeyword = pos_loopkeyword2

    #dbo.oper_Closed()