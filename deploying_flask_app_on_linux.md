# deploying flask app on a linux server (afer you set up the users and security)

## easiest way to do this is to have two bash terminals open at the same time
## and a local terminal inside your flask virtual environment 
## (if you're new, this might sound scary, but Hey! Welcome to computer programming)
### this way you aren't having to log back into your linux linode server 
### and back into your local machine , back and forth


## just clone it from github:
git clone "repository"

## filezilla?
### just copy it over to your server 


## want to do it straight from bash?

### go back into your virtual environtment (on your local machine)
### and save the requirements you have, to do this use pip
pip freeze

### copy and paste the requiremets into a file named requirements.txt in that folder
### (if you're on linux or mac, you can type pip freeze > requirements.txt and it'll
### create the file for you)

## this copies the entire folder to the server (make sure the path is correct
scp -r PineappleAppFlask/PineappleApp_Main john@45.79.17.21:~/


### being logged into your linux system (linode), you can double check to make sure everything is there by 
### checking "ls"

## you always want to have a virtual environment for your flask app 
## this way if it fails it'll be easier to fix and less of a change of the computer having issues

## so install python3 
sudo apt install python3-pip

## install python virtual environment
 sudo apt install python3-venv
 
## create a virtual environment inside of your flask app
pyhton3 -m venv PineappleApp_Main/venv

## you can double check the venv is there by
cd PineappleApp_Main
ls 
## then activate the virtual environment
source venv/bin/activate

## now for the installing of the requirements:
pip install -r requirements.txt


## how to set up environmental variables
## create a app_config.json file in the /etc/ folder
## (if you have more than one flask app on a server, give it a unique name)
## then edit that .json file and add 
## all of your environmental variables (from your local machine) like so:
	#### SIDE NOTE: NOT ALL OF THESE HAVE TO MATCH UP, FOR INSTANCE, MAYBE YOU DON'T HAVE A RESET_EMAIL_PASSWORD

sudo nano /etc/pineapple_config.json

{

        "SECRET_KEY" : "value",
        "SQLALCHEMY_DATABASE_URI" : "value",
        "RESET_EMAIL_USERNAME" : "value@gmail.com",
        "RESET_EMAIL_PASSWORD" : "value"

}


## next edit whatever file you have your environmental variables stored 
## (or your sensitive variables stored) in this case, it's in the __init__ file 

sudo nano PineappleApp/__init__.py

		## there you'll want to import json and open the file with json like so:

		import json
		...
		#### opening json config file
		with open('/etc/pineapple_config.json') as config_file:
		#### when doing json.load() on a json file, python makes it a dictionary
				config =  json.load(config_file)

		#### the syntax is config.get() since it's a dictionary we just call the keys
		app.config['SQLALCHEMY_DATABASE_URI'] = config.get('SQLALCHEMY_DATABASE_URI')


## next you want to export your flask app 
export FLASK_APP=run.py

## but remember it's still in debug mode and that's okay since we want 
## to test it out. so run it on your "local" host (the ip address for the linux server) in port 5000
flask run --host=0.0.0.0
### side note, this ^^ is the syntax but when you look it up online, 
### you'll use whatever ip address for the linux server you're running


## okay if everything is working then we'll need to get it off of development server 
## we can do this with nginx (engine x) and g-unicorn

## install nginx with linux 
sudo apt install nginx 

## install gunicorn with pip (just make sure it's in your virtual environment
pip install gunicorn

## SIDE NOTE: nginx is going to handle the static files, gunicorn is going to handle the python code
### to do this we need to remove some of the defaults and add some rules

## removing nginx default settings 
sudo rm /etc/nginx/sites-enabled/default

## create a new file for your rules
sudo nano /etc/nginx/sites-enabled/pineapple_app


## this is what you'll create in that file:
### location /static {} means you're handing all of the static files 
### location /  <- means if you go to our ip address
### proxy_pass means you're fowarding all of the python files to be handled by gunicorn
### local host is our IP address (since we're running this on a local server in a different state)
### and port 8000 since that's where gunicorn typically runs python code
### the rest of the code is telling nginx to pass it onto gunicorn 

server {
        listen 80;
        server_name 45.79.17.21;

        location /static {
                alias /home/john/PeoplePrototype/PineappleApp/static;

        }
		
		location / {
				proxy_pass http://localhost:8000;
				include /etc/nginx/proxy_params;
				proxy_redirect off;
		}

}

## next you'll need to allow port 8000 through the firewall
 sudo ufw allow http/tcp
 
## and since we're done with testing, delete the port 5000
sudo ufw delete allow 5000

## make sure all of this is enabled 
sudo ufw enable

## restart your server 
sudo systemctl restart nginx 

### but we're not done just yet! we still don't have gunicorn set up yet 
## fairly simple set up
### number of workers formula == (2 x number of cores on the machine) + 1
### need to see how many cores you have on your linux machine?
nproc --all

## gunicorn code to run the app (with the basic if __name__=="__main__"
## or the better app = create_app())
## 	(JUST TESTING IT TO MAKE SURE IT'S ACTUALLY UP AND RUNNING, NOT LONG TERM USE)
gunicorn -w 3 run:app


### BUT THERE'S MORE!!!
## if you close your remote connection to the server, it'll actually crash it 
## what we need is something that will constantly monitor gunicorn and 
## start it and restart it if it crashes 
## we'll use some software called "supervisor" which is easy on linux 
### once that is installed, we'll just need to set up a config file for supervisior 

### creating file "pineapple_app_prototype.conf"
 sudo nano /etc/supervisor/conf.d/pineapple_app.conf
 
 
 
### in that file create this:

[program:pineapple_app_prototype]
directory=/home/john/PeoplePrototype
command=/home/john/PeoplePrototype/venv/bin/gunicorn -w 3 run:app
user=john
autostart=true
autorestart=true
stopasgroup=true
killasgroup=true
stderr_logfile=/var/log/PineappleApp/PineappleApp.err.log
stdout_logfile=/var/log/PineappleApp/PineappleApp.out.log


### next we need to create the PineappleApp.err.log file that we're referencing
#### the -p means "create anyfolder that isn't already there along the way"
sudo mkdir -p /var/log/PineappleApp

sudo touch /var/log/PineappleApp/PineappleApp.err.log
sudo touch /var/log/PineappleApp/PineappleApp.out.log
sudo supervisorctl reload

## then boom! supervisior is up and running and you've got yourself a fucking app



			### SIDE NOTE ABOUT NGINX
			### - won't accept client uploads too big (around 2mb)
			### to change this: 
			sudo nano /etc/nginx/nginx.conf
			### and add under main settings (last thing is typically types_hash_max_size)
			#### increased it to 5megabytes
			client_max_body_size 5M; 
			### make sure to restart nginx for settigs to be updated
			sudo systemctl restart nginx




			## want to completely delete your server and start from scratch? (on linode)
			## click the ... settings next to the server
			## then on Settings tab, there's a Delete Linode 

			## want to spin up a linux server quicker?
			## click on Create New Linode, Create from StackScript
			## they have a bunch, like wordpress for example