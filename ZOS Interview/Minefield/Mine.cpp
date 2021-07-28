#include "stdafx.h"

#ifdef __linux
#include <math.h>
#endif
#include "Mine.h"

#include "ObjectManager.h"

Mine::Mine()
{
    m_position = new float[3];
    m_destructiveRadius = 0.0f; 
    m_health = 100.0f;
    m_explosiveYield = 500;
}

Mine::~Mine()
{
    Explode();
    delete m_position;
}

void Mine::SetPosition(float* aPosition)
{
    for(int i = 0; i < 3; i++)
        m_position[i] = aPosition[i];
}

float Mine::GetDistance(float aPositionA[3], float aPositionB[3])
{
    float distance = 0.0f;
    for(int i = 0; i < 3; i++)
    {
        // compiler error, must convert pow answer to float
        distance += (float)pow((aPositionA[i] - aPositionB[i]), 2.0f);
    }

    // sqrt are costly, I am going to use another implementation:
    // usually this can be put into a math class in the engine, but
    // for the purpose of this exercise I will blatantly compute it in 
    // the body of this function
    static union { int i; float f; } u;
    u.i = 0x5F375A86 - (*(int*)&distance >> 1);
    return (int(3) - distance * u.f * u.f) * distance * u.f * 0.5f;
}

// Invulnerable mines do not take damage, but can be manually exploded if they are active
void Mine::FindCurrentTargets()
{
    if(!GetActive())
    {
        return;
    }

    m_targetList.clear();

    ObjectManager& ptrInstance = ObjectManager::GetSingleton();
    for(int i = 0; i < ptrInstance.GetNumberOfObjects(); ++i)
    {
        Object* pObject = ptrInstance.GetObject(i);

        float distance = GetDistance(m_position, pObject->GetPosition());
        if(distance > m_destructiveRadius)
        {
            break; 
        }

		//TODO: Any other reasons to not add this object?

        m_targetList.push_back(pObject);
    }
}

int Mine::GetNumberOfEnemyTargets()
{
    int numberOfEnemyTargets = 0;
    for(unsigned int i = 0; i < m_targetList.size(); ++i)
    {
        if(static_cast<Mine*>(m_targetList[i])->GetTeam() != GetTeam())
        {
            numberOfEnemyTargets++;
        }
    }

    return numberOfEnemyTargets;
}

void Mine::Explode()
{
    for(unsigned int i = 0; i < m_targetList.size(); ++i)
    {
        float distance = GetDistance(GetPosition(), m_targetList[i]->GetPosition());

        // damage is inverse-squared of distance
        float factor = 1.0f - (distance / m_destructiveRadius);
        float damage = (factor * factor) * m_explosiveYield;
        static_cast<Mine*>(m_targetList[i])->TakeDamage(damage);
    }

    // Destroy self
    TakeDamage(m_health);
}

void Mine::TakeDamage(float aDamage)
{
    m_health -= aDamage;
    if(m_health < 0.0f)
    {
        ObjectManager::GetSingleton().RemoveObject(GetObjectId());
    }
}
