#ifndef MESH_H
#define MESH_H

#include <vector>
#include <array>

#include "Vertice.h"
#include "VariableTypes.h"

class Mesh
{
  public:
    std::array<std::vector<Vertice>, BlockType::NumTypes> vertices;

  protected:

  private:
};

#endif // MESH_H
