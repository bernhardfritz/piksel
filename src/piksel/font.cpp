#include "font.hpp"
#include <stb/stb_truetype.h>

namespace piksel {

Font::~Font() {
    delete info;
}

int Font::load(std::string filename) {
    long size;
    unsigned char* fontBuffer;
    FILE* fontFile = fopen(filename.c_str(), "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    fontBuffer = (unsigned char *) malloc(size);
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);
    info = new stbtt_fontinfo();
    if (!stbtt_InitFont(info, fontBuffer, 0)) {
        fputs("Failed to load font", stderr);
        return 0;
    }
    stbtt_GetFontVMetrics(info, &ascent, &descent, &lineGap);
    stbtt_GetFontBoundingBox(info, &x0, &y1, &x1, &y1);

    for (char ch = ' '; ch <= '~'; ch++) {
        stbtt_vertex* vertices;
        int num_verts = stbtt_GetGlyphShape(info, stbtt_FindGlyphIndex(info, ch), &vertices);
        
        msdfgen::Shape shape;
        msdfgen::Contour contour;
        msdfgen::EdgeHolder edge;
        msdfgen::Point2 p[4];
        for (int i = 0; i < num_verts; i++) {
            stbtt_vertex* vertex = vertices + i;
            switch(vertex->type) {
                case STBTT_vmove:
                    if (i > 0) {
                        shape.addContour(contour);
                        contour = msdfgen::Contour();
                    }
                    break;
                case STBTT_vline:
                    p[1] = msdfgen::Point2((double) vertex->x, (double) vertex->y);
                    contour.addEdge(msdfgen::EdgeHolder(p[0], p[1]));
                    break;
                case STBTT_vcurve:
                    p[1] = msdfgen::Point2((double) vertex->cx, (double) vertex->cy);
                    p[2] = msdfgen::Point2((double) vertex->x, (double) vertex->y);
                    contour.addEdge(msdfgen::EdgeHolder(p[0], p[1], p[2]));
                    break;
                case STBTT_vcubic:
                    p[1] = msdfgen::Point2((double) vertex->cx, (double) vertex->cy);
                    p[2] = msdfgen::Point2((double) vertex->cx1, (double) vertex->cy1);
                    p[3] = msdfgen::Point2((double) vertex->x, (double) vertex->y);
                    edge = msdfgen::EdgeHolder(p[0], p[1], p[2], p[3]);
                    contour.addEdge(edge);
                    break;
            }
            p[0] = msdfgen::Point2((double) vertex->x, (double) vertex->y);
        }
        if (num_verts > 0) {
            shape.addContour(contour);
        }

        Letter letter;
        registerShape(shape, letter.shape);

        stbtt_GetGlyphHMetrics(info, stbtt_FindGlyphIndex(info, ch), &letter.advanceWidth, &letter.leftSideBearing);
        stbtt_GetGlyphBox(info, stbtt_FindGlyphIndex(info, ch), &letter.x0, &letter.y0, &letter.x1, &letter.y1);

        letters.push_back(letter);
    }

    return 1;
}

} // namespace nv
