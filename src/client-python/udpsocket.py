import socket

def singleton(cls, *args, **kw):  
    instances = {}  
    def _singleton():  
        if cls not in instances:  
            instances[cls] = cls(*args, **kw)  
        return instances[cls]  
    return _singleton 

@singleton
class Udpsocket:
	#serverip = "115.28.83.115"
	serverip = "172.18.99.196"
	serverport = 19560
	myip = "0.0.0.0"
	myport = 10000

	def __init__(self):
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
		self.socket.bind(('', self.myport))

	def sendtoServer(self, msg):
		self.socket.sendto(msg, (self.serverip, self.serverport))

	def sendtoOther(self, addr, port, msg):
		self.socket.sendto(msg, (addr, port))

	def recvfrom(self):
		return self.socket.recvfrom(self.myport)