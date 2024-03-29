#pragma once
#include "Entity/Entity.h"

namespace Nit::EditorSystem
{
    void SetSelectedEntity(Entity entity);
    Entity GetSelectedEntity();
    String GetID();
    void Register();
}