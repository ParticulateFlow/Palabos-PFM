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

/** \file
 * Utilities to help users handle data processors -- header file.
 *
 * This file offers explicit wrappers to execute data-processors on atomic-
 * blocks, or to integrate the data-processors into the atomic-blocks. A
 * wrapper is offered for explicit choices of data types, for one or two
 * block arguments. These wrappers are luxury: it would have been sufficient
 * to have a generic wrapper which takes a single atomic-block as argument,
 * then a generic wrapper for two atomic-blocks. The advantage of having an
 * explicit wrapper for each choice of data types is type safety. It is
 * guaranteed that the type of atomic-blocks fits the type of the data
 * processor, which avoids awkward runtime errors.
 */

#ifndef REDUCTIVE_DATA_PROCESSOR_WRAPPER_2D_H
#define REDUCTIVE_DATA_PROCESSOR_WRAPPER_2D_H

#include "atomicBlock/reductiveDataProcessingFunctional2D.h"

namespace plb {

/* *************** Generic wrappers, boxed functionals ********************** */

/// This is the most general wrapper for applying a 2D boxed data
/// functional. Use this if none of the more specific wrappers works.
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D& functional,
                               Box2D domain,
                               std::vector<AtomicBlock2D*> atomicBlocks);

/// Apply a functional on a sequence of block-lattices. If the number
/// of lattices is 1 or 2, you should prefer the _L and _LL version
/// of the functional.
template<typename T, template<typename U> class Descriptor>
void applyProcessingFunctional (
        ReductiveLatticeBoxProcessingFunctional2D<T,Descriptor>& functional,
        Box2D domain,
        std::vector<BlockLattice2D<T,Descriptor>*> lattices );

/// Apply a functional on a sequence of scalar-fields. If the number
/// of lattices is 1 or 2, you should prefer the _S and _SS version
/// of the functional.
template<typename T>
void applyProcessingFunctional (
        ReductiveScalarFieldBoxProcessingFunctional2D<T>& functional,
        Box2D domain,
        std::vector<ScalarField2D<T>*> fields );

/// Apply a functional on a sequence of tensor-fields. If the number
/// of lattices is 1 or 2, you should prefer the _T and _TT version
/// of the functional.
template<typename T, int nDim>
void applyProcessingFunctional (
        ReductiveTensorFieldBoxProcessingFunctional2D<T,nDim>& functional,
        Box2D domain,
        std::vector<TensorField2D<T,nDim>*> fields );

/// Apply a functional on a sequence of tensor-fields. If the number
/// of lattices is 1 or 2, you should prefer the _N and _NN version
/// of the functional.
template<typename T>
void applyProcessingFunctional (
        ReductiveNTensorFieldBoxProcessingFunctional2D<T>& functional,
        Box2D domain,
        std::vector<NTensorField2D<T>*> fields );

/* *************** Typed wrappers with a single argument, boxed functionals * */

template<typename T, template<typename U> class Descriptor>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_L<T,Descriptor>& functional,
                               Box2D domain, BlockLattice2D<T,Descriptor>& lattice);

template<typename T>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_S<T>& functional,
                               Box2D domain, ScalarField2D<T>& field);

template<typename T, int nDim>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_T<T,nDim>& functional,
                               Box2D domain, TensorField2D<T,nDim>& field);

template<typename T>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_N<T>& functional,
                               Box2D domain, NTensorField2D<T>& field);

/* *************** Typed wrappers with two arguments ************************ */

template<typename T1, template<typename U1> class Descriptor1,
         typename T2, template<typename U2> class Descriptor2>
void applyProcessingFunctional (
        ReductiveBoxProcessingFunctional2D_LL<T1,Descriptor1,T2,Descriptor2>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor1>& lattice1,
        BlockLattice2D<T2,Descriptor2>& lattice2 );

template<typename T1, typename T2>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_SS<T1,T2>& functional,
                               Box2D domain,
                               ScalarField2D<T1>& field1,
                               ScalarField2D<T2>& field2);

template<typename T1, int nDim1, typename T2, int nDim2>
void applyProcessingFunctional (
        ReductiveBoxProcessingFunctional2D_TT<T1,nDim1,T2,nDim2>& functional,
        Box2D domain,
        TensorField2D<T1,nDim1>& field1,
        TensorField2D<T2,nDim2>& field2 );

template<typename T1, typename T2, int nDim>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_ST<T1,T2,nDim>& functional,
                               Box2D domain,
                               ScalarField2D<T1>& field1,
                               TensorField2D<T2,nDim>& field2);

template<typename T1, typename T2>
void applyProcessingFunctional (
        ReductiveBoxProcessingFunctional2D_NN<T1,T2>& functional,
        Box2D domain,
        NTensorField2D<T1>& field1,
        NTensorField2D<T2>& field2 );

template<typename T1, typename T2>
void applyProcessingFunctional(ReductiveBoxProcessingFunctional2D_SN<T1,T2>& functional,
                               Box2D domain,
                               ScalarField2D<T1>& field1,
                               NTensorField2D<T2>& field2);

