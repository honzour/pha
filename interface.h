#pragma once

#include "volby.h"
#include "cotyvar.h"
#define LIBRARY_API __declspec(dllexport)

#define ScoreBoundNone 0
#define ScoreBoundExact 1
#define ScoreBoundLower 2
#define ScoreBoundUpper 3

typedef struct {
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
} TCallbacks;

extern TCallbacks Callbacks;

cfunkce LIBRARY_API void SetStartPos();

cfunkce LIBRARY_API void SetPositionFromString(char* AFEN);

cfunkce LIBRARY_API void DoMove(char* AMove);

cfunkce LIBRARY_API void Undo();

cfunkce LIBRARY_API void Analyze(u8 AInfinite);

cfunkce LIBRARY_API void Stop();

cfunkce LIBRARY_API void BookMoves();

cfunkce LIBRARY_API void WriteBoardToConsole();

cfunkce LIBRARY_API void WriteEvalToConsole();

cfunkce LIBRARY_API u64 Perft(u8 AValue);

cfunkce LIBRARY_API void SetCallbacks(TCallbacks *ACallbacks);

cfunkce LIBRARY_API void SetOptionOwnBook(u8 AValue);

cfunkce LIBRARY_API void SetOptionHash(ptrdiff_t AValue);

cfunkce LIBRARY_API void SetOptionClearHash();
