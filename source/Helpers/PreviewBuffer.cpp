#include <SDL_timer.h>
#include "PreviewBuffer.h"
#include "Beatmaps/BeatmapManager.h"

PreviewBuffer PreviewBuffer::sPreviewBuffer;

PreviewBuffer::PreviewBuffer() {

}

void PreviewBuffer::Init() {
    preview_default = IMG_Load( "data/ui/preview.png" );
    window_pixelformat = SDL_GetWindowPixelFormat(window);

    processingNow.store(true, std::memory_order_seq_cst);
    std::unique_lock ulock(mut_buf);
    for (int i = 0; i < BeatmapManager::SongCount(); ++i) {
        buf[i] = preview_default;
    }
    processingNow.store(false, std::memory_order_seq_cst);
}

SDL_Surface* PreviewBuffer::GetTexture(int beatmap_id) {
    std::shared_lock lock(mut_buf);
    return buf.at(beatmap_id);
}

void PreviewBuffer::Update(int last_page, int new_page, int per_page) {
    std::thread t1(&PreviewBuffer::Pics_FillBuffer, this, last_page, new_page, per_page);
    t1.detach();
    printf("PreviewBuffer thread started\n");
}

void PreviewBuffer::Pics_ClearBuffer() {
    printf("Clearing buffer...\n");
    for (auto const& x : buf) {
        if (preview_default != x.second) {
            SDL_FreeSurface(x.second);
        }
    }
    buf.clear();

    SDL_FreeSurface(preview_default);
}

SDL_Surface* PreviewBuffer::LoadSquare(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (loadedSurface == nullptr) {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return loadedSurface;
    }

    SDL_Rect srcrect;
    srcrect.h = srcrect.w = loadedSurface->h;
    srcrect.y = 0;
    srcrect.x = floor(((float)loadedSurface->w / 2.f) - ((float)loadedSurface->h / 2.f));

    SDL_Rect dstrect;
    dstrect.h = dstrect.w = srcrect.h;
    dstrect.y = dstrect.x = 0;

    SDL_Surface* cropped = SDL_CreateRGBSurfaceWithFormat(0, dstrect.w, dstrect.h, 8, window_pixelformat);

    SDL_BlitSurface(loadedSurface,&srcrect,cropped,&dstrect);
    SDL_FreeSurface(loadedSurface);
    return cropped;
}

void PreviewBuffer::Pics_FillBuffer(int last_page, int new_page, int per_page) {
    while ( processingNow.load(std::memory_order_seq_cst) ){
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    processingNow.store(true, std::memory_order_seq_cst);

    if (last_page == -1 && new_page == 0 && pbs.UsedUpBytes == 0) {
        {
            std::unique_lock lock(mut_pbs);
            pbs.FirstLoadedTexId = 0;
        }

        for (int i = 0; i < BeatmapManager::SongCount(); ++i) {
            Beatmap* map = BeatmapManager::Beatmaps()[i];
            SDL_Surface* tex = LoadSquare(map->BackgroundFilename());

            {
                std::unique_lock lock_pbs(mut_pbs);
                std::unique_lock lock_buf(mut_buf);

                if (tex == nullptr) {
                    buf[i] = preview_default;
                    pbs.LastLoadedTexId = i;
                    continue;
                }

                pbs.UsedUpBytes += (tex->w * tex->h * 4);
                if (pbs.UsedUpBytes >= PICTURE_CACHE_MAX_SIZE_BYTES) {
                    printf("Reached PICTURE_CACHE_MAX_SIZE_BYTES at index %i\n", i);
                    SDL_FreeSurface(tex);
                    break;
                } else {
                    buf[i] = tex;
                    pbs.LastLoadedTexId = i;
                }
            }
        }

        {
            std::unique_lock lock(mut_pbs);
            pbs.FillBufferInitialDone = true;
        }
        printf("FillBufferInitialDone\n");
        processingNow.store(false, std::memory_order_seq_cst);
        return;
    }

    {
        std::unique_lock lock_pbs(mut_pbs);
        std::unique_lock lock_buf(mut_buf);

        if (last_page < new_page) {
            // Page moved forward.
            // We have to make sure that we have cache for 3 next pages.
            int page_offset = ((new_page + 3) * per_page) + per_page;
            if (page_offset >= BeatmapManager::SongCount()) page_offset = (BeatmapManager::SongCount() - 1);

            if (pbs.LastLoadedTexId >= page_offset) {
                processingNow.store(false, std::memory_order_seq_cst);
                return;
            }

            do {
                if (buf.at(pbs.FirstLoadedTexId) != preview_default) {
                    SDL_FreeSurface(buf.at(pbs.FirstLoadedTexId));
                    buf[pbs.FirstLoadedTexId] = preview_default;
                }

                pbs.FirstLoadedTexId++;

                int texid = pbs.LastLoadedTexId + 1;
                Beatmap *map = BeatmapManager::Beatmaps()[texid];
                SDL_Surface *tex = LoadSquare(map->BackgroundFilename());

                buf[texid] = tex;
                pbs.LastLoadedTexId++;
            } while (pbs.LastLoadedTexId < page_offset);
        }

        if (last_page > new_page) {
            // Page moved backward.
            // We have to make sure that we have cache for 3 previous pages.
            int page_offset = ((new_page - 3) * per_page);
            if (page_offset < 0) page_offset = 0;

            if (pbs.FirstLoadedTexId <= page_offset) {
                processingNow.store(false, std::memory_order_seq_cst);
                return;
            }

            do {
                if (buf.at(pbs.LastLoadedTexId) != preview_default) {
                    SDL_FreeSurface(buf.at(pbs.LastLoadedTexId));
                    buf[pbs.LastLoadedTexId] = preview_default;
                }

                pbs.LastLoadedTexId--;

                int texid = pbs.FirstLoadedTexId - 1;
                Beatmap *map = BeatmapManager::Beatmaps()[texid];
                SDL_Surface *tex = LoadSquare(map->BackgroundFilename());

                buf[texid] = tex;
                pbs.FirstLoadedTexId--;
            } while (pbs.FirstLoadedTexId > page_offset);
        }
    }

    processingNow.store(false, std::memory_order_seq_cst);
}


