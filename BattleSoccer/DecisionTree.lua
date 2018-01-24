
--lua code is used for AI strategy setting. It is a binary descion tree with decision/action nodes--
--Root node is "ISBallLooseNode"--
--Full Decision tree branch list attached in documentationZ file--



DecisionTreeBrain = 
{
	_root = nil,
}

function DecisionTreeBrain:Create(NewBrain)
   NewBrain = NewBrain or {}
   setmetatable(NewBrain, self)
   self.__index = self
   print("NewBrain Created")
   return NewBrain
end

function DecisionTreeBrain:Initiate()
	self:_BuildDecisionTree();
	return true;
end

function DecisionTreeBrain:Think(ControlledPlayer, MatchSession)
	return self._root:Decide(ControlledPlayer, MatchSession);
end

function DecisionTreeBrain:_BuildDecisionTree()

---Node Creation list ----

   self._root = IsBallLooseNode:Create(nil)
	local   CloseToLooseBallNode =  IsClosestToBallNode:Create(nil);
	local SupLooseBallAction = SupportLooseBallNode:Create(nil);
	local GoForBallActionNode = GoForBallNode:Create(nil);
	local HoldActionNode = HoldNode:Create(nil);
	local TeamOnOffenseNode = isTeamOnOffenseNode:Create(nil);
	local ClosestToBallCarrierNode = IsClosestToBallNode:Create(nil);
	local TackleBallCarrierActionNode = TackleBallCarrierNode:Create(nil);
	local CheckIfInTacklingDistance = CheckIfInTacklingDistanceNode:Create(nil);
	local GetInDefensivePositionActionNode = GetInDefensivePositionNode:Create(nil)
	local IsBallCarrierNode = CheckIsBallCarrierNode:Create(nil);
	local ShootBallActionNode = ShootBallNode:Create(nil);
	local CheckForOpenShotDescionNode = CheckForOpenShotNode:Create(nil)
	local DoesTeammateHaveShotNode = DoesTeammateHaveShot:Create(nil)
	local PassToTeamMateInScoringPositionActionNode =PassToTeamMateInScoringPositionNode:Create(nil)
	local CheckIfDefenderDescionNode = CheckIfDefenderNode:Create(nil)
	local IfMarkedNode =CheckIfMarkedNode:Create(nil)
	local IsOpenPassDescionNode = IsOpenPassNode:Create(nil)
	local AdvanceBallActionNode = AdvanceBallNode:Create(nil)
	local PassBallActionNode = PassBallNode:Create(nil)
	local GetInAttackPositionActionNode = GetInAttackPositionNode:Create(nil)


	---Node link initialization--
	
	CheckIfInTacklingDistance:SetFalseNode(GoForBallActionNode);
	CheckIfInTacklingDistance:SetTrueNode(TackleBallCarrierActionNode);

	IsOpenPassDescionNode:SetFalseNode(AdvanceBallActionNode);
	IsOpenPassDescionNode:SetTrueNode(PassBallActionNode);

	CheckIfDefenderDescionNode:SetFalseNode(AdvanceBallActionNode);
	CheckIfDefenderDescionNode:SetTrueNode(IsOpenPassDescionNode);

	IfMarkedNode:SetFalseNode(CheckIfDefenderDescionNode);
	IfMarkedNode:SetTrueNode(IsOpenPassDescionNode);

	DoesTeammateHaveShotNode:SetFalseNode(IfMarkedNode)
	DoesTeammateHaveShotNode:SetTrueNode(PassToTeamMateInScoringPositionActionNode)

	CheckForOpenShotDescionNode:SetFalseNode(DoesTeammateHaveShotNode)
	CheckForOpenShotDescionNode:SetTrueNode(ShootBallActionNode)

	IsBallCarrierNode:SetFalseNode(GetInAttackPositionActionNode);
	IsBallCarrierNode:SetTrueNode(CheckForOpenShotDescionNode);

	TeamOnOffenseNode:SetFalseNode(ClosestToBallCarrierNode);
	TeamOnOffenseNode:SetTrueNode(IsBallCarrierNode);

	ClosestToBallCarrierNode:SetFalseNode(GetInDefensivePositionActionNode);
	ClosestToBallCarrierNode:SetTrueNode(CheckIfInTacklingDistance);


   CloseToLooseBallNode:SetFalseNode(SupLooseBallAction);
    CloseToLooseBallNode:SetTrueNode(GoForBallActionNode);

  self._root:SetFalseNode(TeamOnOffenseNode);
  self._root:SetTrueNode(CloseToLooseBallNode);


		print("Built Descion Tree")

end


