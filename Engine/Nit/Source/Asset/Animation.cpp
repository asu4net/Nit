#include "Animation.h"
#include "Core/Engine.h"
#include "Content.h"
#include "Sprite.h"

RTTR_REGISTRATION
{
    using namespace Nit;
     rttr::registration::class_<Animation::Key>("Key")
        .constructor<>()
        .property("Key", &Animation::Key::KeyRef)
        .property("KeyTime", &Animation::Key::KeyTime);

    rttr::registration::class_<Animation>("Animation")
        .constructor<>()
        .property("Keys", &Animation::m_Keys);
}

namespace Nit
{
    NIT_FORCE_LINK_IMPL(Animation)

    void Animation::PushKey(const Key& key)
    {
        //NIT_CHECK((key.KeyTime <= m_Time), "The KeyTime should be minor than the animation time");
        m_Keys.emplace_back(key);
        SortKeys();
    }

    void Animation::PopKey(uint32_t index)
    {
        m_Keys.erase(m_Keys.begin() + index);
        SortKeys();
    }

    const Animation::Key& Animation::GetKey(uint32_t index) const
    {
        return m_Keys[index];
    }

    void Animation::PushKeysFromSpriteSheet(const AssetRef spriteSheetRef, float keyTime)
    {
        Sprite& spriteSheet = spriteSheetRef.GetAs<Sprite>();
        float currentKeyTime = 0.f;
        
        spriteSheet.ForEachSubSprite({[&](const String& name, const SubSprite& subSprite) {
            m_Keys.push_back({name, currentKeyTime});
            currentKeyTime += keyTime;
        }});
    }

    size_t Animation::GetSize() const
    {
        return m_Keys.size();
    }

    void Animation::AdjustTime()
    {
        if (m_Keys.empty())
        {
            return;
        }

        DynamicArray<Key> keys(m_Keys.size());
        keys = m_Keys;
        
        std::sort(keys.begin(), keys.end(), [](const Key& a, const Key& b) {
            return a.KeyTime > b.KeyTime;
        });

        m_Time = keys[0].KeyTime;
    }

    void Animation::Clear()
    {
        m_Keys.clear();
    }

    void Animation::SortKeys()
    {
        std::sort(m_Keys.begin(), m_Keys.end(), [](const Key& a, const Key& b) {
            return a.KeyTime < b.KeyTime;
        });
    }
}