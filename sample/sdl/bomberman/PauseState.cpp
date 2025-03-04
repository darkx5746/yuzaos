//
//  PauseState.cpp
//  SDL Game Programming Book
//
//  Created by shaun mitchell on 16/02/2013.
//  Copyright (c) 2013 shaun mitchell. All rights reserved.
//

#include "PauseState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "InputHandler.h"
#include "StateParser.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::s_pauseToMain()
{
    TheGame::Instance()->getStateMachine()->ChangeState(new MainMenuState());
}

void PauseState::s_resumePlay()
{
    TheGame::Instance()->getStateMachine()->PopState();
}

void PauseState::Update()
{
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(int i = 0; i < m_gameObjects.size(); i++)
        {
			m_gameObjects[i]->Update();
        }
    }
}

void PauseState::Render()
{
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(int i = 0; i < m_gameObjects.size(); i++)
        {
            m_gameObjects[i]->Draw();
        }
    }
}

bool PauseState::OnEnter()
{
    StateParser stateParser;
    stateParser.parseState("assets/conan.xml", s_pauseID, &m_gameObjects, &m_textureIDList);
    
    m_callbacks.push_back(0);
    m_callbacks.push_back(s_pauseToMain);
    m_callbacks.push_back(s_resumePlay);
    
    setCallbacks(m_callbacks);
    
    m_loadingComplete = true;
    
    std::cout << "entering PauseState\n";
    return true;
}

bool PauseState::onExit()
{
    if(m_loadingComplete && !m_gameObjects.empty())
    {
        for(int i = 0; i < m_gameObjects.size(); i++)
        {
            m_gameObjects[i]->Clean();
            delete m_gameObjects[i];
        }
        m_gameObjects.clear();
    }
    // clear the texture manager
    for(int i = 0; i < m_textureIDList.size(); i++)
    {
        TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
    }
    TheInputHandler::Instance()->reset();
    
    std::cout << "exiting PauseState\n";
    return true;
}

void PauseState::setCallbacks(const std::vector<Callback>& callbacks)
{
    // go through the game objects
    if(!m_gameObjects.empty())
    {
        for(int i = 0; i < m_gameObjects.size(); i++)
        {
            // if they are of type MenuButton then assign a callback based on the id passed in from the file
            MenuButton* pButton = m_gameObjects[i]->As<MenuButton>();
            if (pButton)
            {
                pButton->setCallback(callbacks[pButton->getCallbackID()]);
            }
        }
    }
}

