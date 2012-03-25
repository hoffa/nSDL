nSDL includes a few useful TI-Nspire-only functions (i.e., functions not part of standard SDL). Currently nSDL supports drawing text using different fonts. It has been designed to be easily extensible, so adding your own fonts shouldn't be an issue.

<table>

<tr><td><b>Return type</b></td><td><b>Name</b></td><td><b>Parameters</b></td><td><b>Notes</b></td></tr>

<tr><td>`int`</td><td>`SDL_nCreatePalette`</td><td>`(SDL_Surface *surface)`</td><td>Should be used on surfaces explicitly created with `SDL_CreateRGBSurface()` if compiling for Touchpad/Clickpad models. Returns 0 if there was an error.</td></tr>

<tr><td>`SDL_nFont *`</td><td>`SDL_nLoadFont`</td><td>`(int font_index, Uint32 color, Uint32 flags)`</td><td>Loads a font. `font_index` should be either `NSP_FONT_THIN`, `NSP_FONT_SPACE`, `NSP_FONT_VGA`, `NSP_FONT_FANTASY` or `NSP_FONT_TINYTYPE`. If `flags` is `NSP_FONT_DEFAULT`, default font settings are used (transparent background, no text wrap). Other possible values for `flags` are `NSP_FONT_OPAQUE` and `NSP_FONT_TEXTWRAP`, which can be OR'd together. Returns NULL if there was an error.</td></tr>

<tr><td>`void`</td><td>`SDL_nFreeFont`</td><td>`(SDL_nFont *font)`</td><td>Frees a font from memory.</td></tr>

<tr><td>`int`</td><td>`SDL_nDrawChar`</td><td>`(SDL_Surface *surface, SDL_nFont *font, SDL_Rect *pos, int c)`</td><td>Draws a character to `surface`. Returns -1 if the character could not be drawn.</td></tr>

<tr><td>`int`</td><td>`SDL_nDrawString`</td><td>`(SDL_Surface *surface, SDL_nFont *font, int x, int y, const char *format, ...)`</td><td>Draws a string to `surface`. Supports printf-like string formatting. Returns -1 if the whole string could not be drawn.</td></tr>

</table>