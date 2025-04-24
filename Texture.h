#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
  private:
  public:
    Texture();
    void load(const char* filename);
    void bind();

    unsigned int texture;
    int width, height, nrChannels;
    unsigned char *data;
  protected:
};

#endif // TEXTURE_H
