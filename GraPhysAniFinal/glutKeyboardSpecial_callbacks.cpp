#include "global.h"

#include <iostream>
#include <sstream>		// Added

// 1. GAME_RUNNING, 
// 2. EDITING_LIGHTS,

static const float oneDegreeInRadians = static_cast<float>(PI) / 180.0f;

void PickNextGameState(void)
{
	//1. GAME_RUNNING --> 2. EDITING_LIGHTS
	switch ( ::g_gameState.currentGameMode )
	{
	case CGameState::UNKNOWN:	// This shouldn't happen
		::g_gameState.currentGameMode = CGameState::GAME_RUNNING;
		break;

	case CGameState::GAME_RUNNING:	// 1
		{
			::g_gameState.currentGameMode = CGameState::EDITING_LIGHTS;
			g_pCamera->setMode_FollowAtDistance(::g_lightModelID);
			::g_pCamera->setMode_IndependentFreeLook();
			//g_pCamera->genBasicAccelCurve();
		}
		break;
	case CGameState::EDITING_LIGHTS:	// 2
		{
			::g_gameState.currentGameMode = CGameState::EDITING_OBJECTS;
			g_pCamera->setMode_FollowAtDistance(vecModelsInfo[g_gameState.selectedObjectID].ID); //vecModelsInfo[g_gameState.selectedObjectID].ID
			::g_pCamera->setMode_IndependentFreeLook();
			//g_pCamera->genBasicAccelCurve();
		}
		break;
	case CGameState::EDITING_OBJECTS:	// 3
		{
			::g_gameState.currentGameMode = CGameState::GAME_RUNNING;
			g_pCamera->setMode_FollowAtDistance(::g_Player_ID);
			::g_pCamera->setMode_IndependentFreeLook();
			//g_pCamera->genBasicAccelCurve();
		}
		break;
	};	
	return;
}


static const float changeInVelocity = 0.1f;
static const float havokTime = 0.5f;
static const float turningState = 0.4f;

