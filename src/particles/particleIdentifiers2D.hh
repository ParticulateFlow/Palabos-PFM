/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2015 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PARTICLE_IDENTIFIERS_2D_HH
#define PARTICLE_IDENTIFIERS_2D_HH

#include "core/blockIdentifiers.h"
#include "core/runTimeDiagnostics.h"
#include <sstream>

namespace plb {

namespace meta {

template<typename T, template<typename U> class Descriptor>
ParticleRegistration2D<T,Descriptor>::~ParticleRegistration2D()
{
    for (pluint iEntry=0; iEntry<particleByNumber.size(); ++iEntry) {
        delete particleByNumber[iEntry].generator;
    }
}

template<typename T, template<typename U> class Descriptor>
int ParticleRegistration2D<T,Descriptor>::announce (
        std::string nameOfParticle,
        ParticleGenerator2D<T,Descriptor>* generator )
{
    Entry entry(nameOfParticle, generator);
    typename EntryMap::iterator it = particleByName.find(entry);
    if (it != particleByName.end()) {
        plbLogicError( std::string("The particle class ") + nameOfParticle +
                       std::string(" was registered twice") );
    }
    particleByNumber.push_back(entry);
    int nextId = particleByNumber.size();
    particleByName[entry] = nextId;
    return nextId;
}

template<typename T, template<typename U> class Descriptor>
int ParticleRegistration2D<T,Descriptor>::getId(std::string name) const
{
    Entry entry(name, 0);
    typename EntryMap::const_iterator it = particleByName.find(entry);
    if (it == particleByName.end()) {
        return 0;
    }
    else {
        return it->second;
    }
}

template<typename T, template<typename U> class Descriptor>
int ParticleRegistration2D<T,Descriptor>::getNumId() const
{
    return (int)(particleByNumber.size());
}

template<typename T, template<typename U> class Descriptor>
std::string ParticleRegistration2D<T,Descriptor>::getName(int id) const
{
    if (id==0) {
        return std::string("Undefined");
    }
    if (id < 0 || id > (int)particleByNumber.size()) {
        std::stringstream message;
        message << "A particle class with ID " << id << " doesn't exist.";
        plbLogicError(message.str());
    }
    return particleByNumber[id-1].name;
}

template<typename T, template<typename U> class Descriptor>
Particle2D<T,Descriptor>* ParticleRegistration2D<T,Descriptor>::generate
                             ( HierarchicUnserializer& unserializer )
{
    plint id = unserializer.getId();
    PLB_ASSERT (id>0 && (pluint)id <= particleByNumber.size());
    return particleByNumber[id-1].generator->generate(unserializer);
}

template<typename T, template<typename U> class Descriptor>
typename ParticleRegistration2D<T,Descriptor>::EntryMap::const_iterator
    ParticleRegistration2D<T,Descriptor>::begin() const
{
    return particleByName.begin();
}

template<typename T, template<typename U> class Descriptor>
typename ParticleRegistration2D<T,Descriptor>::EntryMap::const_iterator
    ParticleRegistration2D<T,Descriptor>::end() const
{
    return particleByName.end();
}


template<typename T, template<typename U> class Descriptor>
ParticleRegistration2D<T,Descriptor>& particleRegistration2D() {
    static ParticleRegistration2D<T,Descriptor> instance;
    return instance;
}

}  // namespace meta

}  // namespace plb

#endif  // PARTICLE_IDENTIFIERS_2D_HH
