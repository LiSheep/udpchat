import wx
from udpsocket import Udpsocket

class LoginDialog(wx.Dialog):
	TEXT_NAME = 400;
	BUTTON_LOGIN = 201;
	

	def __init__(self, parent, id, title):
		wx.Dialog.__init__(self, parent, id, title);
		self.txt_name = wx.TextCtrl(self, self.TEXT_NAME, "", size = (130, 20));
		mainSizer = wx.BoxSizer(wx.VERTICAL);
		staticBox = wx.StaticBox(self, wx.ID_ANY, "User Login");
		staticBoxSizer = wx.StaticBoxSizer(staticBox, wx.VERTICAL)
		flexGridSizer = wx.FlexGridSizer( 1, 2, 6, 3)
		flexGridSizer.Add( wx.StaticText( self, wx.ID_ANY, "Name:"), 0,
				wx.ALIGN_RIGHT + wx.ALIGN_CENTER_VERTICAL);
		flexGridSizer.Add(self.txt_name, 0, wx.GROW);
		buttonSizer = wx.BoxSizer( wx.HORIZONTAL);
		subButton = wx.Button( self, self.BUTTON_LOGIN, "Login");
		buttonSizer.Add( subButton, 0, wx.ALIGN_CENTER+wx.ALL, 5 )
		buttonSizer.Add( wx.Button( self, wx.ID_CANCEL, "Cancle"), 0, wx.ALIGN_CENTER+wx.ALL, 5 )
		staticBoxSizer.Add(flexGridSizer, 0, wx.ALL, 14)
		mainSizer.Add(staticBoxSizer, 0, wx.ALL, 5)
		mainSizer.Add(buttonSizer, 0, wx.ALIGN_CENTER+wx.ALL, 5 )  
		self.SetSizerAndFit(mainSizer);
		subButton.Bind(wx.EVT_BUTTON, self.login)

		self.udpsocket = Udpsocket()

	def login(self, event):
		name = self.txt_name.GetValue();
		if name.strip():
			login = "hello " + self.txt_name.GetValue().decode('utf8')
			self.udpsocket.sendtoServer(login)
			self.Close()
		else:
			wx.MessageBox("name cannot null")

	
