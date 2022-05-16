#!/usr/bin/python 
#coding:utf-8

import os
import sys
import web
import time
from StringIO import StringIO
from PIL import Image,ImageOps,ImageDraw
import json
import shutil




os.environ['PYTHON_EGG_CACHE'] = '/tmp'
#os.chdir('/home/kaist/data/www/101watermark.igor-k.ru/')

pth='/var/www/kaist/data/www/instapp.tk/'
#pth=''
sys.path.append(pth)
os.chdir(pth)
import insta
#sys.path.append('/home/kaist/data/www/igor-k.ru/mod')
www='http://localhost:8080/'
#www='http://m.zalomskij.ru/'
#www='http://shootandview.zalomskij.ru/'
#www='http://tortvkzn.ru/'



#db=web.database(dbn='sqlite',db=pth+'main.db')





urls = [
  '/get_json/(.*)/(.*)', 'get_json',
  '/data/(.*)/(.*)','images',
  '/login','login',
  '/like/(.*)/(.*)','like',
  '/settings/(.*)/(.*.)','settings'





]


render = web.template.render(f'{pth}templ/', cache=True)


class settings:
	def GET(self,sets,is_new):
		web.header('Content-Type','text/html; charset=utf-8', unique=True)
		web.header('Access-Control-Allow-Origin', '*')				
		return render.settings(sets=int(sets),is_new=int(is_new))
class login:
	def POST(self):
		web.header('Access-Control-Allow-Origin', '*')
		i=web.input()
		if l := insta.login(i.login, i.passw):
			return json.dumps({'token':l,'login':True})
		else:
			return json.dumps({'token':'','login':False})			
class like:
	def GET(self,token,numb):
		insta.like(token,numb)
class get_json:
	def GET(self,token,numb):
		web.header('Access-Control-Allow-Origin', '*')
		try:
			rez=insta.get_content(token,int(numb))
			likes=[]
			names=[]
			texts=[]
			lids=[]
			ulikes=[]
			dates=[]
			for x in rez:
				texts.append(x[0])
				names.append(x[1])
				likes.append(x[2])
				lids.append(x[3])
				ulikes.append(x[4])	
				dates.append(x[5])											

			d = {
			    'likes': likes,
			    'all': len(likes),
			    'names': names,
			    'texts': texts,
			    'lids': lids,
			    'ulikes': ulikes,
			    'dates': dates,
			}
			return json.dumps(d)
		except:
			return json.dumps({'all':0})

class images:
	def GET(self,token,numb):
		web.header('Access-Control-Allow-Origin', '*')
		web.header('Content-type','image/png')
		return open(f'{pth}data/{token}/{str(numb)}.png', 'rb').read()
		

app = web.application(urls, globals())

	
		




            









app.internalerror=web.debugerror
application = app.wsgifunc()


if __name__ == "__main__":
    app.run()
