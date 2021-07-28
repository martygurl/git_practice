#include "stdafx.h"

#include "Object.h"

Object::Object()
{
    m_objectId = 0;
    m_team = 0;
    m_bitFlags = 0;
}
bool Object::GetActive() const
{
    return m_bitFlags & OBF_ACTIVE;
}

void Object::SetActive(bool aActive)
{
    if(aActive)
    {
        m_bitFlags &= OBF_ACTIVE;
    }
    else
    {
        m_bitFlags &= ~OBF_ACTIVE;
    }
}

bool Object::GetInvulnerable() const
{
    return (m_bitFlags & OBF_INVULNERABLE) ? true : false;
}

void Object::SetInvulnerable(bool aInvulnerable)
{
    m_bitFlags = aInvulnerable ? (m_bitFlags | OBF_INVULNERABLE) : (m_bitFlags & ~OBF_INVULNERABLE);
}
