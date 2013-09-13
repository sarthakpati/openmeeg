/*
Project Name : OpenMEEG

© INRIA and ENPC (contributors: Geoffray ADDE, Maureen CLERC, Alexandre
GRAMFORT, Renaud KERIVEN, Jan KYBIC, Perrine LANDREAU, Théodore PAPADOPOULO,
Emmanuel OLIVI
Maureen.Clerc.AT.sophia.inria.fr, keriven.AT.certis.enpc.fr,
kybic.AT.fel.cvut.cz, papadop.AT.sophia.inria.fr)

The OpenMEEG software is a C++ package for solving the forward/inverse
problems of electroencephalography and magnetoencephalography.

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's authors,  the holders of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
*/

#include <triangle.h>

namespace OpenMEEG {

    Triangle::Triangle(const Triangle& t) {
        *this = t;
    }

    Triangle::Triangle(Vertex *pts[3]): index_(-1) {
        for ( unsigned i = 0; i < 3; ++i) {
            vertices_[i] = pts[i];
        }
    }

    Triangle::Triangle(Vertex& p1, Vertex& p2, Vertex& p3): index_(-1) {
        vertices_[0] = &p1;
        vertices_[1] = &p2;
        vertices_[2] = &p3;
    }

    Triangle::Triangle(Vertex * p1, Vertex * p2, Vertex * p3): index_(-1) {
        vertices_[0] = p1;
        vertices_[1] = p2;
        vertices_[2] = p3;
    }

    Triangle& Triangle::operator=(const Triangle& t) {
        if ( this != &t ) {
            copy(t);
        }
        return *this;
    }

    void Triangle::copy(const Triangle& t) {
        area_   = t.area();
        normal_ = t.normal();
        index_  = t.index();
        for ( unsigned i = 0; i < 3; ++i) {
            vertices_[i] = const_cast<Vertex *>(t[i]);
        }
    }
    
    void Triangle::destroy() { }

    const bool Triangle::operator==(const Triangle& T) const {
        for ( Triangle::const_iterator i1 = begin(), i2 = T.begin(); i1 != end(); ++i1, ++i2) {
            if ( **i1 != **i2 ) {
                return false;
            }
        }
        return true;
    }

    void Triangle::flip() {
        Vertex * tmp = vertices_[0];
        vertices_[0] = vertices_[1];
        vertices_[1] = tmp;
    }
}