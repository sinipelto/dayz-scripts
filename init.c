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
	
	// Players that have God Mode enabled, listed here
	private ref TIntArray m_gods;
	
	override void OnInit()
	{
		super.OnInit();
		
		// Initialize needed arrays here
		m_admins = new TStringArray;
		m_gods = new TIntArray;
		
		LoadAdmins();
	}
	
	void LoadAdmins()
	{
		string path = "$profile:admins.txt";
		
		FileHandle file = OpenFile(path, FileMode.READ);
		
		// If file doesnt exist, create it
		if ( file == 0 ) {
			file = OpenFile(path, FileMode.WRITE);
			
			FPrintln(file, "// This file contains SteamID64 of all server admins. Add them below.");
			FPrintln(file, "// Line starting with // means a comment line.");
			
			CloseFile(file);
			return;
		}
		
		string line;
		
		while ( FGets( file, line ) > 0 )
		{
			if (line.Length() < 2) continue;
			if (line.Get(0) + line.Get(1) == "//") continue;
			
			m_admins.Insert(line);
		}
		
		CloseFile(file);
	}

	bool Command(PlayerBase player, string command)
	{
		const string helpMsg = "Available commands: /help /car /warp /kill /give /gear /ammo /say /info /heal /god /suicide /here /there";

		// Split command message into args
		TStringArray args = new TStringArray;
		MySplit(command, " ", args);
		
		string arg;
		PlayerBase target;
		int dist;
		
		switch (args[0])
		{
			case "/car":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /car [TYPE] - Spawn a vehicle");
					SpawnCar(player, "help");
					return false;
				}
				SpawnCar(player, args[1]);
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
				
			case "/heal":
				if ( args.Count() != 1 ) {
					SendPlayerMessage(player, "Syntax: /heal - Set all health statuses to max");
					return false;
				}
				RestoreHealth(player);
				break;
				
			case "/gear":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /gear [TYPE] - Spawn item loadout to self");
					SpawnGear(player, "help");
					return false;
				}
				if (SpawnGear(player, args[1])) {
					SendPlayerMessage(player, "Gear spawned.");
				}
				break;
				
			case "/ammo":
				// Args count: 2 <= x <= 3
				if ( args.Count() < 2 || args.Count() > 3 ) {
					SendPlayerMessage(player, "Syntax: /ammo [FOR_WEAPON] (AMOUNT) - Spawn mags and ammo for weapon");
					SpawnAmmo(player, "help");
					return false;
				}
				if ( args.Count() == 3 && SpawnAmmo(player, args[1], args[2].ToInt()) ) {
					SendPlayerMessage(player, "Ammo spawned.");
				}
				else if ( args.Count() == 2 && SpawnAmmo(player, args[1]) ) {
					SendPlayerMessage(player, "Ammo spawned.");
				}
				break;
				
			case "/info":
				if ( args.Count() < 1 || args.Count() > 2 ) {
					SendPlayerMessage(player, "Syntax: /info (0/1) - Get information about players on the server or set continuous info on/off");
					return false;
				}
				if (args.Count() == 2) {
					arg = args[1];
					arg.ToLower();
					
					if (arg.ToInt() == 1) {
						GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.PlayerInfo, 20000, true, player);
						SendPlayerMessage(player, "Continuous info mode enabled.");
						break;
					}
					else if (arg.ToInt() == 0) {
						GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.PlayerInfo);
						SendPlayerMessage(player, "Continuous info mode disabled.");
					}
				}
				else {
					PlayerInfo(player);
				}
				break;
				
			case "/say":
				if ( args.Count() < 2 ) {
					SendPlayerMessage(player, "Syntax: /say [MESSAGE] - Global announcement to all players");
					return false;
				}
				
				// Form the message string from the command text and send to all players
				string msg = command.Substring( 5, command.Length() - 5 );
				
				SendGlobalMessage(msg);
				break;
				
			case "/spawn":
				return false;
				
			case "/god":
				if ( args.Count() != 2 ) {
					SendPlayerMessage(player, "Syntax: /god [0-1] - Enable or disable semi-god mode");
					return false;
				}
				
				int setGod = args[1].ToInt();
				
				// Add player to gods, call godmode function every 1 sec
				if (setGod == 1) {
					int pId = player.GetID();
					
					if ( m_gods.Find(pId) != -1 ) {
						SendPlayerMessage(player, "Player is already god.");
						return false;
					}
					m_gods.Insert(pId);
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.GodMode, 1000, true, player);
					SendPlayerMessage(player, "Godmode enabled.");
				}
				// Do vice versa except for other gods
				else if (setGod == 0) {
					// Remove player id from gods list if found
					int godIdx = m_gods.Find( player.GetID() );
					if (godIdx == -1) {
						SendPlayerMessage(player, "Player not found in gods.");
						return false;
					}
					else {
						m_gods.Remove(godIdx);
					}
					
					// Remove godmode function from call queue but add again for remaining gods
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.GodMode);
					
					foreach (int pid : m_gods)
					{
						PlayerBase godPlayer = GetPlayer(pid.ToString(), Identity.PID);
						GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.GodMode, 1000, true, godPlayer);
					}
					
					SendPlayerMessage(player, "Godmode disabled.");
				}
				else {
					SendPlayerMessage(player, "ERROR: Invalid argument given. Argument: 0-1");
					return false;
				}
				break;
				
			case "/give":
				if ( args.Count() < 2 || args.Count() > 3 ) {
					SendPlayerMessage(player, "Syntax: /give [ITEM_NAME] (AMOUNT) - Spawn item on ground, default amount is 1");
					return false;
				}
				
				EntityAI item = player.SpawnEntityOnGroundPos(args[1], player.GetPosition());
				
				if (!item) {
					SendPlayerMessage(player, "ERROR: Could not create item.");
					return false;
				}
				if ( args.Count() == 3 ) {
					
					int itemCount = args[2].ToInt();
					
					if (itemCount <= 0) {
						SendPlayerMessage(player, "ERROR: Invalid count.");
						return false;
					}
					
					// Spawn the rest of the items if count was specified and valid
					for (int i = 0; i < itemCount - 1; i++) {
						player.SpawnEntityOnGroundPos(args[1], player.GetPosition());
					}
				}
				SendPlayerMessage(player, "Item(s) spawned.");
				break;
				
			case "/here":
				if ( args.Count() < 2 ) {
					SendPlayerMessage(player, "Syntax: /here '[PLAYER IDENTITY]' (DISTANCE) - Moves a player to self, remember to use single quotes around identity");
					return false;
				}
				
				PrepareTeleport(command, args, target, dist);
				
				if (!target) {
					SendPlayerMessage(player, "Could not found target player.");
					return false;
				}				
				if (dist < 1) {
					SendPlayerMessage(player, "Invalid distance.");
					return false;
				}
				TeleportPlayer(target, player, dist);					
				break;
				
			case "/there":
				if ( args.Count() < 2 ) {
					SendPlayerMessage(player, "Syntax: /there '[PLAYER IDENTITY]' (DISTANCE) - Moves self to a player");
					return false;
				}
				
				PrepareTeleport(command, args, target, dist);
				
				if (!target) {
					SendPlayerMessage(player, "Could not found target player.");
					return false;
				}				
				if (dist < 1) {
					SendPlayerMessage(player, "Invalid distance.");
					return false;
				}
				TeleportPlayer(player, target, dist);					
				break;

			case "/suicide":
				if ( args.Count() != 1 ) {
					SendPlayerMessage(player, "Syntax: /suicide - Commit a suicide");
					return false;
				}
				
				// Use SteamID here for sake of certainty
				if (!KillPlayer( player.GetIdentity().GetPlainId() )) {
					SendPlayerMessage(player, "Could not commit suicide.");
				}
				break;

			case "/kill":
				if ( args.Count() < 2 ) {
					SendPlayerMessage(player, "Syntax: /kill '[PLAYER IDENTITY]' - Kills a player by given identity, use single quotes around");
					return false;
				}
				
				arg = MyTrim(command, "'");
				
				if (!KillPlayer(arg)) {
					SendPlayerMessage(player, "Error: Could not kill player.");
				}	
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
	
	void PrepareTeleport(string cmd, TStringArray args, out PlayerBase target, out int distance)
	{
		// Parse target player name: "...stuff 'input' stuff..." -> "input"
		string name = MyTrim(cmd, "'");
		
		distance = args[args.Count() - 1].ToInt();
		target = GetPlayer(name, Identity.ANY);
	}
	
	bool SpawnAmmo(PlayerBase player, string type, int amount = 1)
	{
		type.ToLower();
		
		const string helpMsg = "Available ammo types: svd, m4, akm, fx45";

		vector pos = player.GetPosition();
		pos[0] = pos[0] + 1;
		pos[1] = pos[1] + 1;
		pos[2] = pos[2] + 1;
		
		string mag;
		string ammo;
		
		switch (type)
		{
			case "svd":
				mag = "Mag_SVD_10Rnd";
				ammo = "AmmoBox_762x54Tracer_20Rnd";
				break;
			
			case "m4":
				mag = "Mag_STANAG_30Rnd";
				ammo = "AmmoBox_556x45Tracer_20Rnd";
				break;
				
			case "akm":
				mag = "Mag_AKM_30Rnd";
				ammo = "AmmoBox_762x39Tracer_20Rnd";
				break;
			
			case "fx45":
				mag = "Mag_FNX45_15Rnd";
				ammo = "AmmoBox_45ACP_25rnd";
				break;

			case "help":
				SendPlayerMessage(player, helpMsg);
				return false;
			
			default:
				SendPlayerMessage(player, "Invalid ammo type.");
				SendPlayerMessage(player, helpMsg);
				return false;
		}
		
		for (int i = 0; i < amount; i++)
		{
			player.SpawnEntityOnGroundPos(mag, pos);
			player.SpawnEntityOnGroundPos(ammo, pos);
		}
		
		return true;
	}
	
	void GodMode(PlayerBase player)
	{
		if (!player || player.GetHealth("", "") <= 0.0) {
			m_gods.Remove( player.GetID() );
			return;
		}
		
		// If player is not god, do nothing
		if (m_gods.Find( player.GetID() ) == -1) {
			return;
		}
		
		// Set all health statuses to maximum
		RestoreHealth(player);
	}
	
	void RestoreHealth(PlayerBase player)
	{
		if (!player) return;
		
		player.SetHealth("GlobalHealth", "Blood", player.GetMaxHealth("GlobalHealth", "Blood"));
		player.SetHealth("GlobalHealth", "Health", player.GetMaxHealth("GlobalHealth", "Health"));
		player.SetHealth("GlobalHealth", "Shock", player.GetMaxHealth("GlobalHealth", "Shock"));
	}
	
	bool SpawnCar(PlayerBase player, string type)
	{
		type.ToLower();
		
		const string helpMsg = "Available types: offroad, olga, olgablack, sarka, gunter";
		
		// Set car pos near player
		vector pos = player.GetPosition();
		pos[0] = pos[0] + 3;
		pos[1] = pos[1] + 3;
		pos[2] = pos[2] + 3;

		Car car;
		
		switch (type)
		{
			case "offroad":
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
				break;
			
			case "olga":
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
				break;
				
			case "olgablack":
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
				break;
				
			case "sarka":
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
				break;

			case "gunter":
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
				break;
				
			case "help":
				SendPlayerMessage(player, helpMsg);
				return false;
				
			default:
				SendPlayerMessage(player, "ERROR: Car type invalid.");
				SendPlayerMessage(player, helpMsg);
				return false;
		}
		
		// A car was spawned, so we do some common car configuration

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
		
		return true;
	}
	
	void SafeSetPos(PlayerBase player, string pos)
	{
		// Safe conversion
		vector p = pos.ToVector();
		
		// Check that position is a valid coordinate
		// 0 0 0 wont be accepted even though valid
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
		if (!player) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.PlayerInfo);
			return;
		}
		
		// Clear chat history first
		for (int x = 0; x < 15; x++) {
			SendPlayerMessage(player, " ");
		}
		
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );

		// Send player count
		SendPlayerMessage(player, "Players on server: " + players.Count());
		
		// Maximum amount of single line entries that fit in the chat history: 12
		int max = 10;
		
		if ( players.Count() < max )
			max = players.Count();
		
		PlayerBase p;

		for ( int i = 0; i < max; ++i )
		{
			//if (i > 0)
			//	SendPlayerMessage(player, "*");
			
			Class.CastTo(p, players.Get(i));
			
			string info = "Player {" + string.ToString(i, false, false, false) + "}";
			info = info + "  " + "Name: " + p.GetIdentity().GetName();
			info = info + "  " + "Pos: " + p.GetPosition().ToString();
			info = info	+ "  " + "Health: " + p.GetHealth("GlobalHealth", "Health");
			info = info + "  " + "Blood: " + p.GetHealth("GlobalHealth", "Blood");
			info = info + "  " + "Shock: " + p.GetHealth("GlobalHealth", "Shock");
			info = info + "  " + "PlayerID: " + p.GetID();
			info = info + "  " + "SteamID64: " + p.GetIdentity().GetPlainId();

			SendPlayerMessage(player, info);
		}
		
		SendPlayerMessage(player, " ");
	}

	bool SpawnGear(PlayerBase player, string type) 
	{
		type.ToLower();
		
		const string helpMsg = "Available types: mil, ghillie, medic, nv, svd, m4, akm, fx45";

		vector pos = player.GetPosition();
		pos[0] = pos[0] + 1;
		pos[1] = pos[1] + 1;
		pos[2] = pos[2] + 1;
		
		// DONT spawn a mag as attachment, is buggy ingame, spawn mags in ground instead
		EntityAI item;
		EntityAI subItem;

		switch (type)
		{
			case "mil":
				// Head
				item = player.SpawnEntityOnGroundPos("Mich2001Helmet", pos);
				
				subItem = item.GetInventory().CreateAttachment("NVGoggles");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				subItem = item.GetInventory().CreateAttachment("UniversalLight");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				player.SpawnEntityOnGroundPos("GP5GasMask", pos);
				
				// Vest
				item = player.SpawnEntityOnGroundPos("SmershVest", pos);
				item.GetInventory().CreateAttachment("SmershBag");
				
				// Body
				player.SpawnEntityOnGroundPos("TTsKOJacket_Camo", pos);
				player.SpawnEntityOnGroundPos("TTSKOPants", pos);
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
				player.SpawnEntityOnGroundPos("AliceBag_Camo", pos);

				break;

			case "ghillie":
				player.SpawnEntityOnGroundPos("GhillieAtt_Woodland", pos);
				player.SpawnEntityOnGroundPos("GhillieAtt_Woodland", pos);
				player.SpawnEntityOnGroundPos("GhillieBushrag_Woodland", pos);
				player.SpawnEntityOnGroundPos("GhillieHood_Woodland", pos);
				player.SpawnEntityOnGroundPos("GhillieSuit_Woodland", pos);
				player.SpawnEntityOnGroundPos("GhillieTop_Woodland", pos);
				
				break;
				
			case "svd":
				item = player.SpawnEntityOnGroundPos("SVD", pos);
				
				item.GetInventory().CreateAttachment("AK_Suppressor");
				
				subItem = item.GetInventory().CreateAttachment("PSO1Optic");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				item = player.SpawnEntityOnGroundPos("KazuarOptic", pos);
				item.GetInventory().CreateAttachment("Battery9V");				
				
				player.SpawnEntityOnGroundPos("Mag_SVD_10Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_SVD_10Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_SVD_10Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_SVD_10Rnd", pos);
				
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
				
				item = player.SpawnEntityOnGroundPos("PSO1Optic", pos);
				item.GetInventory().CreateAttachment("Battery9V");

				player.SpawnEntityOnGroundPos("Mag_AKM_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_AKM_30Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_AKM_Drum75Rnd", pos);

				break;
			
			case "fx45":
				item = player.SpawnEntityOnGroundPos("FNX45", pos);
				
				item.GetInventory().CreateAttachment("PistolSuppressor");
				
				subItem = item.GetInventory().CreateAttachment("FNP45_MRDSOptic");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				subItem = item.GetInventory().CreateAttachment("TLRLight");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				player.SpawnEntityOnGroundPos("Mag_FNX45_15Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_FNX45_15Rnd", pos);
				player.SpawnEntityOnGroundPos("Mag_FNX45_15Rnd", pos);
				
				break;
			
			case "nv":
				item = player.SpawnEntityOnGroundPos("NVGHeadstrap", pos);
				
				subItem = item.GetInventory().CreateAttachment("NVGoggles");
				subItem.GetInventory().CreateAttachment("Battery9V");
				
				break;
				
			case "medic":
				player.SpawnEntityOnGroundPos("BandageDressing", pos);
				player.SpawnEntityOnGroundPos("BandageDressing", pos);
				player.SpawnEntityOnGroundPos("BandageDressing", pos);
				player.SpawnEntityOnGroundPos("BandageDressing", pos);
				player.SpawnEntityOnGroundPos("SalineBagIV", pos);
				player.SpawnEntityOnGroundPos("Morphine", pos);
				player.SpawnEntityOnGroundPos("Epinephrine", pos);
				
				break;
				
			case "mosin":
				break;
				
			case "sks":
				break;
				
			case "help":
				SendPlayerMessage(player, helpMsg);
				return false;
			
			default:
				SendPlayerMessage(player, "Invalid gear type.");
				SendPlayerMessage(player, helpMsg);
				return false;
		}
		
		return true;
	}

	void TeleportPlayer(PlayerBase from, PlayerBase to, int distance)
	{
		if (!from) return;
		if (!to) return;
		
		vector toPos = to.GetPosition();

		float pos_x = toPos[0] + distance;
		float pos_z = toPos[2] + distance;
		float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		
		vector pos = Vector(pos_x, pos_y, pos_z);
		
		from.SetPosition(pos);
	}
	
	bool KillPlayer(string tag)
	{
		PlayerBase p = GetPlayer(tag, Identity.ANY);
		
		if (!p) return false;
		
		p.SetHealth("", "", -1);
		
		return true;
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
				PlayerBase sender = GetPlayer(senderName, Identity.NAME);
				
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
				
				// Return after execution instead of breaking to prevent normal event handling
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

	PlayerBase GetPlayer(string tag, Identity type)
	{
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		PlayerBase p;
		
		bool nameMatch;
		bool steamIdMatch;
		bool pidMatch;
		
		for ( int i = 0; i < players.Count(); ++i )
		{
			Class.CastTo(p, players.Get(i));
			
			// Store matches from different checks
			nameMatch = p.GetIdentity().GetName() == tag;
			steamIdMatch = p.GetIdentity().GetPlainId() == tag;
			pidMatch = p.GetID() == tag.ToInt();
			
			if ( type == Identity.ANY ) {
				if ( nameMatch || steamIdMatch || pidMatch )
					return p;
			}
			
			else if ( type == Identity.NAME ) {
				if ( nameMatch )
					return p;
			}
			
			else if ( type == Identity.STEAMID ) {
				if ( steamIdMatch )
					return p;
			}

			else if ( type == Identity.PID ) {
				if ( pidMatch )
					return p;
			}
		}
		
		// Player with given parameter not found
		return NULL;
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
	
	string MyTrim(string text, string c)
	{
		if (text.Length() < 3) return "";
		
		int count = 0;
		
		int start = 0;
		int end = 0;

		for (int i = 0; i < text.Length(); i++)
		{
			if (text.Get(i) == c) {
				count++;
				start = i + 1;
				break;
			}
		}

		for (int j = text.Length() - 1; j >= 0; j--)
		{
			if (text.Get(j) == c) {
				count++;
				end = j - 1;
				break;
			}
		}
		
		// Return substring only if trimmed by c from both sides.
		if (count == 2) return text.Substring(start, end - start + 1);
		
		return "";
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
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
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

enum Identity {
	ANY,
	NAME,
	STEAMID,
	PID
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}