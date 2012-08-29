// Filename: StaggeredPhysicalBoundaryHelper.h
// Created on 22 Jul 2008 by Boyce Griffith
//
// Copyright (c) 2002-2010, Boyce Griffith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of New York University nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef included_StaggeredPhysicalBoundaryHelper
#define included_StaggeredPhysicalBoundaryHelper

/////////////////////////////// INCLUDES /////////////////////////////////////

// SAMRAI INCLUDES
#include <PatchHierarchy.h>
#include <RobinBcCoefStrategy.h>
#include <SideData.h>
#include <Variable.h>
#include <tbox/Pointer.h>

// C++ STDLIB INCLUDES
#include <map>
#include <vector>

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBTK
{
/*!
 * \brief Class StaggeredPhysicalBoundaryHelper provides helper functions
 * to enforce physical boundary conditions for a staggered grid discretizations.
 */
class StaggeredPhysicalBoundaryHelper
    : SAMRAI::tbox::DescribedClass
{
public:
    /*!
     * \brief Default constructor.
     */
    StaggeredPhysicalBoundaryHelper();

    /*!
     * \brief Destructor.
     */
    ~StaggeredPhysicalBoundaryHelper();

    /*!
     * \brief Enforce Dirichlet boundary conditions on the physical boundary
     * over the specified range of levels in the patch hierarchy using the
     * cached boundary data.
     */
    void
    enforceDirichletBcs(
        int u_data_idx,
        bool homogeneous_bcs,
        int coarsest_level_number=-1,
        int finest_ln=-1) const;

    /*!
     * \brief Enforce Dirichlet boundary conditions on the physical boundary on
     * a single patch using the cached boundary data.
     */
    void
    enforceDirichletBcs(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > u_data,
        bool homogeneous_bcs,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch) const;

    /*!
     * \brief Copy data to u_data_out_idx from u_data_in_idx at Dirichlet
     * boundaries over the specified range of levels in the patch hierarchy.
     */
    void
    copyDataAtDirichletBoundaries(
        int u_out_data_idx,
        int u_in_data_idx,
        int coarsest_level_number=-1,
        int finest_ln=-1) const;

    /*!
     * \brief Copy data to u_data_out_idx from u_data_in_idx at Dirichlet
     * boundaries on a single patch.
     */
    void
    copyDataAtDirichletBoundaries(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > u_out_data,
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,double> > u_in_data,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch) const;

    /*!
     * \brief Setup a masking function over the specified range of levels in the
     * patch hierarchy.
     */
    void
    setupMaskingFunction(
        int mask_data_idx,
        int coarsest_level_number=-1,
        int finest_ln=-1) const;

    /*!
     * \brief Setup a masking function on a single patch.
     */
    void
    setupMaskingFunction(
        SAMRAI::tbox::Pointer<SAMRAI::pdat::SideData<NDIM,int> > u_data,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch) const;

    /*!
     * \brief Return a boolean value indicating whether a patch has Dirichlet
     * boundaries.
     */
    bool
    patchHasDirichletBoundary(
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch) const;

    /*!
     * \brief Return a boolean value indicating whether a patch has Dirichlet
     * boundaries in the specified coordinate axis.
     */
    bool
    patchHasDirichletBoundaryAxis(
        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch,
        const unsigned int axis) const;

    /*!
     * \brief Cache boundary coefficient data.
     */
    void
    cacheBcCoefData(
        int u_data_idx,
        SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> > u_var,
        std::vector<SAMRAI::solv::RobinBcCoefStrategy<NDIM>*>& u_bc_coefs,
        double fill_time,
        const SAMRAI::hier::IntVector<NDIM>& gcw_to_fill,
        SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy);

    /*!
     * \brief Clear cached boundary coefficient data.
     */
    virtual void
    clearBcCoefData();

protected:
    /*!
     * Cached hierarchy-related information.
     */
    SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > d_hierarchy;
    std::vector<std::map<int,SAMRAI::tbox::Array<SAMRAI::hier::BoundaryBox<NDIM> > > > d_physical_codim1_boxes;
    std::vector<std::map<int,std::vector<SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,bool  > > > > > d_dirichlet_bdry_locs;
    std::vector<std::map<int,std::vector<SAMRAI::tbox::Pointer<SAMRAI::pdat::ArrayData<NDIM,double> > > > > d_dirichlet_bdry_vals;

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    StaggeredPhysicalBoundaryHelper(
        const StaggeredPhysicalBoundaryHelper& from);

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    StaggeredPhysicalBoundaryHelper&
    operator=(
        const StaggeredPhysicalBoundaryHelper& that);
};
}// namespace IBTK

/////////////////////////////// INLINE ///////////////////////////////////////

//#include <ibtk/StaggeredPhysicalBoundaryHelper.I>

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_StaggeredPhysicalBoundaryHelper
