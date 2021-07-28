#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#pragma once

#include "Mutex.h"
#include <unordered_map>

class Object;

const int cMaximumNumberOfObjects = 1000000;

class ObjectManager
{
public:

    static ObjectManager& GetSingleton();
    static void Terminate();

    int GetNumberOfObjects() { return m_numberOfObjects; }
    Object* GetObject(int aIndex) { return m_objects[aIndex]; }
    Object* GetObjectByObjectId(int aObjectId);
    bool IsValidObject(Object* apObject);

    void RemoveObject(unsigned int aObjectId);

    void AddMineObject(unsigned int aObjectId, float aPosition[3], int aTeam);

    int GetNextFindTargetsIndex();
    void ResetNextFindTargetIndex() { m_nextFindTargetIndex = 0; }

    Object* GetObjectWithMostEnemyTargets(int aTeam);
    int GetNumberOfObjectForTeam(int aTeam);

private:

    Mutex m_lock;
    int m_numberOfObjects;

    // since we use this to search Objects via Ids, making it a hash map would 
    // greatly reduce search time
    std::unordered_map<unsigned int, Object*> m_objects;
    int m_nextFindTargetIndex;

    static ObjectManager* instance;
    ObjectManager();

    ~ObjectManager();
};

#endif // OBJECTMANAGER_H
