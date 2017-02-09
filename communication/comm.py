import base64
import paramiko
import threading
from scp import SCPClient

hostname = "abel.uio.no"
username = "anderhaf"
password = "blank"

#ssh = paramiko.SSHssh()
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.load_system_host_keys()
ssh.connect(hostname, username=username, password=password)
scp = SCPClient(ssh.get_transport())

def doThing():
	print "Will get things"
	stdin, stdout, stderr = ssh.exec_command('cat client.json')

	print "Did get things. Will copy file"
	scp.get('dump.bin')
	print "Did copy file"
	threading.Timer(1, doThing).start()


doThing()
#client.close()
