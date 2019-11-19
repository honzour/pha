#pragma once

#include "cotyvar.h"
#define LIBRARY_API __declspec(dllexport)

const u8 ScoreBoundNone = 0;
const u8 ScoreBoundExact = 1;
const u8 ScoreBoundLower = 2;
const u8 ScoreBoundUpper = 3;

struct TCallbacks {
    void (*NextDepth)();
    void (*SetSelDepth)(const u32);
    void (*NextMove)(const u32, const char*);
    void (*NextNode)();
    void (*EndgameTablebasesHit)();
    void (*NextBestMove)(const char*, const s16 AScore, const u8 AScoreBound);
    void (*OnStop)();
    void (*TellGUIDebug)(const char*);
    void (*TellGUIError)(const char*);
    void (*TellGUIInfo)(const char*);
};

extern TCallbacks Callbacks;

extern "C" LIBRARY_API void SetStartPos();

extern "C" LIBRARY_API void SetPositionFromString(char* AFEN);

extern "C" LIBRARY_API void DoMove(char* AMove);

extern "C" LIBRARY_API void Undo();

extern "C" LIBRARY_API void Analyze(u8 AInfinite);

extern "C" LIBRARY_API void Stop();

extern "C" LIBRARY_API void BookMoves();

extern "C" LIBRARY_API void WriteBoardToConsole();

extern "C" LIBRARY_API void WriteEvalToConsole();

extern "C" LIBRARY_API u64 Perft(u8 AValue);

extern "C" LIBRARY_API void SetCallbacks(TCallbacks& ACallbacks);

extern "C" LIBRARY_API void SetOptionOwnBook(u8 AValue);

extern "C" LIBRARY_API void SetOptionHash(ptrdiff_t AValue);

extern "C" LIBRARY_API void SetOptionClearHash();
