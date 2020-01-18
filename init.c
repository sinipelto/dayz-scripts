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
				car = GetGame().CreateObject("CivilianSedan", pos);
				car.GetInventory().CreateAttachment("CivSedanHood");
				car.GetInventory().CreateAttachment("CivSedanTrunk");
				car.GetInventory().CreateAttachment("CivSedanDoors_Driver");
				car.GetInventory().CreateAttachment("CivSedanDoors_CoDriver");
				car.GetInventory().CreateAttachment("CivSedanDoors_BackLeft");
				car.GetInventory().CreateAttachment("CivSedanDoors_BackRight");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				car.GetInventory().CreateAttachment("CivSedanWheel");
				
				SendPlayerMessage(player, "CivSedan spawned.");
				
				success = true;
				break;
				
			case 2:
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
				
			case 3:
				// Spawn and build the car
				car = GetGame().CreateObject("Sedan_02", pos);
				car.GetInventory().CreateAttachment("Sedan_02_Hood");
				car.GetInventory().CreateAttachment("Sedan_02_Trunk");
				car.GetInventory().CreateAttachment("Sedan_02_Door_1_1");
				car.GetInventory().CreateAttachment("Sedan_02_Door_1_2");
				car.GetInventory().CreateAttachment("Sedan_02_Door_2_1");
				car.GetInventory().CreateAttachment("Sedan_02_Door_2_2");
				car.GetInventory().CreateAttachment("Sedan_02_Wheel");
				car.GetInventory().CreateAttachment("Sedan_02_Wheel");
				car.GetInventory().CreateAttachment("Sedan_02_Wheel");
				car.GetInventory().CreateAttachment("Sedan_02_Wheel");
				
				SendPlayerMessage(player, "Sedan_02 spawned.");
				
				success = true;
				break;

			case 4:
				// Spawn and build the car
				car = GetGame().CreateObject("Hatchback_02", pos);
				car.GetInventory().CreateAttachment("Hatchback_02_Hood");
				car.GetInventory().CreateAttachment("Hatchback_02_Trunk");
				car.GetInventory().CreateAttachment("Hatchback_02_Door_1_1");
				car.GetInventory().CreateAttachment("Hatchback_02_Door_1_2");
				car.GetInventory().CreateAttachment("Hatchback_02_Door_2_1");
				car.GetInventory().CreateAttachment("Hatchback_02_Door_2_2");
				car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
				car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
				car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
				car.GetInventory().CreateAttachment("Hatchback_02_Wheel");
				
				SendPlayerMessage(player, "Hatchback_02 spawned.");
				
				success = true;
				break;
				
			default:
				SendPlayerMessage(player, "ERROR: SpawnCar: Car type invalid.");
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
		
		// Check that position is a valid coordinate
		if (p) {
			// Get safe surface value for Y coordinate in that position
			p[1] = GetGame().SurfaceY(p[0], p[2]);
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
			
			string info = "Player {" + string.ToString(i, false, false, false) + "}";
			info = info + "  " + "Name: " + p.GetIdentity().GetName();
			info = info + "  " + "Pos: " + p.GetPosition().ToString();
			info = info	+ "  " + "Health: " + string.ToString(p.GetHealth("GlobalHealth", "Health"));
			info = info + "  " + "Blood: " + p.GetHealth("GlobalHealth", "Blood");
			info = info + "  " + "SteamID64: " + p.GetIdentity().GetPlainId();

			SendPlayerMessage(player, info);
		}
	}
	
	bool Command(PlayerBase player, string command)
	{
		const string helpMsg = "Available commands: /help /car /warp /kill /give /say /info /suicide /here /there";
		const string carTypesMsg = "Available types: offroad, olga, olgablack, sarka, gunter";
		
		// Split command message into args
		TStringArray args = new TStringArray;
		MySplit(command, " ", args);
		
		switch (args[0])
		{
			case "/car":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /car [TYPE] - Spawn a vehicle near self");
					SendPlayerMessage(player, carTypesMsg);
					return false;
				}
				
				string carType = args[1];
				carType.ToLower();
				
				if (carType == "offroad") {
					SpawnCar(player, 0);
				}
				else if (carType == "olga") {
					SpawnCar(player, 1);
				}
				else if (carType == "olgablack") {
					SpawnCar(player, 2);
				}
				else if (carType == "sarka") {
					SpawnCar(player, 3);
				}
				else if (carType == "gunter") {
					SpawnCar(player, 4);
				}
				else if (carType == "bus") {
					SendPlayerMessage(player, "Currently not implemented, sorry!");
				}
				else {
					SendPlayerMessage(player, "Car type not found.");
					SendPlayerMessage(player, carTypesMsg);
				}
				break;
				
			case "/warp":
				if ( args.Count() < 3 ) {
				SendPlayerMessage(player, "Syntax: /warp [X] [Z] - Teleport to [X, Z]");
					return false;
				}
				string pos = args[1] + " " + "0" + " " + args[2];
				SafeSetPos(player, pos);
				SendPlayerMessage(player, "Teleported to: " + pos);
				break;
				
			case "/gear":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /gear [TYPE] - Spawn item loadout to self");
					return false;
				}
				SpawnGear(player, args[1]);
				SendPlayerMessage(player, "Gear spawned.");
				break;
				
			case "/info":
				if ( args.Count() != 1 ) {
					SendPlayerMessage(player, "Syntax: /info - Get information about players on the server");
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
				string msg = command.Substring( 5, command.Length() - 5 );
				
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
				item = player.SpawnEntityOnGroundPos(args[1], player.GetPosition());
				
				if (!item) {
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
				// We dont want to kill a random player that happens to have the same name
				KillPlayer( player.GetIdentity().GetPlainId() );
				break;

			case "/kill":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /kill [PLAYER] - Kills a player by Name or SteamID");
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
	
	void SpawnGear(PlayerBase player, string type) 
	{
		type.ToLower();
		
		vector pos = player.GetPosition();
		pos[0] = pos[0] + 2;
		pos[1] = pos[1] + 2;
		pos[2] = pos[2] + 2;
		
		// DONT spawn a mag as attachment, is buggy ingame, spawn mags in ground instead
		EntityAI item;
		EntityAI subItem;

		switch (type)
		{
			case "milgear":
				// Head
				item = player.SpawnEntityOnGroundPos("Mich2001Helmet", pos);
				
				subItem = item.GetInventory().CreateAttachment("NVGoggles");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				subItem = item.GetInventory().CreateAttachment("UniversalLight");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				player.SpawnEntityOnGroundPos("GP5GasMask", pos);

				// item = player.SpawnEntityOnGroundPos("GorkaHelmet", pos);
				// item.GetInventory().CreateAttachment("GorkaHelmetVisor", pos);
				
				// Body
				player.SpawnEntityOnGroundPos("TTsKOJacket_Camo", pos);
				player.SpawnEntityOnGroundPos("TTSKOPants", pos);
				player.SpawnEntityOnGroundPos("HighCapacityVest_Olive", pos);
				player.SpawnEntityOnGroundPos("OMNOGloves_Gray", pos);
				
				// Waist
				item = player.SpawnEntityOnGroundPos("MilitaryBelt", pos);
				item.GetInventory().CreateAttachment("Canteen");
				item.GetInventory().CreateAttachment("PlateCarrierHolster");
				
				subItem = item.GetInventory().CreateAttachment("NylonKnifeSheath");
				subItem.GetInventory().CreateAttachment("CombatKnife");
				
				// Legs
				item = player.SpawnEntityOnGroundPos("MilitaryBoots_Black", pos);
				item.GetInventory().CreateAttachment("CombatKnife");
				
				// Back
				player.SpawnEntityOnGroundPos("AssaultBag_Ttsko", pos);

				break;
			
			case "milcloth":
				break;
				
			case: "ghillie":
				break;
			
			case "m4":
				item = player.SpawnEntityOnGroundPos("M4A1", pos);
				
				item.GetInventory().CreateAttachment("M4_Suppressor");
				item.GetInventory().CreateAttachment("M4_OEBttstck");
				item.GetInventory().CreateAttachment("M4_RISHndgrd");
				
				subItem = item.GetInventory().CreateAttachment("ReflexOptic");
				subItem.GetInventory().CreateAttachment("Battery9V");
		
				subItem = item.GetInventory().CreateAttachment("UniversalLight");
				subItem.GetInventory().CreateAttachment("Battery9V");			
				
				player.SpawnEntityOnGroundPos("Mag_STANAG_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_STANAG_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_STANAG_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_STANAG_30Rnd", pos);
				
				player.SpawnEntityOnGroundPos("ACOGOptic", pos);
				
				break;
				
			case "akm":
				item = player.SpawnEntityOnGroundPos("AKM", pos);
				
				item.GetInventory().CreateAttachment("AK_Suppressor");
				item.GetInventory().CreateAttachment("AK_WoodBttstck");
				item.GetInventory().CreateAttachment("AK_RailHndgrd");
				
				subItem = item.GetInventory().CreateAttachment("KobraOptic");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				subItem = item.GetInventory().CreateAttachment("UniversalLight");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				player.SpawnEntityOnGroundPos("Mag_AKM_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_AKM_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_AKM_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_AKM_Drum75Rnd", pos);
				
				player.SpawnEntityOnGroundPos("PSO1Optic", pos);
				
				break;
			
			case "nv":
				item = player.SpawnEntityOnGroundPos("NVGHeadstrap", pos);
				
				subItem = item.GetInventory().CreateAttachment("NVGoggles");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				break;
			
			default:
				SendPlayerMessage(player, "Invalid type.");
				SendPlayerMessage(player, "Available types: milcloth, m4, akm");
		}
	}

	void TeleportPlayer(PlayerBase from, PlayerBase to)
	{
		if (!from) return;
		if (!to) return;
		
		vector toPos = to.GetPosition();

		float pos_x = toPos[0] + 10;
		float pos_z = toPos[2] + 10;
		float pos_y = GetGame().SurfaceY(pos_x, pos_z) + 5;
		
		vector pos = Vector(pos_x, pos_y, pos_z);
		
		from.SetPosition(pos);
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
		switch(eventTypeId)
		{
			// Handle user command
			case ChatMessageEventTypeID:

				ChatMessageEventParams chatParams;
				Class.CastTo(chatParams, params);
				
				// Remove those stupid ' ' => Substring: x, false, false, quotes = false
				
				// Check that input was a command (contains forward slash)
				string cmd = string.ToString(chatParams.param3, false, false, false);

				// command format: /abc def ghi
				// if not command, is normal chat message
				if ( cmd.Get(0) != "/" ) break;
				
				// Get sender player name as string
				string senderName = string.ToString(chatParams.param2, false, false, false);
				
				// Get sender player object
				PlayerBase sender = GetAdminPlayerByName(senderName);
				
				// If fails to get the message sender, stop
				if (!sender) {
					return;
				}
				
				// Check that player has sufficient privileges to execute commands
				if ( !IsAdmin(sender) ) {
					SendPlayerMessage(sender, "Sorry, you are not an admin!");
					return;
				}

				// Execute specified command
				Command(sender, cmd);
				
				// Return after execution instead of breaking so the command is not exposed to other players nearby
				return;
		}
		
		// Unless chat command was executed, operate normally
		// Call super class event handler to handle other events
		super.OnEvent(eventTypeId, params);
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

			itemEnt = player.GetInventory().CreateInInventory("TunaCan");
			itemEnt = itemTop.GetInventory().CreateInInventory("VitaminBottle");
			itemEnt = itemTop.GetInventory().CreateInInventory("TetracyclineAntibiotics");
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}