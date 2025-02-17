/**
 * File: z_shrink_window.c
 * Description: Draws black top/bottom/side borders on the viewing window (e.g. Z-targeting, talking to NPC)
 */

#include "global.h"
#include "z64shrink_window.h"

typedef struct {
    /* 0x0 */ s8 letterboxTarget;
    /* 0x1 */ s8 letterboxSize;
    /* 0x2 */ s8 pillarboxTarget;
    /* 0x3 */ s8 pillarboxSize;
} ShrinkWindow; // size = 0x4

ShrinkWindow sShrinkWindow;
ShrinkWindow* sShrinkWindowPtr;

void ShrinkWindow_Letterbox_SetSizeTarget(s32 target) {
    sShrinkWindowPtr->letterboxTarget = target;
}

s32 ShrinkWindow_Letterbox_GetSizeTarget(void) {
    return sShrinkWindowPtr->letterboxTarget;
}

void ShrinkWindow_Letterbox_SetSize(s32 size) {
    sShrinkWindowPtr->letterboxSize = size;
}

s32 ShrinkWindow_Letterbox_GetSize(void) {
    return sShrinkWindowPtr->letterboxSize;
}

void ShrinkWindow_Pillarbox_SetSizeTarget(s32 target) {
    sShrinkWindowPtr->pillarboxTarget = target;
}

s32 ShrinkWindow_Pillarbox_GetSizeTarget(void) {
    return sShrinkWindowPtr->pillarboxTarget;
}

void ShrinkWindow_Pillarbox_SetSize(s32 size) {
    sShrinkWindowPtr->pillarboxSize = size;
}

s32 ShrinkWindow_Pillarbox_GetSize(void) {
    return sShrinkWindowPtr->pillarboxSize;
}

void ShrinkWindow_Init(void) {
    sShrinkWindowPtr = &sShrinkWindow;
    bzero(sShrinkWindowPtr, sizeof(sShrinkWindow));
}

void ShrinkWindow_Destroy(void) {
    sShrinkWindowPtr = NULL;
}

void ShrinkWindow_Update(s32 framerateDivisor) {
    s32 step = (framerateDivisor == 3) ? 10 : (30 / framerateDivisor);
    s32 nextSize;

    nextSize = sShrinkWindowPtr->letterboxSize;
    Math_StepToIGet(&nextSize, sShrinkWindowPtr->letterboxTarget, step);
    sShrinkWindowPtr->letterboxSize = nextSize;

    nextSize = sShrinkWindowPtr->pillarboxSize;
    Math_StepToIGet(&nextSize, sShrinkWindowPtr->pillarboxTarget, step);
    sShrinkWindowPtr->pillarboxSize = nextSize;
}

void ShrinkWindow_Draw(GraphicsContext* gfxCtx) {
    Gfx* gfx;
    s8 letterboxSize = sShrinkWindowPtr->letterboxSize;
    s8 pillarboxSize = sShrinkWindowPtr->pillarboxSize;

    if (letterboxSize > 0) {
        OPEN_DISPS(gfxCtx);

        gfx = OVERLAY_DISP;

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
        gDPFillRectangle(gfx++, 0, 0, gScreenWidth - 1, letterboxSize - 1);
        gDPFillRectangle(gfx++, 0, gScreenHeight - letterboxSize, gScreenWidth - 1, gScreenHeight - 1);

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 0);
        gDPFillRectangle(gfx++, 0, letterboxSize, gScreenWidth, letterboxSize + 1);
        gDPFillRectangle(gfx++, 0, gScreenHeight - letterboxSize - 1, gScreenWidth, gScreenHeight - letterboxSize);

        gDPPipeSync(gfx++);
        OVERLAY_DISP = gfx++;

        CLOSE_DISPS(gfxCtx);
    }

    if (pillarboxSize > 0) {
        OPEN_DISPS(gfxCtx);

        gfx = OVERLAY_DISP;

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));

        gDPFillRectangle(gfx++, 0, 0, pillarboxSize - 1, gScreenHeight - 1);
        gDPFillRectangle(gfx++, gScreenWidth - pillarboxSize, 0, gScreenWidth - 1, gScreenHeight - 1);

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 0);

        gDPFillRectangle(gfx++, pillarboxSize, 0, pillarboxSize + 2, gScreenHeight);
        gDPFillRectangle(gfx++, gScreenWidth - pillarboxSize - 2, 0, gScreenWidth - pillarboxSize, gScreenHeight);

        gDPPipeSync(gfx++);
        OVERLAY_DISP = gfx++;

        CLOSE_DISPS(gfxCtx);
    }
}
