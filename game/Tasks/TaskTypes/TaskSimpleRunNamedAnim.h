/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "TaskSimpleAnim.h"
#include "../../Animation/AnimBlendHierarchy.h"
#include "../TaskTimer.h"

class CTaskSimpleRunNamedAnim : public CTaskSimpleAnim {
public:
    char                 m_animName[24];
    char                 m_animGroupName[16];
    float                m_fBlendDelta;
    CAnimBlendHierarchy* m_pAnimHierarchy;
    uint32               m_Time;
    CTaskTimer           m_Timer;
    CVector              m_vecOffsetAtEnd;
    uint32               m_animFlags;
    int16                m_nAnimId;

public:
    static constexpr auto Type = TASK_SIMPLE_NAMED_ANIM;

    CTaskSimpleRunNamedAnim();
    CTaskSimpleRunNamedAnim(const char* animName, const char* animGroupName, uint32 animFlags, float blendDelta,
                            uint32 endTime, bool bDontInterrupt, bool bRunInSequence, bool bOffsetPed, bool bHoldLastFrame);

    eTaskType GetTaskType() const override { return Type; }
    CTask* Clone() const override {
        return new CTaskSimpleRunNamedAnim(m_animName, m_animGroupName,
                                           m_animFlags, m_fBlendDelta, m_Time,
                                           (m_nFlags & 2) >> 1, (m_nFlags & 0x10) >> 4, (m_nFlags & 0x20) >> 5, (m_nFlags & 4) >> 2); // todo: flags
    }
    bool ProcessPed(CPedGTA* ped) override;
    void OffsetPedPosition(CPedGTA* ped);

private:
    static void InjectHooks();

};
VALIDATE_SIZE(CTaskSimpleRunNamedAnim, 0x78);
