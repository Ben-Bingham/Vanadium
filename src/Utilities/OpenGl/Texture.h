#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <stb_image.h>

template<int TEXTURE_TYPE>
class Texture {
public:
    enum class Format {
        R = GL_RED,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    enum class StorageType {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        FLOAT = GL_FLOAT
    };

    enum class WrapMode {
        REPEAT = GL_REPEAT
    };

    enum class FilteringMode {
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST
    };

    struct Parameters {
        Parameters(
            Format imageFormat = Format::RGBA, 
            StorageType internalStorageType = StorageType::UNSIGNED_BYTE, 
            WrapMode wrapMode = WrapMode::REPEAT, 
            FilteringMode filteringMode = FilteringMode::LINEAR
        )
            : imageFormat{ imageFormat }
            , internalStorageType(internalStorageType)
            , horizontalWrapMode(wrapMode)
            , verticalWrapMode(wrapMode)
            , minFilter(filteringMode)
            , magFilter(filteringMode) { }

        Format imageFormat{ };
        StorageType internalStorageType{ };

        WrapMode horizontalWrapMode{ };
        WrapMode verticalWrapMode{ };

        FilteringMode minFilter{ };
        FilteringMode magFilter{ };
    };

    Texture(glm::ivec2 size, Parameters parameters = Parameters{ }, std::vector<unsigned char> data = std::vector<unsigned char>{ }) {
        glGenTextures(1, &m_TextureHandle);
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);

        // Set default parameters
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, (int)parameters.horizontalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, (int)parameters.verticalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, (int)parameters.minFilter);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, (int)parameters.magFilter);

        if (data.empty()) {
            glTexImage2D(TEXTURE_TYPE, 0, (int)parameters.imageFormat, size.x, size.y, 0, (int)parameters.imageFormat, (int)parameters.internalStorageType, nullptr);
        }
        else {
            glTexImage2D(TEXTURE_TYPE, 0, (int)parameters.imageFormat, size.x, size.y, 0, (int)parameters.imageFormat, (int)parameters.internalStorageType, data.data());
        }
    }

    Texture(const std::string& path, Parameters parameters = Parameters{ }, bool flip = true) {
        glGenTextures(1, &m_TextureHandle);
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);

        // Set default parameters
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, (int)parameters.horizontalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, (int)parameters.verticalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, (int)parameters.minFilter);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, (int)parameters.magFilter);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
        if (data) {
            glTexImage2D(TEXTURE_TYPE, 0, (int)parameters.imageFormat, width, height, 0, GL_RGB, (int)parameters.internalStorageType, data);
            glGenerateMipmap(TEXTURE_TYPE);
        }
        else {
            std::cout << "Failed to load texture: " << path << std::endl;
        }
        stbi_image_free(data);
    }

    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = default;

    ~Texture() {
        glDeleteTextures(1, &m_TextureHandle);
    }

    void Bind() {
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);
    }

    unsigned int Get() {
        return m_TextureHandle;
    }

private:
    unsigned int m_TextureHandle;
};

using Texture2D = Texture<GL_TEXTURE_2D>;