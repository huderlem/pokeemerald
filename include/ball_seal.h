#ifndef GUARD_BALL_SEALS_H
#define GUARD_BALL_SEALS_H

#define BALL_SEAL_TYPE_STATIC_ICON (1 << 0)

void LoadBallSealGraphics(u8, u8, u8, u8);
void StartBallSealAnimation(u8, u8, u8, u8, u8, u8, u8, u8, int);

#define BALL_SEAL_DURATION 51

#endif  // GUARD_BALL_SEALS_H
