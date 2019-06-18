#ifndef GUARD_BALL_SEALS_SCREEN_H
#define GUARD_BALL_SEALS_SCREEN_H

void OpenBallSealScreen(void (*callback)(void));
void SetBallSealScreenEnterState(u8 state);
u8 GetNumOwnedSeals(void);

#endif  // GUARD_BALL_SEALS_SCREEN_H
