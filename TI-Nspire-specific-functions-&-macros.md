
nSDL includes a few useful TI-Nspire-only functions (i.e., functions not part of standard SDL). Currently nSDL supports drawing text using different fonts. It has been designed to be easily extensible, so adding your own fonts shouldn't be an issue.

<table>

<tr><td><b>Return type</b></td><td><b>Name</b></td><td><b>Parameters</b></td><td><b>Notes</b></td></tr>

<tr><td><font=monospace>int</font></td><td><font=monospace>SDL_nCreatePalette</font></td><td><font=monospace>(SDL_Surface *surface)</font></td><td>Should be used on surfaces explicitly created with <font=monospace>SDL_CreateRGBSurface()</font> if compiling for Touchpad/Clickpad models. Returns 0 if there was an error.</td></tr>

<tr><td><font=monospace>SDL_nFont *</font></td><td><font=monospace>SDL_nLoadFont</font></td><td><font=monospace>(int font_index, Uint32 color, Uint32 flags)</font></td><td>Loads a font. <font=monospace>font_index</font> should be either <font=monospace>NSP_FONT_THIN</font>, <font=monospace>NSP_FONT_SPACE</font>, <font=monospace>NSP_FONT_VGA</font>, <font=monospace>NSP_FONT_FANTASY</font> or <font=monospace>NSP_FONT_TINYTYPE</font>. If <font=monospace>flags</font> is <font=monospace>NSP_FONT_DEFAULT</font>, default font settings are used (transparent background, no text wrap). Other possible values for <font=monospace>flags</font> are <font=monospace>NSP_FONT_OPAQUE</font> and <font=monospace>NSP_FONT_TEXTWRAP</font>, which can be OR'd together. Returns NULL if there was an error.</td></tr>

<tr><td><font=monospace>void</font></td><td><font=monospace>SDL_nFreeFont</font></td><td><font=monospace>(SDL_nFont *font)</font></td><td>Frees a font from memory.</td></tr>

<tr><td><font=monospace>int</font></td><td><font=monospace>SDL_nDrawChar</font></td><td><font=monospace>(SDL_Surface *surface, SDL_nFont *font, SDL_Rect *pos, int c)</font></td><td>Draws a character to <font=monospace>surface</font>. Returns -1 if the character could not be drawn.</td></tr>

<tr><td><font=monospace>int</font></td><td><font=monospace>SDL_nDrawString</font></td><td><font=monospace>(SDL_Surface *surface, SDL_nFont *font, int x, int y, const char *format, ...)</font></td><td>Draws a string to <font=monospace>surface</font>. Supports printf-like string formatting. Returns -1 if the whole string could not be drawn.</td></tr>

</table>