#pragma once
#include "Entity/Entity.h"

namespace Nit::EditorSystem
{
    void SetSelectedEntity(Entity entity);
    Entity GetSelectedEntity();
    TString GetID();
    void Register();
}