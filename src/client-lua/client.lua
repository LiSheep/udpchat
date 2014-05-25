--package.cpath = ";./?.so;/usr/local/lua51/lib/?.so;/usr/local/lib/lib?.so"
local myudp = require "myudp"
local wx = require "wx"
local cjson  = require"cjson"
local thread = require "thread"

local serverip = "115.28.83.115";
--local serverip = "222.26.224.56";
local serverport = 19560;

local CLIENT_LOGIN = 100;
local CLIENT_LOGOUT = 101;
local BUTTON_SEND = 200;
local BUTTON_LOGIN = 201;
local BUTTON_REFRESH = 202;
local TEXT_NAME = 400;
local LIST_NAME = 500;

local userinfo = {};
local myname = "";

local chat_addr;
local chat_port;

local app = wx.wxGetApp();
app.AppName = "udp chatroom (client)";

local frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, app:GetAppName(), 
		wx.wxPoint(100,100), wx.wxSize(560, 415),  wx.wxDEFAULT_FRAME_STYLE);

--menu 		
local menuBar = wx.wxMenuBar();
clientMenu = wx.wxMenu{
	{CLIENT_LOGIN, "&Login", "Login chatroom"},
	{CLIENT_LOGOUT, "L&ogout", "Logout chatroom"},
};
menuBar:Append(clientMenu, "&Server");
frame:SetMenuBar(menuBar);

function menuChange(login, logout)
	clientMenu:Enable(CLIENT_LOGOUT, login);
	clientMenu:Enable(CLIENT_LOGIN, logout);
end

menuChange(false, true);

--status bar
local statusBar = frame:CreateStatusBar(1);
frame:SetStatusText("No login");

--text box
local txt_recv = wx.wxTextCtrl(frame, wx.wxID_ANY, "", 
		wx.wxPoint(2, 0), wx.wxSize(400, 200), wx.wxTE_MULTILINE + wx.wxTE_READONLY);
local txt_send = wx.wxTextCtrl(frame, wx.wxID_ANY, "hello", 
		wx.wxPoint(2,200), wx.wxSize(400, 100), wx.wxTE_MULTILINE);

--button		
local btn_send = wx.wxButton(frame, BUTTON_SEND, "send", 
		wx.wxPoint(330, 315), wx.wxSize(60, 30));

local btn_refresh = wx.wxButton(frame, BUTTON_REFRESH, "refresh",
		wx.wxPoint(410, 315), wx.wxSize(60, 30));
--listbox
local choices = {};
local listName= wx.wxListBox(frame, LIST_NAME, wx.wxPoint(410,0), wx.wxSize(130,300), choices);

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

--frame 
frame:Show(true);

--udp
local conn = myudp.new();
conn:bind(19999); --just test
--recv data deal
function deal_ser(data)
	choices = {};
	userinfo = cjson.decode(data)
	for _, v in pairs(userinfo) do
		table.insert(choices, v["name"]);
	end
	listName:InsertItems(choices, 0);
end

function deal_cli(data)
	txt_recv:AppendText(data.."\n");
end

--thread
function recv()
	a = 1;
	while a do
		print("begin");
		local recvstr = conn:recvfrom();
		local recvtype = string.sub(recvstr, 0, 3);
		if recvtype == "ser" then
			deal_ser(string.sub(recvstr, 4));
		else
			print("from client: ");
			deal_cli(recvstr);
		end
	end
end

local recv_thread = thread.create(recv);

--action
frame:Connect(CLIENT_LOGIN, wx.wxEVT_COMMAND_MENU_SELECTED, 
	function(event)
		mdialog:ShowModal();
	end
);

frame:Connect(CLIENT_LOGOUT, wx.wxEVT_COMMAND_MENU_SELECTED,
	function(event)
		conn:sendto(serverip, serverport, "bye ");
	end
);

frame:Connect(BUTTON_SEND, wx.wxEVT_COMMAND_BUTTON_CLICKED, 
	function(event)
		local sendStr = txt_send:GetValue();
		if chat_addr ~= nil then
			txt_recv:AppendText("me: "..sendStr.."\n");
			conn:sendto(serverip, serverport, myname..": "..sendStr);
			txt_send:Clear();
		else
			wx.wxMessageBox("choice one to send");
		end
	end
);

frame:Connect(BUTTON_REFRESH, wx.wxEVT_COMMAND_BUTTON_CLICKED, 
	function(event)
		if not listName:IsEmpty() then
			listName:Clear();
		end
		conn:sendto(serverip, serverport, "get ");
	end
);

frame:Connect(LIST_NAME, wx.wxEVT_COMMAND_LISTBOX_SELECTED,
	function(event)
		local select_index = event:GetSelection();
		local curr_index = 0;
		for _, v in pairs(userinfo) do
			if select_index == curr_index then
				chat_addr = v["addr"];
				chat_port = v["port"];
				break;
			end
			curr_index = curr_index + 1;
		end
	end
);

mdialog:Connect(BUTTON_LOGIN, wx.wxEVT_COMMAND_BUTTON_CLICKED,
	function(event)
		local login = "hello "..txt_name:GetValue();	--send login info
		conn:sendto(serverip, serverport, login); 
		conn:recvfrom();

		--start thread
		recv_thread:resume();
		myname = txt_name:GetValue()
		frame:SetStatusText("Logined: "..myname);
		mdialog:EndModal(0);
	end
);


wx.wxGetApp():MainLoop();
