#deploying a linux server from scratch

#example being used is ssd drive through linode


## easiest way to do this is to have two bash terminals open at the same time
### this way you aren't having to log back into your linux linode server 
### and back into your local machine , back and forth


# Side Note:
	## want to spin up a linux server quicker?
	## click on Create New Linode, Create from StackScript
	## they have a bunch, like wordpress for example


## take the ssh command and run it in linux bash 
## (could be ubuntu on windows, if you're doing that, but this HAS to be in bash)
## enter your password and you're in 
### should look something like this:
ssh root@45.79.17.21 

##first thing, update and upgrade your ubuntu with command:	

apt update && apt upgrade

##after that happens, you're going to want to set your host name
## with command:
hostnamectl set-hostname PineappleApp-server

## double checkig to make sure it's updated
##(this should return whatever you named your hostname)

hostname 


## now you have to also set his new host name in your host file
## do this with the built in editor nano
nano /etc/hosts

### what you'll see is this:

127.0.0.1       localhost

## This the only thing you add or change:
### add your custom ip address here and update the host name
45.79.17.21     PineappleApp-server

The following lines are desirable for IPv6 capable hosts
::1     localhost ip6-localhost ip6-loopback
ff02::1 ip6-allnodes
ffo2::2 ip6-allrouters


## next, you'll need to add a user with more limited access and commands 
## (this is waaay safer than just running everything as admin)

adduser john

## add a password for the user, and fill out anything else for it 



## then you can add their privilages 
##(sudo if you don't remember/didn't know if for root commands until restricted)
adduser john sudo


## then you want to log out, best practice to not be logged into root
## you can log back into the ssh IP address by just replacing the root username
ssh john@45.79.17.21




## next you'll want to set up SSH key authentication so you can log in without a password
## and it's more secure because it's key-based authentication
## make a .ssh directory inside the linux machine (linode in this case)
mkdir .ssh

## now on your local machine (not the linode)
# generating a key 
## logout of your session with the linux server (or just have two terminals open up)

## this generates the key and the "-b 4096" keeps it more secure
ssh-keygen -b 4096

## this prints:
### Generating public/private rsa key pair.
### Enter file in which to save the key (C:\Users\john/.ssh/id_rsa):
## you can leave the default if you'd like or make a new file name/location
## it'll prompt you for a passcode, do this if you'd want it more secure, 
## but it's not neccesary

## A public key will be generated and look something like 
## this if you kept the defualt settings 

Generating public/private rsa key pair.
Enter file in which to save the key (C:/Users/john/.ssh/id_rsa):
Created directory 'C:/Users/john/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /home/john/.ssh/id_rsa.
Your public key has been saved in /home/john/.ssh/id_rsa.pub.
The key fingerprint is:
SHA256:uAR18w/iogxrdppO7q44FiDIeNh8apmxdACA6JXqOg4 john@DESKTOP-49Q9VPH
The key's randomart image is:
+---[RSA 4096]----+
|     .           |
|    + +          |
|     = o. .      |
|      . .o .     |
|     .  S.. * o  |
|.. . ...  .= E o |
|=.o.= .. ...o +o.|
|.*++.o.  ++ ..o..|
|oo=o.o  .+=o .  o|
+----[SHA256]-----+




## now we need to move our public key to the server
## go back into bash and copy the public rsa key to the server 
### scp might stand for secure copy, 
### and the tidla (~) is for the home directory location, then the .ssh, 
### then a brand new file named "authorized_keys" 
###(you don't need file extensions declared with linux)

scp ~/.ssh/id_rsa.pub john@45.79.17.21:~/.ssh/authorized_keys


## now back in your server, make sure the key is there 
 john@PineappleApp-server:~$ ls .ssh
## this should show:
 authorized_keys

## now for security, setting the permissions for the ssh directory 
## so that the owner has read, write, and execution permissions for any file
## and any owners of the files just have read, write 
### sudo chmod is changing the permissions command
 sudo chmod 700 ~/.ssh/
 sudo chmod 600 ~/.ssh/*
		 
		###  side note about these linux restrictions:
			#### the first digit of permission is for the user 
			####  the second digit is for the read, write and execute
			####  the third digit is for everyone else 

## okay, now logout and log back in, it should just do it without needed a passcode 


## now that we know our ssh keys are working (allowing no passcode to login)
## we need to disallow root logins via ssh
 sudo nano /etc/ssh/sshd_config
 
 ## this opens a huge file, for our use we just want to change two things:
 ### First:
 PermitRootLogin no
 ### this is because since we have a user with limited access we technically don't need 
 ### to login as root (throws off hackers since they might try to login via root)
 
 ### Second:
 PasswordAuthentication no
 ### since we have our ssh keys working, sometimes hackers with try to brutforce
 ### a password 
 
 
## after you save the sshd_config file, we'll want to restart our ssh server 
sudo systemctl restart sshd

## now install an "uncomplicated fire wall"
 sudo apt install ufw
 
 
## now set up some rules for the firewall :
### allowing outgoing traffic
sudo ufw default allow outgoing

### denying incoming traffic 
sudo ufw default deny incoming

## TO MAKE SURE YOU DON'T GET LOCKED OUT OF YOUR SERVER
### allow incoming into certain ports 
### like ssh, and http and any port we want to access outside of our server 

## allows us to use ssh to login 
sudo ufw allow ssh

## since flask uses a port 5000 to debug, we'll need to allow it for now 

sudo ufw allow 5000

# next we need to impliment all of those changes, be very careful here 
## (meaning make sure you at least allow ssh login, if that's what you're using)
sudo ufw enable
Command may disrupt existing ssh connections. Proceed with operation (y|n)? y
Firewall is active and enabled on system startup




## getting stressed that you might have forgotten something?
## check the status:
sudo ufw status
Status: active

To                         Action      From
--                         ------      ----
22/tcp                     ALLOW       Anywhere
5000                       ALLOW       Anywhere
22/tcp (v6)                ALLOW       Anywhere (v6)
5000 (v6)                  ALLOW       Anywhere (v6)

### port 22 is ssh







