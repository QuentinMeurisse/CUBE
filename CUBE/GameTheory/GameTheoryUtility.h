#ifndef CUBE_GAMETHEORYUTILITY_H
#define CUBE_GAMETHEORYUTILITY_H


#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <deque>
#include <memory>
#include <map>

#include "UrbanBlockGame.h"

class GameTheoryUtility {
public:
    static double norm(OpenMesh::PolyMesh_ArrayKernelT<>::Point const& p);
    /**
     *
     * @param mesh
     * @param fid
     * @param radius
     * @return all the cells that are at most radius step from fid (fid not included)
     */

    static std::deque<unsigned int>
    faceBall(std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>> const& mesh, const unsigned int &fid, const unsigned int &radius);

    /**
     *
     * @param mesh
     * @param fid
     * @param radius
     * @return a map with the distance between 1 and radius as keys. For a key k , map[k] is the list of all faces at
     * distance k from face fid
     */
    static std::map<unsigned int, std::deque<unsigned int>>
    faceDistances(std::shared_ptr<OpenMesh::PolyMesh_ArrayKernelT<>> const& mesh, const unsigned int &fid, const unsigned int &radius);

    static void generateRandomGame(const std::shared_ptr<UrbanBlockGame> &ubg, unsigned int const &radius = 0);

    static void generateRandomGameWithGreen(const std::shared_ptr<UrbanBlockGame> &ubg,
                                            unsigned int const& rb = 0,
                                            unsigned int const& rg = 0);

    /**
     *
     * @param q1
     * @param q2
     * @return return the intersection between q1 and q2 (used to intersect two balls)
     */
    static std::deque<unsigned int> qIntersection(std::deque<unsigned int > const& q1, std::deque<unsigned int > const& q2);


};


#endif //CUBE_GAMETHEORYUTILITY_H
