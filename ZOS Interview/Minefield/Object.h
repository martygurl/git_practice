#pragma once

// set some members to private and made getters and setters
// this allows encapsulation

class Object
{
public:
    Object();

    ~Object() {}

    int GetObjectId() const { return m_objectId; }
    void SetObjectId(int id) { m_objectId = id; }

    bool GetActive() const;
    void SetActive(bool aActive);

    int GetBitFlags() const { return m_bitFlags; }
    void SetBitFlags(int flags) { m_bitFlags = flags; }
    bool GetInvulnerable() const;
    void SetInvulnerable(bool aInvulnerable);

    virtual float* GetPosition() const { return 0; }
    virtual void SetPosition(float*) {}

    int GetTeam() const { return m_team; }
    void SetTeam(int team) { m_team = team; }

    enum ObjectBitFlags
    {
        OBF_ACTIVE = 0x0000,
        OBF_INVULNERABLE = 0x0002,
    };

private:
    unsigned int m_objectId;
    int m_team;
    int m_bitFlags;
};