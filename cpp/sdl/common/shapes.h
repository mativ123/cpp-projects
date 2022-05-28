class Box
{
    SDL_Rect box;
    public:
        SDL_Color color { 0, 0, 0, SDL_ALPHA_OPAQUE };
        bool fill { false };
        int x { 0 };
        int y { 0 };
        int w { 0 };
        int h { 0 };

        void draw(SDL_Renderer *rendere)
        {
            box = { x, y, w, h};
            SDL_SetRenderDrawColor(rendere, color.r, color.g, color.b, color.a);
            SDL_RenderDrawRect(rendere, &box);
            SDL_SetRenderDrawColor(rendere, 255, 255, 255, SDL_ALPHA_OPAQUE);
        }
};
