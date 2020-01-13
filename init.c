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

class CustomMission: MissionServer
{
	// SteamIDs of all admin players stored here
	private ref TStringArray m_admins;
		
	override void OnInit()
	{
		super.OnInit();
		
		// Insert admin IDs here
		m_admins = new TStringArray;
		m_admins.Insert("123");
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
	
	void SafeSetPos(PlayerBase player, string pos)
	{
		// Safe conversion
		vector p = pos.ToVector();
		
		if (p) {
			player.SetPosition(p);
			return;
		}
		
		SendPlayerMessage(player, "Invalid coordinates.");
	}
	
	void PlayerInfo(PlayerBase player)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		PlayerBase p;
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Class.CastTo(p, players.Get(i));
			
			string info = "Player {" + string.ToString(i) + "}" + "  " + "Name: " + p.GetIdentity().GetName() + "  " + "Pos: " + p.GetPosition().ToString() + "  " + "Health: " + string.ToString(p.GetHealth()) + "  " + "SteamID64: " + p.GetIdentity().GetPlainId();

			SendPlayerMessage(player, info);
		}
	}
	
	bool Command(PlayerBase player, string command)
	{
		const string helpMsg = "Commands: /help /car /warp /kill /give /say /stats /suicide /here /there";
		
		// Split command message into args
		TStringArray args = new TStringArray;
		MySplit(command, " ", args);
		
		switch (args[0])
		{
			case "/car":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /car [TYPE] - Spawn a vehicle near self");
					return false;
				}
				if (args[1] == "offroad") {
					SpawnCar(player, 0);					
				}
				else if (args[1] == "sedanblack") {
					SpawnCar(player, 1);
				}				
				else if (args[1] == "sedan") {
					SpawnCar(player, 2);					
				}
				else {
					SendPlayerMessage(player, "CommandHandler: Car type not found.");
				}
				break;
				
			case "/warp":
				if ( args.Count() < 4 ) {
					SendPlayerMessage(player, "Syntax: /warp [X] [Y] [Z] - Teleport to x y z");
					return false;
				}
				string pos = args[1] + " " + args[2] + " " + args[3];
				
				SendPlayerMessage(player, pos);
				
				SafeSetPos(player, pos);
				break;
				
			case "/stats":
				if ( args.Count() != 1 ) {
					SendPlayerMessage(player, "Syntax: /stats");
					return false;
				}
				PlayerInfo(player);
				break;
				
			case "/say":
				if ( args.Count() <= 1 ) {
					SendPlayerMessage(player, "Syntax: /say [MESSAGE] - Global announcement to all players");
					return false;
				}
				
				// Form the message string and send to all players
				string msg = "";
				
				args.Remove(0);
				foreach (string i : args) {
					msg = msg + i + " ";
				}
				
				SendGlobalMessage(msg);
				break;
				
			case "/give":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /give [ITEM_NAME] - Gives self an item to hands");
					return false;
				}
				
				EntityAI item = player.GetHumanInventory().GetEntityInHands();
				
				if (item) {
					SendPlayerMessage(player, "Error: hands not empty.");
					return false;
				}
				
				SendPlayerMessage(player, "Spawning item: " + args[1]);
				item = player.GetHumanInventory().CreateInHands(args[1]);
				
				if (item) {
					item.SetHealth("", "", 100);
				}
				else {
					SendPlayerMessage(player, "Could not create item.");
				}
				
				break;
				
			case "/here":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /here [player] - Moves player to self");
					return false;
				}				
				TeleportPlayer(GetPlayerByAny(args[1]), player);
				break;
				
			case "/there":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /there [player] - Moves self to player");
					return false;
				}
				TeleportPlayer(player, GetPlayerByAny(args[1]));
				break;
				
			case "/suicide":
				if ( args.Count() != 1 ) {
					SendPlayerMessage(player, "Syntax: /suicide - Commit a suicide");
					return false;
				}
				// Use SteamID here for sake of certainty
				// We dont want to kill a player that happens to have the same name
				KillPlayer( player.GetIdentity().GetPlainId() );
				break;

			case "/kill":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /kill [PLAYER] - Kills a player by name or id");
					return false;
				}
				
				if (!KillPlayer(args[1]))
					SendPlayerMessage(player, "Error: Could not kill player.");
					
				break;
								
			case "/help":
				SendPlayerMessage(player, helpMsg);
				return false;

			default:
				SendPlayerMessage(player, "Unknown command!");
				SendPlayerMessage(player, helpMsg);
				return false;
		}
		
		return true;
	}

	void TeleportPlayer(PlayerBase from, PlayerBase to)
	{
		if (!from) return;
		if (!to) return;
		
		vector toPos = to.GetPosition();
		toPos[0] = toPos[0] + 10;
		toPos[1] = toPos[1] + 10;
		toPos[2] = toPos[2] + 10;
		
		from.SetPosition(toPos);
	}
	
	bool KillPlayer(string tag)
	{
		PlayerBase p = GetPlayerByAny(tag);
		
		if (!p) return false;
		
		p.SetHealth("", "", -1);
		
		return true;
	}
	

	override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
	{
		super.OnClientRespawnEvent(identity, player);
	}
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		
		switch(eventTypeId)
		{
			// Handle user command
			case ChatMessageEventTypeID:

			ChatMessageEventParams chatParams;
			Class.CastTo(chatParams, params);
			
			// Remove those stupid ' ' => Substring: x, false, false, quotes = false
			
			// Check that input was a command (contains forward slash)
			string cmd = string.ToString(chatParams.param3, false, false, false);
			//cmd = cmd.Substring(1, cmd.Length() - 1);

			// command format: /abc def ghi
			if ( cmd.Get(0) != "/" ) break;
			
			// Get sender player name as string
			string senderName = string.ToString(chatParams.param2, false, false, false);
			//senderName = senderName.Substring(1, senderName.Length() - 1);
			
			// Get sender player object
			PlayerBase sender = GetAdminPlayerByName(senderName);
			
			// If fails to get the message sender, stop
			if (!sender) {
				break;
			}
			
			// Check that player has sufficient privileges to execute commands
			if ( !IsAdmin(sender) ) {
				SendPlayerMessage(sender, "Sorry, you are not an admin!");
				break;
			}
			
			SendPlayerMessage(sender, "Command: " + cmd);
			
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
	
	PlayerBase GetAdminPlayerByName(string name)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		PlayerBase p;
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Class.CastTo(p, players.Get(i));

			if ( p.GetIdentity().GetName() == name && IsAdmin(p) ) {
				return p;
			}
		}
		
		// Player with given parameter not found
		return NULL;
	}
	
	// DANGER: Players with same name might get mixed with each other!
	// If wanted to be certain of the identity, please use another method.
	PlayerBase GetPlayerByName(string name)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		PlayerBase p;
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Class.CastTo(p, players.Get(i));

			if ( p.GetIdentity().GetName() == name ) {
				return p;
			}
		}
		
		// Player with given parameter not found
		return NULL;
	}
	
	PlayerBase GetPlayerById(string id)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		PlayerBase p;
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Class.CastTo(p, players.Get(i));

			if ( p.GetIdentity().GetPlainId() == id ) {
				return p;
			}
		}
		
		// Player with given parameter not found
		return NULL;
	}
	
	// Get player object by any means (name or id)
	PlayerBase GetPlayerByAny(string tag)
	{
		PlayerBase p;
		
		p = GetPlayerById(tag);
		
		if (!p) {
			p = GetPlayerByName(tag);
		}
		
		return p;
	}
	
	void SendGlobalMessage(string message)	
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Man player = players.Get(i);
			if ( player )
				SendPlayerMessage(player, message);
		}
	}
	
	void SendPlayerMessage(PlayerBase player, string message)	
	{
		Param1<string> Msgparam;
		Msgparam = new Param1<string>(message);
		GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
	}
	
	void MySplit(string text, string delim, out TStringArray list)
	{
		string temp = text + delim;
		string word = "";
		
		for (int i = 0; i < temp.Length(); i++ )
		{
			string x = temp.Get(i);
			
			if ( x != delim ) {
				word = word + x;
			}
			else {
				list.Insert(word);
				word = "";
			}
		}
	}
	
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player

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
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}