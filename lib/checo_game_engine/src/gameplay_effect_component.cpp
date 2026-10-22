/*
 * MIT License
 *
 * Copyright (c) 2026 Sergei Cherevichko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "checo/game/gameplay_effect_component.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace checo::game
{

struct GameplayEffectComponent::Impl
{
    Actor *m_Actor{nullptr};
    std::vector<std::unique_ptr<GameplayEffect>> m_Effects;
};

GameplayEffectComponent::GameplayEffectComponent(Actor *actor) noexcept
    : m_Impl(std::make_unique<Impl>())
{
    m_Impl->m_Actor = actor;
}

GameplayEffectComponent::~GameplayEffectComponent() = default;

void GameplayEffectComponent::forEachEffect(void (*callback)(const GameplayEffect *effect))
{
    for (const auto &effect : m_Impl->m_Effects) {
        callback(effect.get());
    }
}

void GameplayEffectComponent::addEffect(std::unique_ptr<GameplayEffect> effect)
{
    m_Impl->m_Effects.push_back(std::move(effect));
    m_Impl->m_Effects.back()->apply(m_Impl->m_Actor);
}

void GameplayEffectComponent::removeEffect(const UniqueId instanceId)
{
    std::erase_if(m_Impl->m_Effects, [&](const std::unique_ptr<GameplayEffect> &effect) {
        if (effect->getInstanceId() == instanceId) {
            effect->remove(m_Impl->m_Actor);
            return true;
        }
        return false;
    });
}

void GameplayEffectComponent::removeEffectsByDefinitionId(const UniqueId definitionId)
{

    std::erase_if(m_Impl->m_Effects, [&](const std::unique_ptr<GameplayEffect> &effect) {
        if (effect->getDefinitionId() == definitionId) {
            effect->remove(m_Impl->m_Actor);
            return true;
        }
        return false;
    });
}

void GameplayEffectComponent::removeEffectsBySourceId(const UniqueId sourceId)
{
    std::erase_if(m_Impl->m_Effects, [&](const std::unique_ptr<GameplayEffect> &effect) {
        if (effect->getSourceId() == sourceId) {
            effect->remove(m_Impl->m_Actor);
            return true;
        }
        return false;
    });
}

bool GameplayEffectComponent::hasEffect(const UniqueId instanceId) const
{
    return std::ranges::any_of(m_Impl->m_Effects, std::bind_front(std::equal_to{}, instanceId),
        &GameplayEffect::getInstanceId);
}

bool GameplayEffectComponent::hasEffectByDefinitionId(const UniqueId definitionId) const
{
    return std::ranges::any_of(m_Impl->m_Effects, std::bind_front(std::equal_to{}, definitionId),
        &GameplayEffect::getDefinitionId);
}

bool GameplayEffectComponent::hasEffectBySourceId(const UniqueId sourceId) const
{
    return std::ranges::any_of(m_Impl->m_Effects, std::bind_front(std::equal_to{}, sourceId),
        &GameplayEffect::getSourceId);
}

void GameplayEffectComponent::clearEffects()
{
    for (auto &effect : m_Impl->m_Effects) {
        effect->remove(m_Impl->m_Actor);
    }
    m_Impl->m_Effects.clear();
}

} // namespace checo::game