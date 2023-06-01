#pragma once
#include "Scene/Actor.h"

namespace Nit
{
    class ActorPanel
    {
    public:
        ActorPanel();

        Actor GetSelectedActor() const { return m_SelectedActor; }
        
        void Draw();
        
    private:
        Actor m_SelectedActor;
    };
}
