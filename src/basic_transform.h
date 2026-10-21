#ifndef BASIC_TRANSFORM_H
#define BASIC_TRANSFORM_H

#include <cassert>

#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // TODO: Determine if possible in general, else provide counterexample:
    // compose(f, g) is equivalent to first applying g, then applying f.

    // BasicTransforms are closed under composition because the result can always be represented by another BasicTransform
    static BasicTransform compose(const BasicTransform& f, const BasicTransform& g) {
        return {
            {f.scale.x*g.scale.x, f.scale.y*g.scale.y}, // scale = f.scale*g.scale
            {f.scale.x*g.translation.x+f.translation.x, f.scale.y*g.translation.y+f.translation.y}
            // translation = f.scale*g.translation+f.translation
        };
    };

    // TODO: Determine if possible in general, else provide counterexample:
    // BasicTransforms with nonzero scale components are closed under inversion. The inverse also has nonzero scale 
    // components, since the reciprocal of a nonzero number is nonzero
    static BasicTransform inverse(const BasicTransform& f) {
        assert(f.scale.x != 0.f && f.scale.y != 0.f);
        return {
            {1.f / f.scale.x, 1.f / f.scale.y}, // scale = 1/f.scale
            {-f.translation.x / f.scale.x, -f.translation.y / f.scale.y} // translation = -f.translation/f.scale
        };
    };

    constexpr BasicTransform(const sf::Vector2f& scale = {1.f, 1.f},
                             const sf::Vector2f& translation = {0.f, 0.f})
        : scale{scale}, translation{translation} {}

    constexpr sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        // first scale, then translate -- if you do the reverse, the translation gets messed up.
        return {p.x * scale.x + translation.x, p.y * scale.y + translation.y};
    }

    constexpr sf::Vector2f applyVector(const sf::Vector2f& p) const {
        // vectors are not translated by transformations
        return {p.x * scale.x, p.y * scale.y};
    }

    constexpr bool operator==(const BasicTransform& other) const {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};

#endif