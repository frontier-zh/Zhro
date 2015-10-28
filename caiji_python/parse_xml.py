__author__ = 'Administrator'
# -*- coding: UTF-8 -*-
from xml.dom import minidom, Node
import sys
import parse_match
import logging
reload(sys)
sys.setdefaultencoding('utf-8')

class xml:

    def __init__(self, file):
        self.result = {}
        self.task = {}
        self.field = []
        doc = minidom.parse(file)
        doc = doc.firstChild
        for child in doc.childNodes:
            print child.nodeType
            if child.nodeType == Node.ELEMENT_NODE and child.tagName == 'Task':
                print child.tagName
                self.task = self.parseTask(child)
                self.result['task'] = self.task
            if child.nodeType == Node.ELEMENT_NODE and child.tagName == 'Fields':
                print child.tagName
                self.field = self.parseField(child)
                self.result['field'] = self.field

    def parseTask(self, doc):
        task = {}
        if doc.tagName == 'Task':
            for child in doc.childNodes:
                if child.nodeType == Node.ELEMENT_NODE:
                    #if child.firstChild.nodeType == Node.TEXT_NODE:
                    task[child.tagName.lower()] = self.getText(child.firstChild)
                    print child.tagName, ":", self.getText(child.firstChild)
        print task
        return task


    def parseField(self, doc):
        fields = []
        if doc.tagName == 'Fields':
            for child in doc.childNodes:
                if child.nodeType == Node.ELEMENT_NODE and child.tagName == 'Field':
                    field = {}
                    for child2 in child.childNodes:
                        if child2.nodeType == Node.ELEMENT_NODE:
                            field[child2.tagName.lower()] = self.getText(child2.firstChild)
                            #print child2.tagName, ":", self.getText(child2.firstChild)
                    print field
                    fields.append(field)
        return fields

    def getText(self, node):
        if node:
            if node.nodeType == Node.TEXT_NODE:
                return node.nodeValue
        else:
            return ""

    def getConfig(self):
        return self.result



# print '-----------------------------------------'
# xml = xml('taskandfields.txt')
# config = xml.getConfig()
# websource = ''
# for line in open('websource.txt', 'r'):
#     websource += line.decode('utf-8')
# print websource
# parse_match.pickMatch(websource, config)

