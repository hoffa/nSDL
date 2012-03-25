nSDL includes a few useful TI-Nspire-only functions (i.e., functions not part of standard SDL). Currently nSDL supports drawing text using different fonts. It has been designed to be easily extensible, so adding your own fonts shouldn't be an issue.

<table>

<tr><td><b>Return type</b></td><td><b>Name</b></td><td><b>Parameters</b></td><td><b>Notes</b></td></tr>

<tr><td><code>int</code></td><td><code>SDL_nCreatePalette</code></td><td><pre>(SDL_Surface *surface)</pre></td><td>Should be used on surfaces explicitly created with <code>SDL_CreateRGBSurface()</code> if compiling for Touchpad/Clickpad models. Returns 0 if there was an error.</td></tr>

<tr><td><code>SDL_nFont *</code></td><td><code>SDL_nLoadFont</code></td><td><pre>(int font_index,
 Uint32 color,
 Uint32 flags)</pre></td><td>Loads a font. <code>font_index</code> should be either <code>NSP_FONT_THIN</code>, <code>NSP_FONT_SPACE</code>, <code>NSP_FONT_VGA</code>, <code>NSP_FONT_FANTASY</code> or <code>NSP_FONT_TINYTYPE</code>. If <code>flags</code> is <code>NSP_FONT_DEFAULT</code>, default font settings are used (transparent background, no text wrap). Other possible values for <code>flags</code> are <code>NSP_FONT_OPAQUE</code> and <code>NSP_FONT_TEXTWRAP</code>, which can be OR'd together. Returns NULL if there was an error.</td></tr>

<tr><td><code>void</code></td><td><code>SDL_nFreeFont</code></td><td><pre>(SDL_nFont *font)</pre></td><td>Frees a font from memory.</td></tr>

<tr><td><code>int</code></td><td><code>SDL_nDrawChar</code></td><td><pre>SDL_Surface *surface,
 SDL_nFont *font,
 SDL_Rect *pos,
 int c</pre></td><td>Draws a character to <code>surface</code>. Returns -1 if the character could not be drawn.</td></tr>

<tr><td><code>int</code></td><td><code>SDL_nDrawString</code></td><td><pre>
(SDL_Surface *surface,
SDL_nFont *font,
int x,
int y,
const char *format,
...)</pre></td><td>Draws a string to <code>surface</code>. Supports printf-like string formatting. Returns -1 if the whole string could not be drawn.</td></tr>

</table>