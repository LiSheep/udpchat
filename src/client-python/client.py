import wx
import threading
import time
import json
import sys  

from loginDialog import LoginDialog
from udpsocket import Udpsocket

reload(sys)  
sys.setdefaultencoding('utf8') 

def call_after(func): 
    def _wrapper(*args, **kwargs): 
        return wx.CallAfter(func, *args, **kwargs) 
    return _wrapper

class ClientFrame(wx.Frame):
	CLIENT_LOGIN = 100;
	CLIENT_LOGOUT = 101;
	BUTTON_SEND = 200;
	BUTTON_LOGIN = 201;
	BUTTON_REFRESH = 202;
	LIST_NAME = 500;

	curr_addr = ""
	curr_port = 0
	myname = ""

	def __init__(self, parent, title, pos, size):
		wx.Frame.__init__(self, parent, title=title, size=size, pos=pos)
		
		#status bar
		statusBar = self.CreateStatusBar()

		#menu
		self.clientMenu = wx.Menu()
		self.clientMenu.Append(self.CLIENT_LOGIN, "&Login", "Login chatroom")
		self.clientMenu.Append(self.CLIENT_LOGOUT, "L&ogout", "Logout chatroom")	

		menuBar = wx.MenuBar()
		menuBar.Append(self.clientMenu, "&Server")
		self.SetMenuBar(menuBar)
		self.change_status_bar(False)

		self.Bind(wx.EVT_MENU, self.menu_login_action, id=self.CLIENT_LOGIN)

		#text box
		self.txt_recv = wx.TextCtrl(self, pos=(2, 0), size=(400, 200), style=wx.TE_MULTILINE + wx.TE_READONLY)
		self.txt_send = wx.TextCtrl(self, pos=(2, 210), size=(400, 130), style=wx.TE_MULTILINE)

		#button
		self.btn_send = wx.Button(self, self.BUTTON_SEND, "send", (330, 345), (60, 30))
		#btn_send = wx.Button(self, self.BUTTON_REFRESH, "send", (420, 345), (60, 30))

		self.btn_send.Bind(wx.EVT_BUTTON, self.btn_send_action, id=self.BUTTON_SEND)

		#listbox
		self.choices = [];
		self.listName= wx.ListBox(self, self.LIST_NAME, (410,0), (130,341), []);

		self.listName.Bind(wx.EVT_LISTBOX, self.listName_action)

		#login dialog
		self.mdialog = LoginDialog(self, wx.ID_ANY, "Login")		

		#udp
		self.udpsocket = Udpsocket()

		#thread
		threading.Thread(target = self.thread_recv_worker, args=(self.udpsocket, )).start()
		threading.Thread(target = self.thread_send_worker, args=(self.udpsocket, )).start()


	def change_status_bar(self, islogin):
		self.clientMenu.Enable(self.CLIENT_LOGIN, not islogin)
		self.clientMenu.Enable(self.CLIENT_LOGOUT, islogin)
		if(not islogin):
			self.SetStatusText("No Login")

	def menu_login_action(self, event):
		self.mdialog.ShowModal()

	def btn_send_action(self, event):
		msg = self.txt_send.GetValue()
		if self.curr_addr.strip():
			self.txt_recv.AppendText("me: " + msg)
			self.udpsocket.sendtoOther(self.curr_addr, self.curr_port, self.myname + ": " + msg)
			self.txt_send.Clear()
		else:
			print("no send")

	def listName_action(self, event):
		select_index = event.GetSelection()
		self.curr_addr = self.choices[select_index]["addr"]
		self.curr_port = self.choices[select_index]["port"]
	
	def thread_recv_worker(self, udpsocket):
		while(True):
			recv = udpsocket.recvfrom()
			msg = recv[0]
			if msg.find("ser") != -1:
				me = msg[3:]
				self.deal_ser(me)
			else:
				self.deal_cli(msg)

	def thread_send_worker(self, udpsocket):
		while (True):
			time.sleep(5)
			udpsocket.sendtoServer("get ")

	@call_after
	def deal_ser(self, data):
		print(data)

		self.listName.Clear()
		namelist = []
		myjdata = json.loads(data)
		
		if myjdata["type"] == "hello":
			self.SetStatusText("Login " + myjdata["name"])
			self.change_status_bar(True)
			self.myname = myjdata["name"]
		elif myjdata["type"] == "bye":
			self.change_status_bar(false)
			self.myname = ""
		else:
			for d in myjdata["data"]:
				self.choices.append(d)
				namelist.append(d["name"])
			self.listName.InsertItems(namelist, 0)
			
	
	@call_after
	def deal_cli(self, data):
		self.txt_recv.AppendText(data + "\n");

#main

app = wx.App()
frame = ClientFrame(None,"udp chatroom (client)", (100,100), (564, 465))

frame.Show(True)
app.MainLoop()
