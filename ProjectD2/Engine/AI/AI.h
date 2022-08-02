#pragma once
#include "Framework.h"
#include "State/State.h"

template <typename T, typename E>
class AI
{
private:
    T* m_owner;

    std::unordered_map<E, State<T>*> m_stateMap;
    
    State<T>* m_currentState;
    State<T>* m_previousState;
    State<T>* m_globalState;

public:
    AI(T* _owner) :
        m_owner(_owner),
        m_currentState(nullptr),
        m_previousState(nullptr),
        m_globalState(nullptr)
    {}
    ~AI()
    {
        for (auto& state : m_stateMap)
        {
            if(state.second != nullptr)
                delete state.second;
        }
    }

public:
    void Update()
    {
        if (m_globalState != nullptr)
        {
            m_globalState->Update(m_owner);
        }
        
        if (m_currentState != nullptr)
        {
            m_currentState->Update(m_owner);
        }
    }
    void ChangeState(E _state)
    {
        auto iter = m_stateMap.find(_state);
        if (iter == m_stateMap.end())
            return;

        if (m_currentState)
        {
            m_previousState = m_currentState;
            m_currentState->Exit(m_owner);
        }
        
        m_currentState = iter->second;
        m_currentState->Enter(m_owner);
    }
    void RevertToPreviousState()
    {
        ChangeState(m_previousState->GetType());
    }
    void SetGlobalState(E _state)
    {
        auto iter = m_stateMap.find(_state);
        if (iter == m_stateMap.end())
            return;
        
        m_globalState = iter->second;
    }
    
    void AddState(State<T>* _state)
    {
        m_stateMap.insert(make_pair(_state->GetType(), _state));
    }
    E GetState()
    {
        return m_currentState->GetType();
    }
};
