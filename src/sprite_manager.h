#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sprite.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>


using SpriteID = size_t;

class SpriteManager {
public:
    sf::Texture& registerTexture(const std::string& filepath) {
        // Check if the filepath already exists. If yes, return the existing texture
        auto it = mTextures.find(filepath);
        if (it != mTextures.end()) {
            return *it->second;
        }

        // Otherwise create and load a new texture
        // Store the texture in a unique pointer since we need to keep the texture at a stable memory address
        auto texture = std::make_unique<sf::Texture>(); 
        if (!texture->loadFromFile(filepath)) {
            throw std::runtime_error("Failed to load texture: " + filepath);
        }
        // reference to the texture obj
        sf::Texture& result = *texture;
        // stores the texture's unique_ptr in the map under the filepath
        mTextures[filepath] = std::move(texture);
        return result;
    }
    template <typename... Args>
    SpriteID createSprite(Args&&... args) {
        // construct a Sprite directly in the vector
        mSprites.emplace_back(std::forward<Args>(args)...);
        // the index of the new sprite
        return mSprites.size() - 1;
    }
    Sprite& getSprite(SpriteID id) {
        return mSprites.at(id);
    }
private:
        // Store textures by filepath to avoid duplicate registrations
        std::unordered_map<std::string, std::unique_ptr<sf::Texture>> mTextures;

        // Store sprites in slots indexed by SpriteID
        std::vector<Sprite> mSprites;
};

#endif  // SPRITE_MANAGER_H