void glutKeyboard_callback(unsigned char key, int x, int y)
{
	//std::cout << "Key: " << static_cast<unsigned int>(key) << std::endl;


	int modifierKeyState = glutGetModifiers();
	bool bAltKeyDown = ( modifierKeyState & GLUT_ACTIVE_ALT ) == GLUT_ACTIVE_ALT;
	bool bShiftKeyDown = ( modifierKeyState & GLUT_ACTIVE_SHIFT ) == GLUT_ACTIVE_SHIFT;
	bool bCtrlKeyDown = ( modifierKeyState & GLUT_ACTIVE_CTRL ) == GLUT_ACTIVE_CTRL;
	float cameraMove = 0.5f;

	unsigned int objectID;
	//if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		//objectID = vecModelsInfo[g_gameState.selectedObjectID].ID;
	//else
	objectID = ::g_Player_ID;

	std::wstringstream ss;
	ss << objectID;

	switch( key )
	{
	case 13:	// Enter key
		if ( bAltKeyDown ) 
		{	// Could also use glutFullScreenToggle() I suppose
			if ( ! ::g_gameState.bIsFullScreen )
			{
				glutFullScreen();
				::g_gameState.bIsFullScreen = true;
			}
			else
			{
				glutLeaveFullScreen();
				::g_gameState.bIsFullScreen = false;
			}
		}
		break;
	case '1': 
		PickNextGameState();
		switch ( ::g_gameState.currentGameMode )
		{
		case CGameState::EDITING_LIGHTS:
			std::cout << "GameState = EDITING_LIGHTS, specifically light: " << ::g_gameState.selectedLightID << std::endl;
			break;
		case CGameState::GAME_LOADING:
			std::cout << "GameState = GAME_LOADING" << std::endl;
			break;
		case CGameState::GAME_RUNNING:
			std::cout << "GameState = GAME_RUNNING" << std::endl;
			break;
		case CGameState::IN_COCKPIT:
			std::cout << "GameState = IN_COCKPIT" << std::endl;
			break;
		}

		break;
	case '<':
		if ( ::g_gameState.selectedLightID == 0 ) { ::g_gameState.selectedLightID = ::g_p_LightManager->getLastLightID(); }
		else { ::g_gameState.selectedLightID--; }
		std::cout << "Selected light: " << ::g_gameState.selectedLightID << std::endl;
		break;
	case '>':
		::g_gameState.selectedLightID++;
		if ( ::g_gameState.selectedLightID > ::g_p_LightManager->getLastLightID() ) { ::g_gameState.selectedLightID = 0; }
		std::cout << "Selected light: " << ::g_gameState.selectedLightID << std::endl;
		break;
	case '/':	// Turn off selected light
		{
			CLight tempLightInfo;
			::g_p_LightManager->GetLightInformation( ::g_gameState.selectedLightID, tempLightInfo );
			std::cout << "Light# " << tempLightInfo.getID() << " was enabled... now disabled." << std::endl;
			tempLightInfo.bIsEnabled = false;
			::g_p_LightManager->SetLight( tempLightInfo, true );
		}
		break;
	case '?':	// Turn ON selected light
		{
			CLight tempLightInfo;
			::g_p_LightManager->GetLightInformation( ::g_gameState.selectedLightID, tempLightInfo );
			std::cout << "Light# " << tempLightInfo.getID() << " was disabled... now enabled." << std::endl;
			tempLightInfo.bIsEnabled = true;
			::g_p_LightManager->SetLight( tempLightInfo, true );
		}
		break;


	// *********************************************************************************
	// For light editing:
	// U, J : ::g_ShaderUniformVariables.Lights[0].constantAttenuation = 0.5f;
	case 'U':
		if ( ::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS )
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->constantAttenuation *= 1.01f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " constantAttenuation = " 
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->constantAttenuation << std::endl;;
		}
		break;
	case 'u':
		if ( ::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS )
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->constantAttenuation *= 0.99f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " constantAttenuation = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->constantAttenuation << std::endl;
		}
		break;
	// I, K : ::g_ShaderUniformVariables.Lights[0].linearAttenuation = 0.5f;
	case 'I':
		if ( ::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS )
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->linearAttenuation *= 1.01f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " linearAttenuation = " 
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->linearAttenuation << std::endl;
		}
		break;
	case 'i':
		if ( ::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS )
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->linearAttenuation *= 0.99f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " linearAttenuation = " 
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->linearAttenuation << std::endl;
		}
		break;
	// O, L : ::g_ShaderUniformVariables.Lights[0].quadraticAttenuation = 0.5f;
	case 'O':
		if ( ::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS )
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->quadraticAttenuation *= 1.01f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " quadraticAttenuation = " 
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->quadraticAttenuation << std::endl;
		}
		break;
	case 'o':
		if ( ::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS )
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->quadraticAttenuation *= 0.99f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " quadraticAttenuation = " 
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->quadraticAttenuation << std::endl;
		}
		break;

	case 'j':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.x -= 0.1f;
			
			std::cout << "light #" << ::g_gameState.selectedLightID << " red = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.x << std::endl;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, galacticaProps);
			galacticaProps.adjRotationalSpeedEuler(CVector3f(oneDegreeInRadians, 0.0f, 0.0f));
			::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, galacticaProps);
		}
		break;
	case 'J':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.x += 0.1f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " red = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.x << std::endl;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, galacticaProps);
			galacticaProps.adjRotationalSpeedEuler(CVector3f(-oneDegreeInRadians, 0.0f, 0.0f));
			::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, galacticaProps);
		}
		break;

	case 'k':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.y -= 0.1f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " green = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.y << std::endl;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		{
			static CPhysicalProp galacticaProps;
				::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, galacticaProps);
				galacticaProps.adjRotationalSpeedEuler(CVector3f(0.0f, oneDegreeInRadians, 0.0f));
				::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, galacticaProps);
		}
		break;
	case 'K':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.y += 0.1f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " green = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.y << std::endl;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, galacticaProps);
			galacticaProps.adjRotationalSpeedEuler(CVector3f(0.0f, -oneDegreeInRadians, 0.0f));
			::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, galacticaProps);
		}
		break;

	case 'l': 
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.z -= 0.1f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " blue = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.z << std::endl;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, galacticaProps);
			galacticaProps.adjRotationalSpeedEuler(CVector3f(0.0f, 0.0f, oneDegreeInRadians));
			::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, galacticaProps);
		}
		break;
	case 'L':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
		{
			::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.z += 0.1f;
			std::cout << "light #" << ::g_gameState.selectedLightID << " blue = "
				<< ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.z << std::endl;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, galacticaProps);
			galacticaProps.adjRotationalSpeedEuler(CVector3f(0.0f, 0.0f, -oneDegreeInRadians));
			::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, galacticaProps);
		}
		break;

	case 't': 
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS){
			
			if (g_gameState.selectedLightID == ::g_p_LightManager->getMaxLights() - 1)
				g_gameState.selectedLightID = 0;
			else
				g_gameState.selectedLightID++;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS){
			if (g_gameState.selectedObjectID == vecModelsInfo.size() -1)
				g_gameState.selectedObjectID = 3;
			else
				g_gameState.selectedObjectID++;
			g_pCamera->setMode_FollowAtDistance(vecModelsInfo[g_gameState.selectedObjectID].ID);
			::g_pCamera->setMode_IndependentFreeLook();
		}
	break;

	case 'T':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS){
			if (g_gameState.selectedLightID == 0)
				g_gameState.selectedLightID = ::g_p_LightManager->getMaxLights() - 1;
			else
				g_gameState.selectedLightID--;
		}
		if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS){
			if (g_gameState.selectedObjectID <= 3)
				g_gameState.selectedObjectID = vecModelsInfo.size() -1;
			else
				g_gameState.selectedObjectID--;
			g_pCamera->setMode_FollowAtDistance(vecModelsInfo[g_gameState.selectedObjectID].ID);
			::g_pCamera->setMode_IndependentFreeLook();
		}
		break;
	// *********************************************************************************

	case 'p': case 'P':
		// Save state of lights
		std::cout << "light #" << ::g_gameState.selectedLightID << std::endl;
		std::cout << "constantAttenuation = " << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->constantAttenuation << std::endl;
		std::cout << " linearAttenuation = " << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->linearAttenuation << std::endl;
		std::cout << " quadraticAttenuation = " << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->quadraticAttenuation << std::endl;
		std::cout << " position: " 
			 << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position.x << ", " 
			 << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position.y << ", " 
			 << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position.z <<  std::endl;
		std::cout << " color: red = " << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.x
			<< " green = " << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.y
			<< " blue = " << ::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->diffuseColour.z << std::endl;
		break;

	case 'h':
		if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			isFocusedOnLight = !isFocusedOnLight;
		break;

	case 'r':
		//::g_pCamera->eye.x = 58.0f;		// Centred (left and right)
		//::g_pCamera->eye.y = 498.0f;		// 2.0 units "above" the "ground"
		//::g_pCamera->eye.z = 764.0f;		// .0funits0 from "back" the origin    .  1, 2.5, 1.8
		//::g_pCamera->cameraAngleYaw = 0.9f;
		//::g_pCamera->cameraAnglePitch = -0.2f;
		g_pPhysicsManager->AddFlipVehicle(true, ss.str());
		break;

		// Press M to emit particles
	case 'm': case 'M':
		drawParticles = !drawParticles;
		break;

	case ' ':		// Space
		{
			g_bApplyBrakes = true;
			g_pPhysicsManager->SetBrakingState(g_bApplyBrakes, ss.str());
		}
		break;



	case 'w': case 'W':
		if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE){
			if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			{
				CPhysicalProp lightPos;
				::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightPos);
				lightPos.position.z += 1.0f;
				::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightPos);
				
				::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position.z += 1.0f;
				std::cout << "Light #" << ::g_gameState.selectedLightID << " @ " <<
					::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->getPositionString() << std::endl;
			}
			else
			{
				//CVector3f force(0.0f, 0.0f, changeInVelocity);
				//g_pPhysicsManager->SetForce(ss.str(), havokTime, force);
				g_pPhysicsManager->SetAccelerationState(-0.7f, ss.str());
			}
		}
		else
			::g_pCamera->deltaMove = cameraMove;					//move camera forward
		break;
	case 's': case 'S':
		if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE){
			if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			{
				CPhysicalProp lightPos;
				::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightPos);
				lightPos.position.z -= 1.0f;
				::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightPos);

				::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position = lightPos.position;
				std::cout << "Light #" << ::g_gameState.selectedLightID << " @ " <<
					::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->getPositionString() << std::endl;
			}
			else
			{
				//CVector3f force(0.0f, 0.0f, -changeInVelocity);
				//g_pPhysicsManager->SetForce(ss.str(), havokTime, force);
				g_pPhysicsManager->SetAccelerationState(0.7f, ss.str());
			}
		}
		else
			::g_pCamera->deltaMove = -cameraMove;					//move camera backwards
		break;
	case 'a': case 'A':
		if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE){
			if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			{
				CPhysicalProp lightPos;
				::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightPos);
				lightPos.position.x += 1.0f;
				::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightPos);

				::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position = lightPos.position;
				std::cout << "Light #" << ::g_gameState.selectedLightID << " @ " <<
					::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->getPositionString() << std::endl;
			}
			else
			{
				//CVector3f force(changeInVelocity, 0.0f, 0.0f);
				//g_pPhysicsManager->SetForce(ss.str(), havokTime, force);
				g_pPhysicsManager->SetTurningState(-turningState, ss.str());
			}
		}
		else if (key == 'a'){
			::g_pCamera->cameraMove = (3.14f / 2.0f);			//move camera left
			::g_pCamera->deltaMove = -cameraMove;
		}
		else
			::g_pCamera->deltaAngleYaw = -0.1f;					//rotate camera left
		break;
	case 'd': case 'D':
		if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE){
			if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			{
				CPhysicalProp lightPos;
				::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightPos);
				lightPos.position.x -= 1.0f;
				::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightPos);

				::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position = lightPos.position;
				std::cout << "Light #" << ::g_gameState.selectedLightID << " @ " <<
					::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->getPositionString() << std::endl;
			}
			else
			{
				//CVector3f force(-changeInVelocity, 0.0f, 0.0f);
				//g_pPhysicsManager->SetForce(ss.str(), havokTime, force);
				g_pPhysicsManager->SetTurningState(turningState, ss.str());
			}
		}
		else if (key == 'd'){
			::g_pCamera->cameraMove = (3.14f / 2.0f);			//move camera right
			::g_pCamera->deltaMove = cameraMove;
		}
		else
			::g_pCamera->deltaAngleYaw = 0.1f;					//rotate camera right
		break;
	case 'q': case 'Q':
		if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE){
			if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			{
				CPhysicalProp lightPos;
				::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightPos);
				lightPos.position.y -= 1.0f;
				::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightPos);

				::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position = lightPos.position;
				std::cout << "Light #" << ::g_gameState.selectedLightID << " @ " <<
					::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->getPositionString() << std::endl;
			}
			else
			{
				CPhysicalProp viperProp;
				::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, viperProp);
				viperProp.velocity.y -= changeInVelocity;
				::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, viperProp);
			}
		}
		else if (key == 'q')
			::g_pCamera->eye.y -= cameraMove;			//move camera down
		else
			::g_pCamera->deltaAnglePitch = -0.1f;				//look down
		break;
	case 'e': case 'E':
		if (::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE){
			if (::g_gameState.currentGameMode == CGameState::EDITING_LIGHTS)
			{
				CPhysicalProp lightPos;
				::g_pFactoryMediator->getPhysicalPropertiesByID(::g_lightModelID, lightPos);
				lightPos.position.y += 1.0f;
				::g_pFactoryMediator->setPhysicalPropertiesByID(::g_lightModelID, lightPos);

				::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->position = lightPos.position;
				std::cout << "Light #" << ::g_gameState.selectedLightID << " @ " <<
					::g_p_LightManager->GetLightPointer(::g_gameState.selectedLightID)->getPositionString() << std::endl;
			}
			else
			{
				CPhysicalProp viperProp;
				::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, viperProp);
				viperProp.velocity.y += changeInVelocity;
				::g_pFactoryMediator->setPhysicalPropertiesByID(objectID, viperProp);
			}
		}
		else if (key == 'e')
			::g_pCamera->eye.y += cameraMove;			//move camera up
		else
			::g_pCamera->deltaAnglePitch = 0.1f;			//look up
		break;



	case 'f': case 'F':
		// Follow or stop following
		if ( ::g_pCamera->getCameraState() == CCamera::FOLLOW_TARGET_AT_DISTANCE_CHASE_PLANE )
		{
			::g_pCamera->setMode_IndependentFreeLook();
		}
		else if ( ::g_pCamera->getCameraState() == CCamera::INDEPENDENT_FREE_LOOK )
		{
			::g_pCamera->setMode_FollowAtDistance();
		}

		break;

	case 'Y': 
		::g_MyMixValue -= 0.01f;
		if ( ::g_MyMixValue < 0.0f ) { ::g_MyMixValue = 0.0f; }
		break;
	case 'y': 
		::g_MyMixValue += 0.01f;
		if ( ::g_MyMixValue > 1.0f ) { ::g_MyMixValue = 1.0f; }
		break;

	case 'c': case 'C':
	{
		std::cout << "Camera: " << std::endl;
		std::cout << "	  eye: " << ::g_pCamera->eye.x << ", " << ::g_pCamera->eye.y << ", " << ::g_pCamera->eye.z << std::endl;
		std::cout << " target: " << ::g_pCamera->target.x << ", " << ::g_pCamera->target.y << ", " << ::g_pCamera->target.z << std::endl;
		std::cout << "  Pitch: " << ::g_pCamera->cameraAnglePitch << "     Yaw: " << ::g_pCamera->cameraAngleYaw << std::endl;
		CPhysicalProp viperProp;
		::g_pFactoryMediator->getPhysicalPropertiesByID(objectID, viperProp);
		std::cout << "Player position x= " << viperProp.position.x << " y= " << viperProp.position.y << " z= " << viperProp.position.z << std::endl;
		std::cout << "Player rotation x= " << viperProp.rotStep.x << " y= " << viperProp.rotStep.y << " z= " << viperProp.rotStep.z << std::endl;
		//g_pCamera->target = viperProp.position;
	}
	break;

	case '8':		// Frank speed, Mr. Data!!
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID( ::g_Galactica_ID, galacticaProps );
			galacticaProps.velocity.z += changeInVelocity;
			::g_pFactoryMediator->setPhysicalPropertiesByID( ::g_Galactica_ID, galacticaProps );
		}
		break;
	case '9':		// Frank speed, Mr. Data!!
		{
			static CPhysicalProp galacticaProps;
			::g_pFactoryMediator->getPhysicalPropertiesByID( ::g_Galactica_ID, galacticaProps );
			galacticaProps.velocity.z -= changeInVelocity;
			::g_pFactoryMediator->setPhysicalPropertiesByID( ::g_Galactica_ID, galacticaProps );
		}		
		break;
	case 'v': case 'V':
		::g_gameState.bPrintViperPhysicsStats = !(::g_gameState.bPrintViperPhysicsStats);
		break;

	case '4':
		drawHitbox = !drawHitbox;
		break;

	case '5':
		// start the viper on 'auto' patrol
		::g_pFactoryMediator->SendMessageToObject( ::g_Player_ID, 0, CMessage( CNameValuePair( "StartPatrol", 0 ) ) );
		break;
	
	case '6':
		// return control to the user
		::g_pFactoryMediator->SendMessageToObject( ::g_Player_ID, 0, CMessage( CNameValuePair( "UnderUserControl", 0 ) ) );
		break;

	case 'b': case 'B':		// Save the current positions to a file
		g_SavingData = true;
		break;
	};



	return;
}


