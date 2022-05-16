
import requests
import time
import random
import json

import urllib
import os
from StringIO import StringIO
from PIL import Image
from datetime import datetime
pth='mysite/data/'


def pretty_date(time=False):


	now = datetime.now()
	if type(time) is int:
		diff = now - datetime.fromtimestamp(time)
	elif isinstance(time,datetime):
		diff = now - time
	elif not time:
		diff = now - now
	second_diff = diff.seconds
	day_diff = diff.days

	if day_diff < 0:
		return ''

	if day_diff == 0:
		if second_diff < 10:
			return "just now"
		if second_diff < 60:
			return f"{str(second_diff)} seconds ago"
		if second_diff < 120:
			return "a minute ago"
		if second_diff < 3600:
			return f"{str(second_diff / 60)} minutes ago"
		if second_diff < 7200:
			return "an hour ago"
		if second_diff < 86400:
			return f"{str(second_diff / 3600)} hours ago"
	if day_diff == 1:
		return "Yesterday"
	if day_diff < 7:
		return f"{str(day_diff)} days ago"
	if day_diff < 31:
		return f"{str(day_diff / 7)} weeks ago"
	if day_diff < 365:
		return f"{str(day_diff / 30)} months ago"
	return f"{str(day_diff / 365)} years ago"


def login(username,password):
	s = requests.Session()
	s.cookies.update({'sessionid': '', 'mid': '', 'ig_pr': '1','ig_vw': '1920', 'csrftoken': '','s_network': '', 'ds_user_id': ''})
	login_post = {'username': username,'password': password}
	s.headers.update({'Accept-Encoding': 'gzip, deflate',
	'Accept-Language': 'ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4',
	'Connection': 'keep-alive',
	'Content-Length': '0',
	'Host': 'www.instagram.com',
	'Origin': 'https://www.instagram.com',
	'Referer': 'https://www.instagram.com/',
	'User-Agent': ("Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 "
	"(KHTML, like Gecko) Chrome/48.0.2564.103 Safari/537.36"),
	'X-Instagram-AJAX': '1',
	'X-Requested-With': 'XMLHttpRequest'})
	r = s.get('https://www.instagram.com/')
	s.headers.update({'X-CSRFToken': r.cookies['csrftoken']})
	time.sleep(3 * random.random())
	login = s.post('https://www.instagram.com/accounts/login/ajax/', login_post,
			allow_redirects=True)
	s.headers.update({'X-CSRFToken': login.cookies['csrftoken']})
	csrftoken = login.cookies['csrftoken']
	time.sleep(3 * random.random())

	if login.status_code == 200:
		r = s.get('https://www.instagram.com/')
		finder = r.text
		if username not in finder :return False
		d = {x: login.cookies[x] for x in login.cookies.keys()}
		try:
			os.mkdir(pth+csrftoken)
		except:
			pass
		open(pth+csrftoken+'/'+'token.txt','w').write(repr(d))
		return csrftoken
	else:
		return False

def like(token,lid):
	c=eval(open(pth+token+'/'+'token.txt').read())
	s = requests.Session()
	s.cookies.update(c)
	s.headers.update({'Accept-Encoding': 'gzip, deflate',
	'Accept-Language': 'ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4',
	'Connection': 'keep-alive',
	'Content-Length': '0',
	'Host': 'www.instagram.com',
	'Origin': 'https://www.instagram.com',
	'Referer': 'https://www.instagram.com/',
	'User-Agent': ("Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 "
	"(KHTML, like Gecko) Chrome/48.0.2564.103 Safari/537.36"),
	'X-Instagram-AJAX': '1',
	'X-Requested-With': 'XMLHttpRequest'})
	s.headers.update({'content-type':'application/x-www-form-urlencoded'})
	s.headers.update({'path': f'/web/likes/{str(lid)}/like/'})
	s.headers.update({'x-csrftoken':c['csrftoken']})
	re = s.post(f'https://www.instagram.com/web/likes/{lid}/like/')



def get_content(token,ln):
	all_list=[]
	c=eval(open(pth+token+'/'+'token.txt').read())
	s = requests.Session()
	s.cookies.update(c)
	s.headers.update({'Accept-Encoding': 'gzip, deflate',
				'Accept-Language': 'ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4',
				'Connection': 'keep-alive',
				'Content-Length': '0',
				'Host': 'www.instagram.com',
				'Origin': 'https://www.instagram.com',
				'Referer': 'https://www.instagram.com/',
				'User-Agent': ("Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 "
           	  	   "(KHTML, like Gecko) Chrome/48.0.2564.103 Safari/537.36"),
				'X-Instagram-AJAX': '1',
				'X-Requested-With': 'XMLHttpRequest'})

	rez=s.get('http://www.instagram.com/').text

	data=rez.split('<script type="text/javascript">window._sharedData = ')[1].split('</script>')[0]
	j=json.loads(data[:-1])








	for n,x in enumerate(j['entry_data']['FeedPage'][0]['feed']['media']['nodes'][:ln]):
		im=x['display_src'].replace('15/e35/','15/s120x120/e35/')


		try:caption=x['caption'].encode('utf-8')
		except:caption=''
		uname=x['owner']['username']
		likes=x['likes']['count']
		lid=x['id']
		ulike=x['likes']['viewer_has_liked']
		d=pretty_date(x['date'])

		im=urllib.urlopen(im).read()
		i=StringIO()
		i.write(im)
		i.seek(0)
		img=Image.open(i)
		img.thumbnail((120,120),Image.ANTIALIAS)

		if img.size[0]>img.size[1]:
			t=Image.new('RGB',(img.size[0],img.size[0]),'white')
			t.paste(img,(0,img.size[0]/2-img.size[1]/2))
			img=t
		elif img.size[0]<img.size[1]:
			t=Image.new('RGB',(img.size[1],img.size[1]),'white')
			t.paste(img,(img.size[1]/2-img.size[0]/2,0))
			img=t



		img=img.convert(mode='P', palette='ADAPTIVE', colors=64,dither=Image.FLOYDSTEINBERG)
		img.save(pth+token+'/'+str(n)+'.png')




		#s.headers.update({'content-type':'application/x-www-form-urlencoded'})
		#s.headers.update({'path':'/web/likes/'+str(lid)+'/like/'})
		#s.headers.update({'x-csrftoken':c['csrftoken']})
		#re=s.post('https://www.instagram.com/web/likes/%s/like/'%(lid))



		all_list.append([caption,uname,likes,lid,ulike,d])
	return all_list



















