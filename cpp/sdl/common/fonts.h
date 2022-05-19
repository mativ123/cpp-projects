class Text
{
    TTF_Font *font { nullptr };
    SDL_Rect textRect;
    public:
        SDL_Color color { 0, 0, 0, SDL_ALPHA_OPAQUE };
        std::string textString;
        int x { 0 };
        int y { 0 };
        int fontSize { 20 };

    void init(SDL_Renderer *rendere, const char *src)
    {
        font = TTF_OpenFont(src, fontSize);

        textRect.x = x;
        textRect.y = y;
    }

    void draw(SDL_Renderer *rendere, std::string textString)
    {
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, textString.c_str(), color);
        SDL_Texture *text = SDL_CreateTextureFromSurface(rendere, textSurface);

        SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
        
        SDL_FreeSurface(textSurface);
        textSurface = nullptr;

        SDL_RenderCopy(rendere, text, NULL, &textRect);

        SDL_DestroyTexture(text);
        text = nullptr;
    }

};
