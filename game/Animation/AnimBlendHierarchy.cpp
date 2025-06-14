#include "AnimBlendHierarchy.h"
#include "AnimManager.h"
#include "game/Core/KeyGen.h"
#include "../vendor/armhook/patch.h"
#include "game/Core/Quaternion.h"
#include "game/MemoryMgr.h"

void CAnimBlendHierarchy::InjectHooks() {

}

// 0x4CF270
CAnimBlendHierarchy::CAnimBlendHierarchy() {
    m_pSequences = nullptr;
    m_nSeqCount = 0;
    m_bRunningCompressed = false;
    m_bKeepCompressed = false;
    m_nAnimBlockId = -1;
    m_fTotalTime = 0.0f;
    m_Link = nullptr;
    m_hashKey = 0; // NOTSA
}

// 0x4CF970
CAnimBlendHierarchy::~CAnimBlendHierarchy() {
    RemoveAnimSequences();
}

// 0x4CF980
void CAnimBlendHierarchy::Shutdown() {
    RemoveAnimSequences();
    m_bRunningCompressed = false;
}

// 0x4CF8E0
void CAnimBlendHierarchy::RemoveAnimSequences() {
    void* oldFrameData = nullptr;
    if (m_pSequences && m_pSequences->m_usingExternalMemory) {
        oldFrameData = m_pSequences->m_pFrames;
    }
    CAnimManager::RemoveFromUncompressedCache(this);
    delete[] m_pSequences;
    m_pSequences = nullptr;
    m_nSeqCount = 0;
    m_fTotalTime = 0.0f;
    if (oldFrameData) {
        CMemoryMgr::Free(oldFrameData);
    }
}

// 0x4CF510
uint8* CAnimBlendHierarchy::AllocSequenceBlock(bool compressed) {
    size_t size = 0;
    for (auto& sequence : GetSequences()) {
        size += sequence.GetDataSize(compressed);
    }
    return static_cast<uint8*>(CMemoryMgr::Malloc(size));
}

// 0x4CF290
CAnimBlendSequence* CAnimBlendHierarchy::FindSequence(const char* name) {
    if (m_nSeqCount <= 0) {
        return nullptr;
    }

    auto hash = CKeyGen::GetUppercaseKey(name);
    for (auto& sequence : GetSequences()) {
        if (sequence.m_hash == hash) {
            return &sequence;
        }
    }
    return nullptr;
}

// 0x4CF2D0
void CAnimBlendHierarchy::SetName(const char* name) {
    m_hashKey = CKeyGen::GetUppercaseKey(name);
}

// 0x4CF2F0
void CAnimBlendHierarchy::CalcTotalTime() {
    m_fTotalTime = 0.0f;
    for (auto& sequence : GetSequences()) {
        if (sequence.m_nFrameCount == 0) { // FIX_BUGS by Mitchell Tobass
            continue;
        }
        m_fTotalTime = std::max(m_fTotalTime, sequence.GetUncompressedFrame(sequence.m_nFrameCount - 1)->deltaTime);
        for (auto j = sequence.m_nFrameCount - 1; j >= 1; j--) {
            KeyFrame* kf1 = sequence.GetUncompressedFrame(j);
            KeyFrame* kf2 = sequence.GetUncompressedFrame(j - 1);
            kf1->deltaTime -= kf2->deltaTime;
        }
    }
}

// 0x4CF3E0
void CAnimBlendHierarchy::CalcTotalTimeCompressed() {
    m_fTotalTime = 0.0f;
    for (auto& sequence : GetSequences()) {
        if (sequence.m_nFrameCount == 0) { // FIX_BUGS by Mitchell Tobass
            continue;
        }
        m_fTotalTime = std::max(m_fTotalTime, sequence.GetCompressedFrame(sequence.m_nFrameCount - 1)->GetDeltaTime());
        for (auto j = sequence.m_nFrameCount - 1; j >= 1; j--) {
            KeyFrameCompressed* kf1 = sequence.GetCompressedFrame(j);
            KeyFrameCompressed* kf2 = sequence.GetCompressedFrame(j - 1);
            kf1->deltaTime -= kf2->deltaTime;
        }
    }
}

// 0x4CF4E0
void CAnimBlendHierarchy::RemoveQuaternionFlips() {
    for (auto& sequence : GetSequences()) {
        sequence.RemoveQuaternionFlips();
    }
}

// 0x4CF560
void* CAnimBlendHierarchy::GetSequenceBlock() {
    if (m_pSequences->m_usingExternalMemory) {
        return m_pSequences->m_pFrames;
    } else {
        return nullptr;
    }
}

// 0x4CF5F0
void CAnimBlendHierarchy::Uncompress() {
    assert(m_nSeqCount > 0);

    auto frameData = AllocSequenceBlock(false);
    auto oldFrameData = GetSequenceBlock();

    for (auto& sequence : GetSequences()) {
        sequence.Uncompress(frameData);
        frameData = (uint8*)((size_t)frameData + sequence.GetDataSize(false));
    }

    if (oldFrameData) {
        CMemoryMgr::Free(oldFrameData);
    }

    m_bRunningCompressed = false;
    if (m_fTotalTime == 0.0f) {
        RemoveQuaternionFlips();
        CalcTotalTime();
    }
}

// 0x4CF6C0
void CAnimBlendHierarchy::CompressKeyframes() {
    auto frameData = AllocSequenceBlock(true);
    auto oldFrameData = GetSequenceBlock();

    for (auto& sequence : GetSequences()) {
        sequence.CompressKeyframes(frameData);
        frameData += sequence.GetDataSize(true);
    }

    if (oldFrameData) {
        CMemoryMgr::Free(oldFrameData);
    }
}

// 0x4CF760
void CAnimBlendHierarchy::RemoveUncompressedData() {
    assert(m_nSeqCount > 0);

    auto frameData = AllocSequenceBlock(true);
    auto oldFrameData = GetSequenceBlock();

    for (auto& sequence : GetSequences()) {
        sequence.RemoveUncompressedData(frameData);
        frameData = (uint8*)((size_t)frameData + sequence.GetDataSize(true));
    }

    if (oldFrameData) {
        CMemoryMgr::Free(oldFrameData);
    }

    m_bRunningCompressed = true;
}

// 0x4CF800
void CAnimBlendHierarchy::MoveMemory() {
    CHook::CallFunction<void>(g_libGTASA + 0x462C28, this);
}

// 0x4CF8A0
void CAnimBlendHierarchy::Print() {
    CAnimManager::UncompressAnimation(this);
    for (auto& sequence : GetSequences()) {
        sequence.Print();
    }
}
