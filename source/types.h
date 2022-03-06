#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

typedef enum BeatmapFilter {
    FILTER_NONE,
    FILTER_ALPHA_A_E,
    FILTER_ALPHA_F_J,
    FILTER_ALPHA_K_O,
    FILTER_ALPHA_P_T,
    FILTER_ALPHA_U_Z
} BeatmapFilter;

#endif // __TYPES_H__
