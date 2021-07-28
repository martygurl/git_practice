#pragma once

#include "Object.h"
#include "Random.h"

#include <vector>

class Mine : public Object
{
public:
    Mine();
    ~Mine();

    float* GetPosition() const { return m_position; }
    void SetPosition(float* aPosition);

    float GetDistance(float aPositionA[3], float aPositionB[3]);
    void FindCurrentTargets();
    int GetNumberOfEnemyTargets();
    
    void Explode();
    void TakeDamage(float aDamage);

private:
    float* m_position;
    float m_destructiveRadius;
    float m_health;
    float m_explosiveYield;

    std::vector<Object*> m_targetList;
};
