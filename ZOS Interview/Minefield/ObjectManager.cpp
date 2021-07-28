#include "stdafx.h"

#ifdef __linux
#include <stdio.h>
#endif

#include "ObjectManager.h"
#include "Object.h"
#include "Mine.h"

ObjectManager* ObjectManager::instance = nullptr;

ObjectManager::ObjectManager()
    : m_numberOfObjects(0)
    , m_nextFindTargetIndex(0)
{
}

ObjectManager::~ObjectManager()
{
    m_objects.erase(m_objects.begin(), m_objects.end());
}

void ObjectManager::Terminate()
{
    delete instance;
    instance = nullptr;
}

// is this the instance of the singleton intended? if so it needs to be in header file
// static ObjectManager* s_pObjectManager;

ObjectManager& ObjectManager::GetSingleton()
{
    // this is very bad, it is not keeping a record of all the instance it
    // created and thus causing a memory leak. And, 
    // the point of a Singleton design is to only one instance of it.
    // the solution is to create the instance at construction, and make a getter
    // function that returns a reference to the instance

    // old code
    /*static ObjectManager* pSingetonInstance = new ObjectManager();
    return *pSingetonInstance; */

    // new code
    if (!instance)
    {
        instance = new ObjectManager();
    }
    return *instance;
    
}

void ObjectManager::AddMineObject(unsigned int aObjectId, float aPosition[3], int aTeam)
{
    MutexLock lock(m_lock);

    for(int i = 0; i < m_numberOfObjects; i++)
    {
        if(m_objects[i]->GetObjectId() == aObjectId)
        {
            // If objectId matches an existing entry then just consider it as getting updated (even potentially switched to a new team)
            Mine* pMineObject = new Mine();
            pMineObject->SetObjectId(m_objects[i]->GetObjectId());
            pMineObject->SetTeam(m_objects[i]->GetTeam());
            pMineObject->SetBitFlags(m_objects[i]->GetBitFlags());
            for(int j = 0; j < 3; j++)
                pMineObject->m_position[j] = aPosition[j];
            pMineObject->m_destructiveRadius = static_cast<Mine*>(m_objects[i])->m_destructiveRadius;

            delete m_objects[i];
            m_objects[i] = pMineObject;

            return;
        }
    }

	if(m_numberOfObjects == cMaximumNumberOfObjects)
		return; 
    
	m_objects[m_numberOfObjects] = new Mine();
    m_objects[m_numberOfObjects]->SetObjectId(aObjectId);
    m_objects[m_numberOfObjects]->SetTeam(aTeam);
    m_objects[m_numberOfObjects]->SetPosition(aPosition);

    // should be a null check here in case the static_cast failed
    Mine* m = static_cast<Mine*>(m_objects[m_numberOfObjects]);
    if (m)
    {
        m->m_destructiveRadius = GetRandomFloat32_Range(10.0f, 100.0f);
    }
#if DEBUFG
    else
    {
        printf("static cast from Object to Mine failed. Radius not set!");
    }
#endif

    bool active = (GetRandomFloat32() < 0.95f);
    m_objects[m_numberOfObjects]->SetActive(active);

    if(GetRandomFloat32() < 0.1f)
    {
        m_objects[m_numberOfObjects]->SetInvulnerable(Object::OBF_INVULNERABLE);
    }

    m_numberOfObjects++;

    return;
}

void ObjectManager::RemoveObject(unsigned int aObjectId)
{
    for(int i = 0; i < m_numberOfObjects; i++)
    {
        if(m_objects[i]->m_objectId == aObjectId)
        {
            delete m_objects[i];

            // Do a fast remove and replace this location with object currently at end
            m_objects[i] = m_objects[m_numberOfObjects - 1];
            m_objects[m_numberOfObjects - 1] = NULL;
        }
    }

    if(m_numberOfObjects % 100 == 0)
        printf("Number of objects in system %u\n", m_numberOfObjects);

    return;
}

// the old implementation of this was not used, even though it was needed
// new implementation with unordered_maps
Object* ObjectManager::GetObjectByObjectId(int aObjectId)
{
    if (m_objects.find(aObjectId) != m_objects.end())
    {
        return m_objects[aObjectId];
    }

    return NULL;
}

bool ObjectManager::IsValidObject(Object* apObject)
{
    for(int i = 0; i < m_numberOfObjects; i++)
    {
        if(m_objects[i] == apObject)
        {
            return true;
        }
    }

    return false;
}

int ObjectManager::GetNextFindTargetsIndex()
{
    MutexLock lock(m_lock);

    int index = m_nextFindTargetIndex;
    m_nextFindTargetIndex++;

    return index;
}

Object* ObjectManager::GetObjectWithMostEnemyTargets(int aTeam)
{
    Object* pBestObject = NULL;
    for(int i = 0; i < m_numberOfObjects; i++)
    {
        if(m_objects[i]->m_team == aTeam)
        {
            if(pBestObject == NULL)
            {
                pBestObject = m_objects[i];
            }
            else if(static_cast<Mine*>(m_objects[i])->GetNumberOfEnemyTargets() > static_cast<Mine*>(pBestObject)->GetNumberOfEnemyTargets())
            {
                pBestObject = m_objects[i];
            }
        }
    }

    return pBestObject;
}

int ObjectManager::GetNumberOfObjectForTeam(int aTeam)
{
    int count = 0;
    for(int i = 0; i < m_numberOfObjects; i++)
    {
        if(m_objects[i]->GetTeam() == aTeam)
        {
            count++;
        }
    }

    return count;
}
