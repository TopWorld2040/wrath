/*! 
 * \file WRATHGradientSourceBase.hpp
 * \brief file WRATHGradientSourceBase.hpp
 * 
 * Copyright 2013 by Nomovok Ltd.
 * 
 * Contact: info@nomovok.com
 * 
 * This Source Code Form is subject to the
 * terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with
 * this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 * 
 * \author Kevin Rogovin <kevin.rogovin@nomovok.com>
 * 
 */




#ifndef __WRATH_GRADIENT_SOURCE_BASE_HPP__
#define __WRATH_GRADIENT_SOURCE_BASE_HPP__


#include "WRATHConfig.hpp"
#include "WRATHImage.hpp"
#include "WRATHBaseSource.hpp"
#include "WRATHGradient.hpp"

namespace WRATHGradientSourceBasePrivate
{
  class NonLinearFacade;
}

/*! \addtogroup Imaging
 * @{
 */

/*!\class WRATHGradientSourceBase
  A WRATHGradientSourceBase represents how to
  compute the interpolate value to feed
  to the texture look up for \ref WRATHGradient.
  The class WRATHGradientSourceBase defines
  an interface an convention for computing a gradient
  interpolate value as follows:

  - A gradient interpolate may be computed entirely
    linearly, partially linearly and fully non-linearly.
    This is controlled by the enumeration \ref
    interpolation_behaviour_t

  - A gradient interpolate is implemented in GLSL by
    implementing 2 functions: pre_compute_gradient()
    and compute_gradient(). 

  The interpolation behavior can be linear, partially-nonlinear
  and fully non-linear. The caller requests the nature
  of the interpolation behavior. An implementation can
  promote the interpolation behavior, where the lowest
  linear < partially-nonlinear < fully nonlinear,
  for example if partially-nonlinear is requested,
  then the implementation may choose to implement as 
  partially-nonlinear or fully nonlinear.

  A class derived from WRATHGradientSourceBase must
  abide by the following conventions:
  - When the interpolation behavior is linear,
    implement the function 
    \code
    float compute_gradient(in vec2 p)
    \endcode
    in the vertex shader. The coordinate p
    is in item local coordinates

  - When the interpolation behavior is partially
    non-linear, implement the function
    \code   
    void pre_compute_gradient(in vec2 p)
    \endcode
    in the vertex shader and the function
    \code
    float compute_gradient(in vec2 p)
    \endcode
    in the fragment shader. The coordinate p
    is in item local coordinates

  - When the interpolation behavior is fully
    non-linear, implement the function
    \code   
    void pre_compute_gradient(void)
    \endcode
    in the vertex shader and the function
    \code
    float compute_gradient(in vec2 p)
    \endcode
    in the fragment shader. The coordinate p
    is in item local coordinates


  The class \ref WRATHShaderBrushSourceHoard, in implementing the 
  GLSL code for brush functions, obey the added macros LINEAR_GRADIENT, 
  NON_LINEAR_GRADIENT and FULLY_NON_LINEAR_GRADIENT by adding \#ifdef's checking 
  for those macros when calling the functions compute_gradient() 
  and pre_compute_gradient(). These macros are added
  by \ref add_shader_source_code_specify_interpolation() when the
  passed parameter suffic is empty as follows:
  - WRATHBaseSource::linear_computation: LINEAR_GRADIENT in vertex and fragment shader
  - WRATHBaseSource::nonlinear_computation NON_LINEAR_GRADIENT in vertex and fragment shader
  - WRATHBaseSource::fully_nonlinear_computation NON_LINEAR_GRADIENT and FULLY_NON_LINEAR_GRADIENT in vertex and fragment shader
 */
class WRATHGradientSourceBase:public WRATHBaseSource
{
public:

  /*!\fn WRATHGradientSourceBase(void)
    Public ctor.
   */ 
  WRATHGradientSourceBase(void);

  virtual
  ~WRATHGradientSourceBase();

  /*!\fn enum interpolation_behaviour_t adjust_interpolation_behavior
    To be implemented by a derived class to adjust the
    intepolation behavior to what the implementation
    can accept. The return value must be greater
    than or equal to the input value, where:
    \code
    WRATHBaseSource::linear_computation <= WRATHBaseSource::nonlinear_computation <= WRATHBaseSource::fully_nonlinear_computation
    \endcode
    \param ibt interpolation behavior to potentially promote
   */
  virtual
  enum interpolation_behaviour_t
  adjust_interpolation_behavior(enum interpolation_behaviour_t ibt) const=0;


  /*!\fn void add_shader_source_code_specify_interpolation
    Adds the GLSL code to compute a gradient interpolate
    with the specified the interpolation behavior. 
    \param ibt interpolation behavior of the computation
               of the gradient interpolate. It is an error
               if adjust_interpolation_behavior(ibt)!=ibt.
    \param src an std::map keyed by shader type with values
               of shader source code to which to add source code
    \param prec precision qaulifier to use
    \param suffix suffix to which to append to all function, macros, etc
                  added to the GLSL code, including the functions
                  compute_gradient() and pre_compute_gradient(). 
                  A non-empty suffix indicates that the functions
                  are being chained from another function, in this
                  case none of the macros LINEAR_GRADIENT, NON_LINEAR_GRADIENT
                  and FULLY_NON_LINEAR_GRADIENT will be added
   */
  void
  add_shader_source_code_specify_interpolation(enum interpolation_behaviour_t ibt,
                                               std::map<GLenum, WRATHGLShader::shader_source> &src,
                                               enum precision_t prec,
                                               const std::string &suffix) const;

  /*!\fn const WRATHGradientSourceBase* non_linear_facade
    Returns a WRATHGradientSourceBase using the same
    underlying code of this WRATHGradientSourceBase,
    but forces the interpolation mode to be 
    \ref WRATHBaseSource::fully_nonlinear_computation
   */
  const WRATHGradientSourceBase*
  non_linear_facade(void) const
  {
    return m_fully_non_linear_facade;
  }

protected:

  virtual
  void
  add_shader_source_code_implement(std::map<GLenum, WRATHGLShader::shader_source> &src,
                                   enum precision_t prec,
                                   const std::string &suffix) const;

  /*!\fn add_shader_source_code_specify_interpolation_implementation
    To be implemented by a derived class to add the 
    GLSL code to compute a gradient interpolate
    with the specified the interpolation behavior. 
    \param ibt interpolation behavior of the computation
               of the gradient interpolate. It is an guaranteed 
               that adjust_interpolation_behavior(ibt)==ibt.
    \param src an std::map keyed by shader type with values
               of shader source code to which to add source code
    \param prec precision qaulifier to use
    \param suffix suffix to which to append to all symbols of GLSL shaders
                  that are at global scope, including the functions
                  compute_gradient() and pre_compute_gradient()
   */
  virtual
  void
  add_shader_source_code_specify_interpolation_implementation(enum interpolation_behaviour_t ibt,
                                                              std::map<GLenum, WRATHGLShader::shader_source> &src,
                                                              enum precision_t prec,
                                                              const std::string &suffix) const=0;

private:
  friend class WRATHGradientSourceBasePrivate::NonLinearFacade;

  enum is_facade_t
    {
      is_facade
    };

  //different ctor to ctop creation of facade.
  WRATHGradientSourceBase(enum is_facade_t);


  WRATHGradientSourceBase *m_fully_non_linear_facade;

};
/*! @} */

#endif
