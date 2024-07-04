#include "gba/m4a_internal.h"
#include "constants/songs.h"

static void DisableTrack(int trackIndex, int numTracks, struct MusicPlayerTrack *tracks)
{
    if (trackIndex < numTracks)
    {
        tracks[trackIndex].disabled = TRUE;
    }
}

void m4aInitDynamicTracks(int numTracks, struct MusicPlayerTrack *tracks, u16 songId)
{
    if (songId == MUS_RAVE)
    {
        // Check vars etc. to determine which tracks should be disabled.
        // DisableTrack(0, numTracks, tracks);
    }
}
