
# A very simple Bottle Hello World app for you to get started with...
from bottle import default_app, route,response,template,post,request

import insta
import json


pth='mysite/'








urls = [
  '/get_json/(.*)/(.*)', 'get_json',
  '/data/(.*)/(.*)','images',
  '/login','login',
  '/like/(.*)/(.*)','like',
  '/settings/(.*)/(.*.)','settings'





]



@route('/settings/<sets>/<is_new>')
def settings(sets,is_new):
    response.content_type = 'text/html; charset=utf-8'
    response.headers['Access-Control-Allow-Origin'] = '*'
    return template('settings', sets=sets,is_new=int(is_new))

@post('/login')
def login():
    response.headers['Access-Control-Allow-Origin'] = '*'
    login = request.forms.get('login')
    passw = request.forms.get('passw')
    if l := insta.login(login, passw):
        return json.dumps({'token':l,'login':True})
    else:
        return json.dumps({'token':'','login':False})

@route('/like/<token>/<numb>')
def like(token,numb):
    insta.like(token,numb)

@route('/get_json/<token>/<numb>')
def get_json(token,numb):
    response.headers['Access-Control-Allow-Origin'] = '*'
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

@route('/data/<token>/<numb>')
def data(token,numb):
    response.headers['Access-Control-Allow-Origin'] = '*'
    response.content_type = 'image/png'
    return open(f'{pth}data/{token}/{str(numb)}.png', 'rb').read()




application = default_app()

