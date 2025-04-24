#ifndef VERTICE_H
#define VERTICE_H


class Vertice
{
  public:
    float x, y, z, texX, texY;

    Vertice()
    {

    }

    Vertice(float x, float y, float z, float texX, float texY):
      x(x), y(y), z(z), texX(texX), texY(texY)
    {

    }

    bool operator==(const Vertice vertice)
    {
      if (this->x != vertice.x) {
        return false;
      } else if (this->y != vertice.y) {
        return false;
      } else if (this->z != vertice.z) {
        return false;
      } else if (this->texX != vertice.texX) {
        return false;
      } else if (this->texY != vertice.texY) {
        return false;
      }
      return true;
    }

    bool operator!=(const Vertice vertice)
    {
      return !(*this == vertice);
    }
  protected:

  private:
};

#endif // VERTICE_H
