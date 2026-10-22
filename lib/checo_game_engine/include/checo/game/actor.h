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

#pragma once

#include "checo/game/actor_component.h"
#include "checo/game/unique_id.h"
#include "checo_game_engine_export.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace checo::game
{

class CHECO_GAME_ENGINE_EXPORT Actor
{
  public:
    explicit Actor(const UniqueId id) noexcept;
    virtual ~Actor();

  public:
    Actor(const Actor &) = delete;
    Actor &operator=(const Actor &) = delete;

  public:
    template <ActorComponentType T>
    T *getComponent()
    {
        uint64_t typeId = T::getUniqueTypeId();
        if (m_Components.find(typeId) == m_Components.end()) {
            return nullptr;
        }

        return static_cast<T *>(m_Components[typeId].get());
    }

    template <ActorComponentType T, typename... Args>
    void addComponent(Args &&...args)
    {
        m_Components[T::getUniqueTypeId()] = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <ActorComponentType T>
    void removeComponent()
    {
        m_Components.erase(T::getUniqueTypeId());
    }

  public:
    inline UniqueId getId() const noexcept { return m_Id; }

    inline void setId(const UniqueId id) noexcept { m_Id = id; }

  private:
    std::unordered_map<uint64_t, std::unique_ptr<ActorComponent>> m_Components{};
    UniqueId m_Id{};
};

} // namespace checo::game