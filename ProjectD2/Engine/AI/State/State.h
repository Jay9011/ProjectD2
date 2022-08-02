#pragma once
template <typename T>
class State
{
public:
    State() = default;
    virtual ~State() = default;

public:
    virtual void Enter (T* _entity) = 0;
    virtual void Update(T* _entity) = 0;
    virtual void Exit  (T* _entity) = 0;
    
    virtual MON_STATE GetType() const = 0;
};