-- DecisionNode--
--Main Parent Node --

DecisionNode = 
{
	_trueNode = nil,
	_falseNode = nil,
}

function DecisionNode:Create(NewNode)
   NewNode = NewNode or {}
   setmetatable(NewNode, self)
   self.__index = self
   print("Default Node Created")
   return NewNode
end

function DecisionNode:Decide(ControlledPlayer, MatchSession)
	error("Calling unimplemented function DecisionNode.Decide()");
	return nil;
end

function DecisionNode:SetTrueNode(node)
	self._trueNode = node;
end

function DecisionNode:SetFalseNode(node)
	self._falseNode = node;
end

--IsOpenPassNode--
IsOpenPassNode = {
}

function IsOpenPassNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("IsOpenPassNode Created")
   return NewNode
end

function IsOpenPassNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:DoesBallCarrierHaveAnOpenPass()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function IsOpenPassNode:SetTrueNode(node)
	self._trueNode = node;
end

function IsOpenPassNode:SetFalseNode(node)
	self._falseNode = node;
end

--IsBallLooseNode--
IsBallLooseNode = {
}

function IsBallLooseNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("IsBallLooseNode Created")
   return NewNode
end

function IsBallLooseNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:IsBallLoose()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function IsBallLooseNode:SetTrueNode(node)
	self._trueNode = node;
end

function IsBallLooseNode:SetFalseNode(node)
	self._falseNode = node;
end


--IsClosestToBallNode--
IsClosestToBallNode = {
}

function IsClosestToBallNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("IsClosestToBallNode Created")
   return NewNode
end

function IsClosestToBallNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:isClosestToBall(ControlledPlayer:GetPlayerID(), ControlledPlayer:GetTeamNumber())) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function IsClosestToBallNode:SetTrueNode(node)
	self._trueNode = node;
end

function IsClosestToBallNode:SetFalseNode(node)
	self._falseNode = node;
end

--CheckIsBallCarrierNode--
CheckIsBallCarrierNode = {
}

function CheckIsBallCarrierNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("CheckIsBallCarrierNode Created")
   return NewNode
end

function CheckIsBallCarrierNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:CheckIsBallCarrier(ControlledPlayer:GetPlayerID())) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function CheckIsBallCarrierNode:SetTrueNode(node)
	self._trueNode = node;
end

function CheckIsBallCarrierNode:SetFalseNode(node)
	self._falseNode = node;
end

--CheckIfDefenderNode--
CheckIfDefenderNode = {
}

function CheckIfDefenderNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("CheckIfDefenderNode Created")
   return NewNode
end

function CheckIfDefenderNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:isBallCarrierAGoalie()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function CheckIfDefenderNode:SetTrueNode(node)
	self._trueNode = node;
end

function CheckIfDefenderNode:SetFalseNode(node)
	self._falseNode = node;
end


--CheckIfInTacklingDistanceNode--
CheckIfInTacklingDistanceNode = {
}

function CheckIfInTacklingDistanceNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("CheckIfInTacklingDistanceNode Created")
   return NewNode
end

function CheckIfInTacklingDistanceNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:GetDistanceToBall(ControlledPlayer:GetPlayerID()) < 80) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function CheckIfInTacklingDistanceNode:SetTrueNode(node)
	self._trueNode = node;
end

function CheckIfInTacklingDistanceNode:SetFalseNode(node)
	self._falseNode = node;
end

--CheckIfMarkedNode--
CheckIfMarkedNode = {
}

function CheckIfMarkedNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("CheckIfMarkedNode Created")
   return NewNode
end

function CheckIfMarkedNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:isBallCarrierMarked()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function CheckIfMarkedNode:SetTrueNode(node)
	self._trueNode = node;
end

function CheckIfMarkedNode:SetFalseNode(node)
	self._falseNode = node;
end

--SupportLooseBallNode--
SupportLooseBallNode = {
}

function SupportLooseBallNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("SupportLooseBallNode Created")
   return NewNode
end

function SupportLooseBallNode:Decide(ControlledPlayer, MatchSession)
	--go to support loose ball state--
	MatchSession:HavePlayerSupportBall(ControlledPlayer:GetPlayerID(), ControlledPlayer:GetTeamNumber());
end

function SupportLooseBallNode:SetTrueNode(node)
	error("Trying to set True Node for an action Node: SupportLooseBallNode");
end

function SupportLooseBallNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

	--GetInAttackPositionNode--
GetInAttackPositionNode = {
}

function GetInAttackPositionNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("GetInAttackPositionNode Created")
   return NewNode
end

function GetInAttackPositionNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:SendPlayerToAttackingPosition(ControlledPlayer:GetPlayerID(), ControlledPlayer:GetTeamNumber());
end

function GetInAttackPositionNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function GetInAttackPositionNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end



--TackleBallCarrierNode--
TackleBallCarrierNode = {
}

function TackleBallCarrierNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("TackleBallCarrierNode Created")
   return NewNode
end

function TackleBallCarrierNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:AITackle(ControlledPlayer:GetPlayerID());
end

function TackleBallCarrierNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function TackleBallCarrierNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--AdvanceBallNode--
AdvanceBallNode = {
}

function AdvanceBallNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("AdvanceBallNode Created")
   return NewNode
end

function AdvanceBallNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:AdvanceBall();
end

function AdvanceBallNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function AdvanceBallNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--GoForBallNode--
GoForBallNode = {
}

function GoForBallNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("GoForBallNode Created")
   return NewNode
end

function GoForBallNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:HavePlayerChaseLooseBall(ControlledPlayer:GetPlayerID());
end

function GoForBallNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function GoForBallNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--PassToTeamMateInScoringPositionNode--
PassToTeamMateInScoringPositionNode = {
}

function PassToTeamMateInScoringPositionNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("PassToTeamMateInScoringPositionNode Created")
   return NewNode
end

function PassToTeamMateInScoringPositionNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:PassToTeamMateInScoringPosition();
end

function PassToTeamMateInScoringPositionNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function PassToTeamMateInScoringPositionNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--GetInDefensivePositionNode--
GetInDefensivePositionNode = {
}

function GetInDefensivePositionNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("GetInDefensivePositionNode Created")
   return NewNode
end

function GetInDefensivePositionNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:GetInDefensivePosition(ControlledPlayer:GetPlayerID(), ControlledPlayer:GetTeamNumber());
end

function GetInDefensivePositionNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function GetInDefensivePositionNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end


--HoldNode--
HoldNode = {
}

function HoldNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("HoldNode Created")
   return NewNode
end

function HoldNode:Decide(ControlledPlayer, MatchSession)
	ControlledPlayer:HoldPosition();
end

function HoldNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function HoldNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--ShootBallNode--
ShootBallNode = {
}

function ShootBallNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("ShootBallNode Created")
   return NewNode
end

function ShootBallNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:AiShootBall();
end

function ShootBallNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function ShootBallNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--PassBallNode--
PassBallNode = {
}

function PassBallNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("PassBallNode Created")
   return NewNode
end

function PassBallNode:Decide(ControlledPlayer, MatchSession)
	MatchSession:AiPassBall();
end

function PassBallNode:SetTrueNode(node)
	error("Trying to set true Node for an action Node: SupportLooseBallNode");
end

function PassBallNode:SetFalseNode(node)
	error("Trying to set False Node for an action Node: SupportLooseBallNode");
end

--isTeamOnOffenseNode--
isTeamOnOffenseNode = {
}

function isTeamOnOffenseNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("isTeamOnOffenseNode Created")
   return NewNode
end

function isTeamOnOffenseNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession.TeamWithBall == ControlledPlayer:GetTeamNumber()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function isTeamOnOffenseNode:SetTrueNode(node)
	self._trueNode = node;
end

function isTeamOnOffenseNode:SetFalseNode(node)
	self._falseNode = node;
end

--DoesTeammateHaveShot--
DoesTeammateHaveShot = {
}

function DoesTeammateHaveShot:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("DoesTeammateHaveShot Created")
   return NewNode
end

function DoesTeammateHaveShot:Decide(ControlledPlayer, MatchSession)

	if(MatchSession:DoesTeammateHaveShot()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function DoesTeammateHaveShot:SetTrueNode(node)
	self._trueNode = node;
end

function DoesTeammateHaveShot:SetFalseNode(node)
	self._falseNode = node;
end

--CheckForOpenShotNode--
CheckForOpenShotNode = {
}

function CheckForOpenShotNode:Create(NewNode)
   NewNode = NewNode or DecisionNode:Create{NewNode}
   setmetatable(NewNode, self)
   self.__index = self
      print("CheckForOpenShotNode Created")
   return NewNode
end

function CheckForOpenShotNode:Decide(ControlledPlayer, MatchSession)
	if(MatchSession:DoesBallCarrierHaveOpenShot()) then
	return self._trueNode:Decide(ControlledPlayer, MatchSession);
	else
	return self._falseNode:Decide(ControlledPlayer, MatchSession);
	end
end

function CheckForOpenShotNode:SetTrueNode(node)
	self._trueNode = node;
end

function CheckForOpenShotNode:SetFalseNode(node)
	self._falseNode = node;
end
