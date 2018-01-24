#pragma once
enum class EventType { NoEvent, WindowClosed, MoveUpToggled, MoveDownToggled, MoveLeftToggled, MoveRightToggled,
    SwitchedPlayer, PassBall, SettingsMenu_LeftClickPressed ,MainMenu_RightClickPressed, MainMenu_LeftClickPressed, PreMatchMenu_RightClickPressed, PreMatchMenu_LeftClickPressed, ShootBall ,SpecialShootBall, AbilityUsed,
    SwitchToClosestPlayer, GoalScored, BallShot, PlayerTackled, BallPassed, TackleMissed, BallHitWall, CountDownStarted, MatchStarted, MatchEnded, PreMatchStarted,
    NextFormation, PreviousFormation, EscapePressed, MatchSession_LeftClickPressed, SeasonComplete, SettingsButtonPressed, GameVolumeUp, GameVolumeDown, MusicVolumeUp, MusicVolumeDown, BindingChanged,
    UserTeamChanged, AbortMatch, ResetData, QuitGame
};