#pragma once
#include <vector>

template <typename T>
class Observer
{
public:
    virtual void FieldChanged(T& field, const string& fieldName) = 0;
};

template <typename T>
class Observable
{
public:
    Observable() = default;
    ~Observable()
    {
        observers.clear();
    }

private:
    std::vector<Observer<T>*> observers = {};

public:
    void Notify(T& field, const string& fieldName)
    {
        for (auto observer : observers)
        {
            observer->FieldChanged(field, fieldName);
        }
    }
    void Subscribe(Observer<T>* observer)
    {
        observers.emplace_back(observer);
    }
    void Unsubscribe(Observer<T>* observer)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

};