void main()
{
	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather();

	weather.MissionWeather(false);    // false = use weather controller from Weather.c

	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
	weather.GetRain().Set( 0, 0, 1);
	weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

enum CmdType
{
	CAR,
	TELEPORT,
	KILL,
	SUICIDE,
	PLAYERINFO,
	SPEAK,
	ITEM,
	UNKNOWN
}

class CustomMission: MissionServer
{
	// SteamIDs of all admin players stored here
	private ref TStringArray m_admins;
		
	override void OnInit()
	{
		super.OnInit();
		
		m_admins = new TStringArray;
		m_admins.Insert("");
	}
	
	void SpawnCar(PlayerBase player, int type)
	{

		// Set car pos near player
		vector pos = player.GetPosition();
		pos[0] = pos[0] + 5;
		pos[1] = pos[1] + 5;
		pos[2] = pos[2] + 5;

		bool success = false;
		
		Car car;
		
		switch (type)
		{
			case 0:
				// Spawn and build the car
				car = GetGame().CreateObject("OffroadHatchback", pos);
				car.GetInventory().CreateAttachment("HatchbackTrunk");
				car.GetInventory().CreateAttachment("HatchbackHood");
				car.GetInventory().CreateAttachment("HatchbackDoors_CoDriver");
				car.GetInventory().CreateAttachment("HatchbackDoors_Driver");
				car.GetInventory().CreateAttachment("HatchbackWheel");
				car.GetInventory().CreateAttachment("HatchbackWheel");
				car.GetInventory().CreateAttachment("HatchbackWheel");
				car.GetInventory().CreateAttachment("HatchbackWheel");
				
				SendPlayerMessage(player, "OffroadHatchback spawned.");
				
				success = true;
				break;
				
			case 1:
				// Spawn and build the car
				car = GetGame().CreateObject("CivilianSedan_Black", pos);
				car.GetInventory().CreateAttachment("CivSedanHood_Black");
				car.GetInventory().CreateAttachment("CivSedanTrunk_Black");
				car.GetInventory().CreateAttachment("CivSedanDoors_Driver_Black");
				car.GetInventory().CreateAttachment("CivSedanDoors_CoDriver_Black");
				car.GetInventory().CreateAttachment("CivSedanDoors_BackLeft_Black");
				car.GetInventory().CreateAttachment("CivSedanDoors_BackRight_Black");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				
				SendPlayerMessage(player, "CivSedan_Black spawned.");
				
				success = true;
				break;
			
			case 2:
				break;
				
			default:
				SendPlayerMessage(player, "SpawnCar: Car type not found.");
				break;
		}
		
		// If a car was spawned, do some common configuration
		if (success) {
			// Do general car building matching all car types
			car.GetInventory().CreateAttachment("CarRadiator");
			car.GetInventory().CreateAttachment("CarBattery");
			car.GetInventory().CreateAttachment("SparkPlug");
			car.GetInventory().CreateAttachment("HeadlightH7");
			car.GetInventory().CreateAttachment("HeadlightH7");
			
			// Fill all the fluids
			car.Fill(CarFluid.FUEL, car.GetFluidCapacity(CarFluid.FUEL));
			car.Fill(CarFluid.OIL, car.GetFluidCapacity(CarFluid.OIL));
			car.Fill(CarFluid.BRAKE, car.GetFluidCapacity(CarFluid.BRAKE));
			car.Fill(CarFluid.COOLANT, car.GetFluidCapacity(CarFluid.COOLANT));
			
			// Set neutral gear
			car.GetController().ShiftTo(CarGear.NEUTRAL);
		}
	}
	
	void SetPos(PlayerBase player, string pos)
	{
		vector p = pos.ToVector();
		player.SetPosition(p);
	}
	
	void PlayerInfo(PlayerBase player)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Man plr = players.Get(i);
			
			PlayerBase p;
			Class.CastTo(p, plr);
			
			string info = "Player {" + string.ToString(i) + "}" + "  " + "Name: " + plr.GetIdentity().GetName() + "  " + "Pos: " + plr.GetPosition().ToString() + "  " + "Health: " + string.ToString(plr.GetHealth()) + "  " + "SteamID64: " + plr.GetIdentity().GetPlainId();

			SendPlayerMessage(player, info);
		}
	}
	
	bool Command(PlayerBase player, string command)
	{
		CmdType type;
		
		if (command.Contains("/car")) type = CmdType.CAR;
		else if (command.Contains("/tele")) type = CmdType.TELEPORT;
		else if (command.Contains("/kill")) type = CmdType.KILL;
		else if (command.Contains("/stats")) type = CmdType.PLAYERINFO;
		else if (command.Contains("/say")) type = CmdType.SPEAK;
		else if (command.Contains("/give")) type = CmdType.ITEM;
		else type = CmdType.UNKNOWN;
		
		if (type == CmdType.UNKNOWN) {
			SendPlayerMessage(player, "Unknown command!");
			SendPlayerMessage(player, "Commands: /car [TYPE] /tele [x y z] /kill [PLAYER] /give [ITEM] /say [MESSAGE] /stats /suicide");
			return false;
		}
		
		switch (type)
		{
			case CmdType.CAR:
				if (command.Contains("offroad")) {
					SpawnCar(player, 0);					
				}
				else if (command.Contains("sedanblack")) {
					SpawnCar(player, 1);
				}				
				else if (command.Contains("sedan")) {
					SpawnCar(player, 2);					
				}
				else {
					SendPlayerMessage(player, "CommandHandler: Car type not found.");
				}
				
				break;
				
			case CmdType.TELEPORT:
				SetPos(player, "4300 30 11200");
				break;
				
			case CmdType.PLAYERINFO:
				PlayerInfo(player);
				break;
				
			case CmdType.SPEAK:
				if (command.Length() < 8) {
					SendPlayerMessage(player, "Syntax: /say [MESSAGE]");
					return false;
				}
				
				string speakArg = command.Substring( 6, command.Length() - 6 - 1 ); // Last char is ', skip it
				SendGlobalMessage(speakArg);
				
				break;
				
			case CmdType.ITEM:
				if (command.Length() < 9) {
					SendPlayerMessage(player, "Syntax: /give [ITEM_NAME]");
					return false;
				}

				string itemArg = command.Substring( 7, command.Length() - 7 - 1 );
				
				EntityAI item = player.GetHumanInventory().GetEntityInHands();
				
				if (item) {
					SendPlayerMessage(player, "Error: hands not empty.");
					return false;
				}
				
				SendPlayerMessage(player, "Spawning item: " + itemArg);
				item = player.GetHumanInventory().CreateInHands(itemArg);
				
				if (item) {
					item.SetHealth("", "", 100);
				}
				else {
					SendPlayerMessage(player, "Could not create item.");
				}
				
				break;
				
			default:
				return false;
		}
		
		return true;
	}
	
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		// Custom user spawn handling by Steam ID (steamID64)

		const TStringArray ids = {"", "", ""};
		
		const vector custPos = "6400 0 10300".ToVector();

		string id = identity.GetPlainId();

		bool special = false;
		foreach (string x : ids) {
			if (x == id) {
				special = true;
				break;
			}
		}
		
		// Disable custom spawn handling -> uncomment line below
		special = false;

		Entity playerEnt;

		if (special) {
			playerEnt = GetGame().CreatePlayer(identity, characterName, custPos, 0, "NONE");
		}
		else {
			playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		}

		Class.CastTo(m_player, playerEnt);

		GetGame().SelectPlayer(identity, m_player);

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemTop;
		EntityAI itemEnt;
		ItemBase itemBs;

		itemTop = player.FindAttachmentBySlotName("Body"); 

		if ( itemTop )
		{
			itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
			
			if ( Class.CastTo(itemBs, itemEnt ) )
				itemBs.SetQuantity(6);
			itemEnt.SetHealth("","",100);

			itemEnt = player.GetInventory().CreateInInventory("TunaCan");
			itemEnt.SetHealth("","",100);
			
			itemEnt = itemTop.GetInventory().CreateInInventory("VitaminBottle");
			itemEnt.SetHealth("","",100);
			
			itemEnt = itemTop.GetInventory().CreateInInventory("TetracyclineAntibiotics");
			itemEnt.SetHealth("","",100);
		}
	}

	override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientRespawnEvent(identity, player);
	}
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		
		PlayerIdentity identity;
		PlayerBase player;
		
		switch(eventTypeId)
		{
		case LoginStatusEventTypeID:
			break;
			
		case ClientNewEventTypeID:
			ClientNewEventParams newParams;
			Class.CastTo(newParams, params);
			
			player = OnClientNewEvent(newParams.param1, newParams.param2, newParams.param3);
			identity = newParams.param1;
			
			if (!player)
			{
				Debug.Log("ClientNewEvent: Player is empty");
				return;
			}
			
			identity = newParams.param1;
			
			string name = identity.GetName();
			SendGlobalMessage("Player " + name + " joined.");
			
			break;
			
		case ChatMessageEventTypeID:
			ChatMessageEventParams chatParams;
			Class.CastTo(chatParams, params);
			
			// Check that input was a command (contains forward slash)
			string cmd = string.ToString(chatParams.param3);
			if ( !cmd.Contains("/") ) break;
			
			string senderName = string.ToString(chatParams.param2);
			
			// Get sender player object
			PlayerBase sender = GetPlayerByName(senderName);
			
			// If fails to get the message sender, stop
			if (!sender) {
				break;
			}
			
			// Check that player has sufficient privileges to execute commands
			if ( !IsAdmin(sender) ) {
				SendPlayerMessage(sender, "Sorry, you are not an admin!");
				break;
			}
			
			// Execute specified command
			if ( Command(sender, cmd) )
				SendPlayerMessage( sender, "Command executed successfully." );
			
			break;
		}
	}
	
	bool IsAdmin(PlayerBase player)
	{
		return m_admins.Find( player.GetIdentity().GetPlainId() ) != -1;
	}
	
	PlayerBase GetPlayerByName(string name)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		PlayerBase p;
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Man player = players.Get(i);
			Class.CastTo(p, player);

			if ( "'" + p.GetIdentity().GetName() + "'" == name && IsAdmin(p) ) {
				return p;
			}
		}
		
		// Player with given name not found
		return NULL;
	}
	
	void SendGlobalMessage(string message)	
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Man player = players.Get(i);
			if( player )
				SendPlayerMessage(player, message);
		}
	}
	
	void SendPlayerMessage(PlayerBase player, string message)	
	{
		Param1<string> Msgparam;
		Msgparam = new Param1<string>(message);
		GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}