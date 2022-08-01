#include "Framework.h"
#include "Part.h"

Part::Part(Scene* _scene, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _updateOrder, _parent)
{
}

Part::~Part()
{
}

void Part::UpdateObject()
{
}
