#include <pa/common.hpp>

namespace pa::raylib {

inline void Texture_Deleter(Texture2D *texture) {
    UnloadTexture(*texture);
    delete[] texture;
}

}