template<typename T1, template<typename U> class Descriptor, typename T2>
void applyProcessingFunctional (
        ReductiveBoxProcessingFunctional2D_LS<T1,Descriptor,T2>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor>& lattice,
        ScalarField2D<T2>& field );

template<typename T1, template<typename U> class Descriptor, typename T2, int nDim>
void applyProcessingFunctional (
        ReductiveBoxProcessingFunctional2D_LT<T1,Descriptor,T2,nDim>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor>& lattice,
        TensorField2D<T2,nDim>& field );

template<typename T1, template<typename U> class Descriptor, typename T2>
void applyProcessingFunctional (
        ReductiveBoxProcessingFunctional2D_LN<T1,Descriptor,T2>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor>& lattice,
        NTensorField2D<T2>& field );


/* *************** Generic wrappers, dotted functionals ********************* */

void applyProcessingFunctional(ReductiveDotProcessingFunctional2D& functional,
                               DotList2D const& dotList,
                               std::vector<AtomicBlock2D*> atomicBlocks);

template<typename T, template<typename U> class Descriptor>
void applyProcessingFunctional (
        ReductiveLatticeDotProcessingFunctional2D<T,Descriptor>& functional,
        DotList2D const& dotList,
        std::vector<BlockLattice2D<T,Descriptor>*> lattices );

template<typename T>
void applyProcessingFunctional (
        ReductiveScalarFieldDotProcessingFunctional2D<T>& functional,
        DotList2D const& dotList,
        std::vector<ScalarField2D<T>*> fields );

template<typename T, int nDim>
void applyProcessingFunctional (
        ReductiveTensorFieldDotProcessingFunctional2D<T,nDim>& functional,
        DotList2D const& dotList,
        std::vector<TensorField2D<T,nDim>*> fields );

template<typename T>
void applyProcessingFunctional (
        ReductiveNTensorFieldDotProcessingFunctional2D<T>& functional,
        DotList2D const& dotList,
        std::vector<NTensorField2D<T>*> fields );


/* *************** Typed wrappers with a single argument, dotted functionals* */

template<typename T, template<typename U> class Descriptor>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_L<T,Descriptor>& functional,
        DotList2D const& dotList,
        BlockLattice2D<T,Descriptor>& lattice );

template<typename T>
void applyProcessingFunctional(ReductiveDotProcessingFunctional2D_S<T>& functional,
                               DotList2D const& dotList,
                               ScalarField2D<T>& field);

template<typename T, int nDim>
void applyProcessingFunctional(ReductiveDotProcessingFunctional2D_T<T,nDim>& functional,
                               DotList2D const& dotList,
                               TensorField2D<T,nDim>& field);

template<typename T>
void applyProcessingFunctional(ReductiveDotProcessingFunctional2D_N<T>& functional,
                               DotList2D const& dotList,
                               NTensorField2D<T>& field);

/* *************** Typed wrappers with two arguments, dotted functionals* *****/

/// Easy instantiation of dotted data processor for lattice-lattice coupling
template<typename T1, template<typename U1> class Descriptor1,
         typename T2, template<typename U2> class Descriptor2>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_LL<T1,Descriptor1,T2,Descriptor2>& functional,
        DotList2D const& dotList,
        BlockLattice2D<T1,Descriptor1>& lattice1,
        BlockLattice2D<T2,Descriptor2>& lattice2 );

template<typename T1, typename T2>
void applyProcessingFunctional(ReductiveDotProcessingFunctional2D_SS<T1,T2>& functional,
                               DotList2D const& dotList,
                               ScalarField2D<T1>& field1,
                               ScalarField2D<T2>& field2);

/// Easy instantiation of dotted data processor for TensorField-TensorField coupling
template<typename T1, int nDim1, typename T2, int nDim2>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_TT<T1,nDim1,T2,nDim2>& functional,
        DotList2D const& dotList,
        TensorField2D<T1,nDim1>& field1,
        TensorField2D<T2,nDim2>& field2 );

/// Easy instantiation of dotted data processor for NTensorField-NTensorField coupling
template<typename T1, typename T2>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_NN<T1,T2>& functional,
        DotList2D const& dotList,
        NTensorField2D<T1>& field1,
        NTensorField2D<T2>& field2 );

template<typename T1, typename T2, int nDim>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_ST<T1,T2,nDim>& functional,
        DotList2D const& dotList,
        ScalarField2D<T1>& field1,
        TensorField2D<T2,nDim>& field2 );

template<typename T1, typename T2>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_SN<T1,T2>& functional,
        DotList2D const& dotList,
        ScalarField2D<T1>& field1,
        NTensorField2D<T2>& field2 );

template<typename T1, template<typename U> class Descriptor, typename T2>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_LS<T1,Descriptor,T2>& functional,
        DotList2D const& dotList,
        BlockLattice2D<T1,Descriptor>& lattice,
        ScalarField2D<T2>& field );