void resetEverythingHack(void)
{
	g_pCamera->target.x = 0.0f;		// Centred (left and right)
	g_pCamera->target.y = 2.0f;		// 2.0 units "above" the "ground"
	g_pCamera->target.z = 0.0f;		// 4.0 units "back" from the origin


	//g_pCamera->eye.x = -13.1195f;			// 30.0f;		
	//g_pCamera->eye.y = 13.50319f;		// 15.0f;		
	//g_pCamera->eye.z = 65.2409f;		// 0.0f;		
	g_pCamera->eye.x = 3.0f;			// 30.0f;		
	g_pCamera->eye.y = 5.23347f;		// 15.0f;		
	g_pCamera->eye.z = 124.979f;		// 0.0f;		

	// Reset viper
	CPhysicalProp viperPhysProps;
	::g_pFactoryMediator->getPhysicalPropertiesByID( ::g_Player_ID, viperPhysProps );
	//viperPhysProps.position = CVector3f( -17.1195f, 9.50319f, 60.2409f );
	viperPhysProps.position = CVector3f( 1.742699f, 4.2033738f, 120.092857f );		// In front
	viperPhysProps.setOrientationEulerAngles( CVector3f(0.0f,0.0f,0.0f) );
	viperPhysProps.rotationalSpeed = glm::fquat( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	viperPhysProps.acceleration = CVector3f(0.0f,0.0f,0.0f);
	viperPhysProps.velocity = CVector3f(0.0f,0.0f,0.0f);
	viperPhysProps.directedAcceleration = 0.0f;
	viperPhysProps.directedVelocity = 0.0f;
	::g_pFactoryMediator->setPhysicalPropertiesByID( ::g_Player_ID, viperPhysProps );


	// Reset Galactica
	CPhysicalProp galacticalProps;		
	::g_pFactoryMediator->getPhysicalPropertiesByID( ::g_Galactica_ID, galacticalProps );
	galacticalProps.position = CVector3f( -30.0f, 0.0f, -45.0f );
	galacticalProps.setOrientationEulerAngles( CVector3f(0.0f,0.0f,0.0f) );
	galacticalProps.rotationalSpeed = glm::fquat( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	galacticalProps.acceleration = CVector3f(0.0f,0.0f,0.0f);
	galacticalProps.velocity = CVector3f(0.0f,0.0f,0.0f);
	galacticalProps.directedAcceleration = 0.0f;
	galacticalProps.directedVelocity = 0.0f;
	::g_pFactoryMediator->setPhysicalPropertiesByID( ::g_Galactica_ID, galacticalProps );


	return;
}

void glutSpecialKey_callback( int key, int x, int y )
{
	// NOTE: the Galactica is ALREADY rotated 90 degrees, so the "axes" that I'm picking here aren't "right" in the 
	//	sense that it's difficult to predict. For example, around the y-axis would normally be "left and right", but 
	//	since it's rotated already, we're rotating around the z. This could be "fixed" by fixing the original model
	// (which is facing down along the y axis for some reason. I blame the Cylons...)

	unsigned int objectID;
	
	objectID = ::g_Player_ID;

	std::wstringstream ss;
	ss << objectID;

	switch ( key )
	{
	case GLUT_KEY_F2:
		::resetEverythingHack();

		break;
	case GLUT_KEY_F4:
		// See if we are also pressing "alt"
		if ( glutGetModifiers() == GLUT_ACTIVE_ALT )
		{
			// exit, baby
			glutLeaveMainLoop();	// Note, this doesn't exist in the original GLUT
		}
		break;

	case GLUT_KEY_UP:
			g_pPhysicsManager->SetAccelerationState(-0.7f, ss.str());
		break;
	case GLUT_KEY_DOWN:
			g_pPhysicsManager->SetAccelerationState(0.7f, ss.str());
		break;
	case GLUT_KEY_LEFT:
		g_pPhysicsManager->SetTurningState(-turningState, ss.str());
		break;
	case GLUT_KEY_RIGHT:
		g_pPhysicsManager->SetTurningState(turningState, ss.str());
		break;

	};
	return;
}

void glutKeyboardUp_callback(unsigned char key, int x, int y)
{
	//std::cout << "Key: " << static_cast<unsigned int>(key) << std::endl;


	int modifierKeyState = glutGetModifiers();
	bool bAltKeyDown = (modifierKeyState & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT;
	bool bShiftKeyDown = (modifierKeyState & GLUT_ACTIVE_SHIFT) == GLUT_ACTIVE_SHIFT;
	bool bCtrlKeyDown = (modifierKeyState & GLUT_ACTIVE_CTRL) == GLUT_ACTIVE_CTRL;
	float cameraMove = 1.0f;

	unsigned int objectID;
	//if (::g_gameState.currentGameMode == CGameState::EDITING_OBJECTS)
	//objectID = vecModelsInfo[g_gameState.selectedObjectID].ID;
	//else
	objectID = ::g_Player_ID;

	std::wstringstream ss;
	ss << objectID;

	switch (key)
	{
		case 'w': case 'W': case 's': case 'S':
			g_pPhysicsManager->SetAccelerationState(0.0f, ss.str());
		break;
		
		case 'a': case 'A': case 'd': case 'D':
			g_pPhysicsManager->SetTurningState(0.0, ss.str());
		break;

		case ' ':		// Space
			g_bApplyBrakes = false;
			g_pPhysicsManager->SetBrakingState(g_bApplyBrakes, ss.str());
		break;

		case 'r':
		g_pPhysicsManager->AddFlipVehicle(false, ss.str());
		break;
	};
	return;
}

void glutSpecialUp_callback(int key, int x, int y)
{
	unsigned int objectID;

	objectID = ::g_Player_ID;

	std::wstringstream ss;
	ss << objectID;

	switch (key)
	{
		
		case GLUT_KEY_UP: case GLUT_KEY_DOWN:
			g_pPhysicsManager->SetAccelerationState(0.0f, ss.str());
		break;
		case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
			g_pPhysicsManager->SetTurningState(0.0f, ss.str());
		break;
		

	};
	return;

}