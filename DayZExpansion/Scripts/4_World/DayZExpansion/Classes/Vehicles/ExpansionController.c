/**
 * ExpansionController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPlaneController
 * @brief		Abstract implementation for vehicle controllers
 */
class ExpansionController
{
	protected bool m_UseUAInterface = false;

	protected EntityAI m_Vehicle;
	protected PlayerBase m_Player;

	void ExpansionController( EntityAI vehicle )
	{
		m_Vehicle = vehicle;
	}

	void Update()
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "ExpansionController::Update" );

		Transport transport;
		ExpansionVehicleScript vehicleScript;
		if ( Class.CastTo( transport, m_Vehicle ) )
		{
			ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "Vanilla Transport" );
			m_Player = PlayerBase.Cast( transport.CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );
		} else if ( Class.CastTo( vehicleScript, m_Vehicle ) )
		{
			ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "Expansion Transport" );
			m_Player = PlayerBase.Cast( vehicleScript.CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );
		}

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "Player: " + m_Player );

		if ( m_Player == NULL )
		{
			OnReset();

			return;
		}

		if ( !m_UseUAInterface && GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			return;
		}

		HumanCommandVehicle hcv = m_Player.GetCommand_Vehicle();
		ExpansionHumanVehicleCommand hcev = m_Player.GetCommand_ExpansionVehicle();

		if ( hcv || hcev )
		{
			if ( hcv )
			{
				if ( hcv.IsGettingIn() )
				{
					OnReset();
				} else if ( hcv.IsGettingOut() )
				{
					OnReset();
				} else if ( hcv.IsSwitchSeat() )
				{
					OnReset();
				} else
				{
					OnUpdate();
				}
			} else if ( hcev )
			{
				OnUpdate();
			}
		} else
		{
			OnReset();
		}
	}

	/**
	 * Called when player is inside the driver seat
	 */
	protected void OnUpdate()
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "ExpansionController::OnUpdate" );
	}

	/**
	 * Called when player is not inside vehicle or they are performing an action
	 */
	protected void OnReset()
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "ExpansionController::OnReset" );
	}

	void NetworkSend( ref ParamsWriteContext ctx )
	{
		if ( !m_UseUAInterface )
		{
			OnNetworkSend( ctx );
		}
	}

	protected void OnNetworkSend( ref ParamsWriteContext ctx )
	{
		
	}

	bool NetworkRecieve( ref ParamsWriteContext ctx )
	{
		if ( !m_UseUAInterface )
		{
			return OnNetworkRecieve( ctx );
		}

		return true;
	}

	protected bool OnNetworkRecieve( ref ParamsReadContext ctx )
	{
		return true;
	}

	protected void GetInputValue( string input, out float value )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( m_Player == NULL || !m_UseUAInterface )
			{
				value = value;
			} else 
			{
				value = m_Player.GetInputInterface().SyncedValue( input );
			}
		} else if ( m_Player )
		{
			value = GetGame().GetInput().LocalValue( input );
		}
	}

	protected void GetInputPress( string input, out bool value )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			if ( m_Player == NULL || !m_UseUAInterface )
			{
				value = value;
			} else 
			{
				value = m_Player.GetInputInterface().SyncedPress( input );
			}
		} else if ( m_Player )
		{
			value = GetGame().GetInput().LocalPress( input );
		}
	}
}