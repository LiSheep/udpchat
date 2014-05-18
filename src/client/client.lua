--package.cpath = ";./?.so;/usr/local/lua51/lib/?.so;/usr/local/lib/lib?.so"
local myudp = require "myudp"
local wx = require "wx"

local udpserver = ""

local CLIENT_LOGIN = 100;
local CLIENT_LOGOUT = 101;
local BUTTON_SEND = 200;
local BUTTON_LOGIN = 201;
local TEXT_NAME = 400;

local app = wx.wxGetApp();
app.AppName = "udp chatroom (client)";

local frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, app:GetAppName(), 
		wx.wxPoint(100,100), wx.wxSize(410, 415),  wx.wxDEFAULT_FRAME_STYLE);

--menu 		
local menuBar = wx.wxMenuBar();
clientMenu = wx.wxMenu{
	{CLIENT_LOGIN, "&Login", "Login chatroom"},
	{CLIENT_LOGOUT, "L&ogout", "Logout chatroom"},
};
menuBar:Append(clientMenu, "&Server");
clientMenu:Enable(CLIENT_LOGOUT, false);
clientMenu:Enable(CLIENT_LOGIN, true);
frame:SetMenuBar(menuBar);

--status bar
local statusBar = frame:CreateStatusBar(1);
frame:SetStatusText("No login");

--text box
local txt_recv = wx.wxTextCtrl(frame, wx.wxID_ANY, "Welcome to udp chatroom", 
		wx.wxPoint(2, 0), wx.wxSize(400, 200), wx.wxTE_MULTILINE + wx.wxTE_READONLY);
local txt_send = wx.wxTextCtrl(frame, wx.wxID_ANY, "hello", 
		wx.wxPoint(2,200), wx.wxSize(400, 100), wx.wxTE_MULTILINE);

--button		
local btn_send = wx.wxButton(frame, BUTTON_SEND, "send", 
		wx.wxPoint(330, 305), wx.wxSize(60, 30));

--login dialog
local mdialog = wx.wxDialog(frame, wx.wxID_ANY, "User Login", 
		wx.wxDefaultPosition, wx.wxDefaultSize);
local txt_name = wx.wxTextCtrl(mdialog, TEXT_NAME, "", wx.wxDefaultPosition, wx.wxSize(130, 20));
local mainSizer = wx.wxBoxSizer(wx.wxVERTICAL)
local staticBox = wx.wxStaticBox(mdialog, wx.wxID_ANY, "User Login");
local staticBoxSizer = wx.wxStaticBoxSizer(staticBox, wx.wxVERTICAL)
local flexGridSizer = wx.wxFlexGridSizer( 1, 2, 6, 3)
flexGridSizer:Add( wx.wxStaticText( mdialog, wx.wxID_ANY, "Name:"), 0,
		wx.wxALIGN_RIGHT + wx.wxALIGN_CENTER_VERTICAL )
flexGridSizer:Add( txt_name, 0, wx.wxGROW )
local buttonSizer = wx.wxBoxSizer( wx.wxHORIZONTAL);
local subButton = wx.wxButton( mdialog, BUTTON_LOGIN, "Login");
buttonSizer:Add( subButton, 0, wx.wxALIGN_CENTER+wx.wxALL, 5 )
buttonSizer:Add( wx.wxButton( mdialog, wx.wxID_CANCEL, "Cancle"), 0, wx.wxALIGN_CENTER+wx.wxALL, 5 )
staticBoxSizer:Add(flexGridSizer, 0, wx.wxALL, 14)
mainSizer:Add(staticBoxSizer, 0, wx.wxALL, 5)
mainSizer:Add(buttonSizer, 0, wx.wxALIGN_CENTER+wx.wxALL, 5 )    
mdialog:SetSizerAndFit(mainSizer)


frame:Show(true);


wx.wxGetApp():MainLoop();

frame:Connect(CLIENT_LOGIN, wx.wxEVT_COMMAND_MENU_SELECTED, 
	function(event)
		mdialog:ShowModal();
	end
);

frame:Connect(CLIENT_LOGOUT, wx.wxEVT_COMMAND_MENU_SELECTED,
	function(event)
		wx.wxMessageBox("asd");
	end
);

frame:Connect(BUTTON_SEND, wx.wxEVT_COMMAND_BUTTON_CLICKED, 
	function(event)
		local sendStr = txt_send:GetValue();

		txt_send:Clear();
	end
);

mdialog:Connect(BUTTON_LOGIN, wx.wxEVT_COMMAND_BUTTON_CLICKED,
	function(event)
		wx.wxMessageBox("hel");

	end
);



