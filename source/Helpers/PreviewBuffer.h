#ifndef __PreviewBuffer_H__
#define __PreviewBuffer_H__

#include <map>
#include <SDL.h>
#include <shared_mutex>
#include <thread>
#include <utility>
#include <atomic>
#include "Beatmaps/BeatmapManager.h"

#ifdef DESKTOP
#define PICTURE_CACHE_MAX_SIZE_BYTES (512 * 1024 * 1024) // 512 MB
#endif

#ifdef VITA
#define PICTURE_CACHE_MAX_SIZE_BYTES (16 * 1024 * 1024) // 16 MB
#endif

typedef struct PreviewBufferState {
    int FirstLoadedTexId = -1;
    int LastLoadedTexId = -1;
    int UsedUpBytes = 0;
    bool FillBufferInitialDone = false;
} PreviewBufferState;

class PreviewBuffer
{
    public:
        void Init();
        void Update(int last_page, int new_page, int per_page);
        void Pics_ResetBuffer();
        SDL_Surface* GetTexture(int beatmap_id);

        BeatmapFilter lastAppliedFilter = FILTER_NONE;
        static std::string GeneratePreview(const std::string& map_subdir, const std::string& BackgroundFilename, const std::string& Checksum);
        static PreviewBuffer& GetInstance() { return sPreviewBuffer; }

    protected:
        PreviewBufferState pbs;
        std::shared_mutex mut_pbs;

        SDL_Surface* preview_default;
        uint32_t window_pixelformat;

        std::map<int, SDL_Surface*> buf;
        std::shared_mutex mut_buf;

        std::atomic<bool> processingNow = false;
        std::atomic<bool> wantTerminate = false;

        static PreviewBuffer sPreviewBuffer;

        SDL_Surface* LoadSquare(const std::string& path);
        void Pics_FillBuffer(int last_page, int new_page, int per_page);
        void Pics_ClearBuffer();

    private:
        PreviewBuffer();
        ~PreviewBuffer() { Pics_ClearBuffer(); };
};

#endif

