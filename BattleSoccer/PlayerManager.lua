require("DecisionTree");


PlayerManager =
{
	_PlayersContainer = {},  
	_DecisionTree = nil,

	
}

function PlayerManager:Initiate(MatchSession)
self._DecisionTree = DecisionTreeBrain:Create(nil);
self._DecisionTree:Initiate();
end


function PlayerManager:CreateNewPlayer(NewPlayer)
	print("Adding Player");
	-- add the Player to the list of Player
	local PlayerID = NewPlayer:GetPlayerID();
	if (self._PlayersContainer[PlayerID] ~= nil) then
		print("Overwriting enemy actor; id = " .. PlayerID);
	end
	self._PlayersContainer[PlayerID] = NewPlayer;

		print("PlayerID's ")
	for i,v in ipairs(self._PlayersContainer) do print(self._PlayersContainer[i]:GetPlayerID()) end
end


function PlayerManager:GetPlayer(PlayerID)
	print("Fetching ")
	print(self._PlayersContainer[PlayerID]:GetPlayerID())
	return self._PlayersContainer[PlayerID];
end

function PlayerManager:GetActorById(PlayerID)
	local Player = nil;

	Player = self:GetPlayer(PlayerID);

	return Player;
end


function PlayerManager:ChooseBestStates(MatchSession)
	-- get a PLayer
	for i,v in ipairs(self._PlayersContainer) do
		local CurrentPlayer = self._PlayersContainer[i];
		self._DecisionTree:Think(CurrentPlayer, MatchSession);
	end

end


---- non PlayerManager Function
function CalculateLength(XPOS, YPOS)
	local Distance;
	Distance = math.sqrt(XPOS^2 + YPOS^2)
	return Distance;
end

