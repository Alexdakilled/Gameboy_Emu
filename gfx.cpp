#include "gfx.h"
#include "core.h"
#include <cstdlib>


gfx_context* gfx_init(unsigned int width, unsigned int height, const char* windowTitle)
{
    auto* ctx = static_cast<gfx_context *>(calloc(1, sizeof(gfx_context)));

    if (!ctx)
    {
        Corelog << "GFX context failed to initialize\n";
        return nullptr;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    if (!SDL_CreateWindowAndRenderer(windowTitle, width, height, 0, &ctx->window, &ctx->renderer))
    {
        Corelog << "GFX Unable to initialize Window or Renderer: " << SDL_GetError() << '\n';
        gfx_destroy(ctx);
        return nullptr;
    }

    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!ctx->texture)
    {
        Corelog << "GFX Unable to initialize Texture: " << SDL_GetError() << '\n';
        gfx_destroy(ctx);
        return nullptr;
    }
    SDL_SetTextureScaleMode(ctx->texture, SDL_SCALEMODE_NEAREST);

    //SDL_SetRenderVSync(ctx->renderer, 1);

    return ctx;
}

void gfx_clear(gfx_context* ctx, SDL_Color color)
{
    SDL_SetRenderDrawColor(ctx->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(ctx->renderer);
}

void gfx_putpixel(gfx_context* ctx, unsigned int x, unsigned int y, SDL_Color color)
{
    SDL_SetRenderDrawColor(ctx->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderPoint(ctx->renderer, x, y);
}

void gfx_update(gfx_context* ctx)
{
    SDL_RenderPresent(ctx->renderer);
}

void gfx_destroy(gfx_context* ctx)
{
    if (!ctx) return;
    if (ctx->texture) SDL_DestroyTexture(ctx->texture);
    if (ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
    if (ctx->window) SDL_DestroyWindow(ctx->window);
    free(ctx);
}