template<typename T1, template<typename U> class Descriptor, typename T2, int nDim>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_LT<T1,Descriptor,T2,nDim>& functional,
        DotList2D const& dotList,
        BlockLattice2D<T1,Descriptor>& lattice,
        TensorField2D<T2,nDim>& field );

template<typename T1, template<typename U> class Descriptor, typename T2>
void applyProcessingFunctional (
        ReductiveDotProcessingFunctional2D_LN<T1,Descriptor,T2>& functional,
        DotList2D const& dotList,
        BlockLattice2D<T1,Descriptor>& lattice,
        NTensorField2D<T2>& field );

/* *************** Generic wrappers, bounded and boxed functionals ********** */

void applyProcessingFunctional(BoundedReductiveBoxProcessingFunctional2D& functional,
                               Box2D domain, std::vector<AtomicBlock2D*> atomicBlocks,
                               plint boundaryWidth );

template<typename T, template<typename U> class Descriptor>
void applyProcessingFunctional (
        BoundedReductiveLatticeBoxProcessingFunctional2D<T,Descriptor>& functional,
        Box2D domain,
        std::vector<BlockLattice2D<T,Descriptor>*> lattices,
        plint boundaryWidth = Descriptor<T>::boundaryWidth );

template<typename T>
void applyProcessingFunctional (
        BoundedReductiveScalarFieldBoxProcessingFunctional2D<T>& functional,
        Box2D domain, std::vector<ScalarField2D<T>*> fields,
        plint boundaryWidth );

template<typename T, int nDim>
void applyProcessingFunctional (
        BoundedReductiveTensorFieldBoxProcessingFunctional2D<T,nDim>& functional,
        Box2D domain, std::vector<TensorField2D<T,nDim>*> fields,
        plint boundaryWidth );

template<typename T>
void applyProcessingFunctional (
        BoundedReductiveNTensorFieldBoxProcessingFunctional2D<T>& functional,
        Box2D domain, std::vector<NTensorField2D<T>*> fields,
        plint boundaryWidth );


/* ** Typed wrappers with a single argument, bounded and boxed functionals * */

template<typename T, template<typename U> class Descriptor>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_L<T,Descriptor>& functional,
        Box2D domain,
        BlockLattice2D<T,Descriptor>& lattice,
        plint boundaryWidth = Descriptor<T>::boundaryWidth );

template<typename T>
void applyProcessingFunctional(BoundedReductiveBoxProcessingFunctional2D_S<T>& functional,
                               Box2D domain, ScalarField2D<T>& field,
                               plint boundaryWidth);


/* ** Typed wrappers with two arguments, bounded and boxed functionals *** */

template<typename T1, template<typename U1> class Descriptor1,
         typename T2, template<typename U2> class Descriptor2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_LL<T1,Descriptor1,T2,Descriptor2>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor1>& lattice1,
        BlockLattice2D<T2,Descriptor2>& lattice2,
        plint boundaryWidth = Descriptor1<T1>::boundaryWidth );

template<typename T1, typename T2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_SS<T1,T2>& functional,
        Box2D domain,
        ScalarField2D<T1>& field1,
        ScalarField2D<T2>& field2,
        plint boundaryWidth );

template<typename T1, int nDim1, typename T2, int nDim2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_TT<T1,nDim1,T2,nDim2>& functional,
        Box2D domain,
        TensorField2D<T1,nDim1>& field1,
        TensorField2D<T2,nDim2>& field2,
        plint boundaryWidth );

template<typename T1, typename T2, int nDim>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_ST<T1,T2,nDim>& functional,
        Box2D domain,
        ScalarField2D<T1>& field1,
        TensorField2D<T2,nDim>& field2,
        plint boundaryWidth );

template<typename T1, int nDim1, typename T2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_NN<T1,T2>& functional,
        Box2D domain,
        NTensorField2D<T1>& field1,
        NTensorField2D<T2>& field2,
        plint boundaryWidth );

template<typename T1, typename T2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_SN<T1,T2>& functional,
        Box2D domain,
        ScalarField2D<T1>& field1,
        NTensorField2D<T2>& field2,
        plint boundaryWidth );

template<typename T1, template<typename U> class Descriptor, typename T2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_LS<T1,Descriptor,T2>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor>& lattice,
        ScalarField2D<T2>& field,
        plint boundaryWidth = Descriptor<T1>::boundaryWidth );

template<typename T1, template<typename U> class Descriptor, typename T2, int nDim>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_LT<T1,Descriptor,T2,nDim>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor>& lattice,
        TensorField2D<T2,nDim>& field,
        plint boundaryWidth = Descriptor<T1>::boundaryWidth );

template<typename T1, template<typename U> class Descriptor, typename T2>
void applyProcessingFunctional (
        BoundedReductiveBoxProcessingFunctional2D_LN<T1,Descriptor,T2>& functional,
        Box2D domain,
        BlockLattice2D<T1,Descriptor>& lattice,
        NTensorField2D<T2>& field,
        plint boundaryWidth = Descriptor<T1>::boundaryWidth );

}  // namespace plb

#endif  // REDUCTIVE_DATA_PROCESSOR_WRAPPER_2D_H
