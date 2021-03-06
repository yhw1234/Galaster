#ifndef _FONT_H_
#define _FONT_H_


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

#include <map>
#include <string>


// 
// Bitmaped glyphs are cached for future usage, so that we can load glyphs as
// textures directly from cache, which is much faster than rendering glyphs to
// bitmaps every time we want to display a character.
// 
class fontcache
{
public:
    fontcache();
    struct facecache;

    // cached freetype glyph and pre-rendered opengl texture
    struct glyph {
        glyph(FT_Glyph gly = nullptr, unsigned tex = 0)
            : gly(gly), tex(tex) { }
        FT_Glyph gly;
        unsigned tex;
    };

    // cache particular glyph of various sizes 
    struct glyphcache {
        glyphcache(facecache *glyph_cache, wchar_t ch)
            : glyph_cache(glyph_cache), ch(ch) {
        }
        glyph get_glyph(int size);

        facecache *glyph_cache;
        wchar_t ch;
        std::map<size_t, glyph> c_glyph;
    };

    // cache loaded font faces
    struct facecache {
        facecache(fontcache *font_cache, FT_Face face)
            : font_cache(font_cache), face(face) {
        }
        glyphcache *get_char(wchar_t ch);
        FT_Glyph load_glyph(wchar_t ch, size_t size);
        
        fontcache *font_cache;
        FT_Face face;
        std::map<wchar_t, glyphcache*> c_face;
    };

    glyph glyph_of(const std::string &fontpath, wchar_t ch, size_t size);
    facecache *get_face(const std::string &fontpath);

protected:
    FT_Library library;
    std::map<std::string, facecache*> c_font;
};



#endif /* _FONT_H_ */
