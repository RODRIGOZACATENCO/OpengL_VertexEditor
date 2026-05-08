#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "stb_image.h"

class Texture2d
{
public:
    unsigned int textureID;
    unsigned int activeTexture;
    int width, height, nrchannels;
    Texture2d(std::string texturePath, unsigned int aTexture, bool isTransparent = 0)
    {
        activeTexture = aTexture;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrchannels, 0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLenum format = GL_RED;
        if (isTransparent)
        {
            format = GL_RGBA;
        }
        else if (nrchannels == 1)
            format = GL_RED;
        else if (nrchannels == 3)
            format = GL_RGB;
        else if (nrchannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    void use()
    {
        glActiveTexture(GL_TEXTURE0 + activeTexture);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    void setWrapMode(GLint wrapS, GLint WrapT) const
    {
        glActiveTexture(GL_TEXTURE0 + activeTexture);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

class TextureCubeMap
{
public:
    unsigned int textureID;
    unsigned int activeTexture;
    int width, height, nrchannels;
    TextureCubeMap(std::vector<std::string> texturePaths, unsigned int aTexture, bool isTransparent = 0)
    {
        activeTexture = aTexture;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for (int i = 0; i < texturePaths.size(); i++)
        {
            unsigned char *data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrchannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap failed to load at path: " << texturePaths[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                        GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    void use()
    {
        glActiveTexture(GL_TEXTURE0 + activeTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    }
};
#endif // TEXTUREHANDLER_H
